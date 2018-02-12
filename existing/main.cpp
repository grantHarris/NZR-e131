#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include <iostream>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <thread>
#include <mutex>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <e131.h>
#include <err.h>

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "lib/clk.h"
#include "lib/gpio.h"
#include "lib/dma.h"
#include "lib/pwm.h"
#include "lib/version.h"
#include "lib/ws2811.h"
#include "yaml-cpp/yaml.h"

namespace po = boost::program_options;
using namespace boost::log;
namespace logging = boost::log;
#define DMA             10
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      WS2811_STRIP_GRB

static bool running;
YAML::Node config;
ws2811_t output;

int sockfd;
e131_packet_t packet;
e131_error_t error;
uint8_t last_seq = 0x00;

std::mutex output_mutex;

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    
    if (!pipe){
        throw std::runtime_error("popen() failed");
    }

    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr){
            result += buffer.data();
        }
    }
    return result;
}

void setup_ouput(){
    output.freq = TARGET_FREQ;
    output.dmanum = DMA;
    YAML::Node strip_channel = config["strip_channel"];
    for(YAML::const_iterator it=strip_channel.begin(); it!=strip_channel.end(); ++it) {
        int ch = it->first.as<int>();

        int gpio_pin = it->second["gpionum"].as<int>();
        int count = it->second["count"].as<int>();
        int invert = it->second["invert"].as<int>();
        int brightness = it->second["brightness"].as<int>();

        output.channel[ch].gpionum = gpio_pin;
        output.channel[ch].count = count;
        output.channel[ch].invert = invert;
        output.channel[ch].brightness = brightness;
        output.channel[ch].strip_type = STRIP_TYPE;
    }
}

static void sig_handler(int t_signum){
    (void)(t_signum);
    running = false;
}

static void setup_handlers(void){
    struct sigaction action;
    action.sa_handler = &sig_handler;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

void receive_data() {
    int strip_channel, start_address, total_rgb_channels, end_address, start_address_offset;
    
    while(running == true) {
        if (e131_recv(sockfd, &packet) < 0)
            err(EXIT_FAILURE, "e131_recv failed");

        if ((error = e131_pkt_validate(&packet)) != E131_ERR_NONE) {
            BOOST_LOG_TRIVIAL(error) << "e131 packet validation error" << e131_strerror(error);
            continue;
        }

        if (e131_pkt_discard(&packet, last_seq)) {
            BOOST_LOG_TRIVIAL(warning) << "e131 packet received out of sequence. Last: " 
            << static_cast<int>(last_seq) << ", Seq: " << static_cast<int>(packet.frame.seq_number);
            last_seq = packet.frame.seq_number;
            continue;
        }

        int universe = ntohs(packet.frame.universe);
        BOOST_LOG_TRIVIAL(debug) << "Packet for universe: " << universe;

        output_mutex.lock();
        YAML::Node universeConfig = config["mapping"][universe];
        for(YAML::const_iterator it = universeConfig.begin(); it != universeConfig.end(); ++it) {
            const YAML::Node& entry = *it;
            YAML::Node output_params = entry["output"];
            YAML::Node input_params = entry["input"];

            strip_channel = output_params["strip_channel"].as<int>();
            start_address = std::max(1, std::min(input_params["start_address"].as<int>(), 511));
            total_rgb_channels = std::max(0, std::min(input_params["total_rgb_channels"].as<int>() - 1, 511));
            end_address = std::max(1, std::min(total_rgb_channels - start_address, 511));
            start_address_offset = output_params["start_address"].as<int>();
            
            for(int i = start_address - 1; i < end_address; i++){
                int index = i * 3 + 1;
                uint8_t r = packet.dmp.prop_val[index];
                uint8_t g = packet.dmp.prop_val[index + 1];
                uint8_t b = packet.dmp.prop_val[index + 2];

                BOOST_LOG_TRIVIAL(trace) 
                << "Channel: " << strip_channel 
                << ", LED Index: " << i + start_address_offset
                << ", R: " << static_cast<int>(r)
                << ", G: " << static_cast<int>(g)
                << ", B: " << static_cast<int>(b);

                output.channel[strip_channel].leds[i + start_address_offset] = 
                static_cast<uint32_t>(r << 16) |
                static_cast<uint32_t>(g << 8) |
                static_cast<uint32_t>(b);
            }
        }
        last_seq = packet.frame.seq_number;
        output_mutex.unlock();
    }
}

void render_ws2811() {
    ws2811_return_t ret;
    
    while(running == true){
        output_mutex.lock();
        if ((ret = ws2811_render(&output)) != WS2811_SUCCESS){
            BOOST_LOG_TRIVIAL(error) << "ws2811_render failed:" << ws2811_get_return_t_str(ret);
        }
        output_mutex.unlock();
        usleep(1000000 / 30);
    }

    if(running == false){
        output_mutex.lock();
        ws2811_fini(&output);
        output_mutex.unlock();
    }

}

void join_universe(int t_universe){

    BOOST_LOG_TRIVIAL(info) << "Joining universe: " << t_universe;

    std::stringstream ss;
    ss << "sudo ip maddr add 239.255.0." << t_universe << " dev wlan0";
    
    BOOST_LOG_TRIVIAL(debug) << "Executing shell command: " << ss.str();
    exec(ss.str().c_str());

    if (e131_multicast_join(sockfd, t_universe) < 0)
        err(EXIT_FAILURE, "e131_multicast_join failed");   
}

void setup_e131(){
    // create a socket for E1.31
    if ((sockfd = e131_socket()) < 0)
        err(EXIT_FAILURE, "e131_socket failed");

    // bind the socket to the default E1.31 port
    if (e131_bind(sockfd, E131_DEFAULT_PORT) < 0)
        err(EXIT_FAILURE, "e131_bind failed");
}

void setup_ws2811(){
    ws2811_return_t ret;
    if ((ret = ws2811_init(&output)) != WS2811_SUCCESS){
        BOOST_LOG_TRIVIAL(fatal) << "ws2811_init failed:" << ws2811_get_return_t_str(ret);
        exit(1);
    }

    for(YAML::const_iterator it=config["mapping"].begin(); it != config["mapping"].end(); ++it) {
        int universe = it->first.as<int>();
        join_universe(universe);
    }
}

int main(int argc, char* argv[]) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "Produce help message")
        ("config,c", po::value<std::string>()->default_value("./config.yaml"), "Config file path")
        ("log,l", po::value<std::string>(),
          "Logging file path")
        ("verbosity,v", po::value<std::string>()->implicit_value("info"),
          "Enable verbosity (optionally specify level)")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }

        if (vm.count("log")){
            //log specified here, save to file
            //logging::add_file_log(vm.count("log"));
        }

        int verbosity;
        std::string verbosityStr = vm["verbosity"].as<std::string>();

        if(verbosityStr == "trace"){
            verbosity = logging::trivial::trace;
        }else if(verbosityStr == "debug"){
            verbosity = logging::trivial::debug;
        }else if(verbosityStr == "info"){
            verbosity = logging::trivial::info;
        }else if(verbosityStr == "warning"){ 
            verbosity = logging::trivial::warning;
        }else if(verbosityStr == "error"){
            verbosity = logging::trivial::error;
        }else if(verbosityStr == "fatal"){
            verbosity = logging::trivial::error;
        }else{
            verbosity = logging::trivial::info;
        }

        logging::core::get()->set_filter(logging::trivial::severity >= verbosity);

        BOOST_LOG_TRIVIAL(info) << "Using config file " << vm["config"].as<std::string>();
        config = YAML::LoadFile(vm["config"].as<std::string>());

        setup_handlers();
        setup_ouput();
        setup_e131();
        setup_ws2811();

        running = true;

        std::thread thread1(render_ws2811);
        std::thread thread2(receive_data);

        thread1.join();
        thread2.join();
    }

    catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "error: " << e.what() << std::endl;
        return 1;
    }

    catch(...) {
        BOOST_LOG_TRIVIAL(fatal) << "Exception of unknown type!" << std::endl;
    }

    return 0;
}
