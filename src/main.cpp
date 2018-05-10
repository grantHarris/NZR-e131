#include <cstdio>
#include <stdexcept>

#include <iostream>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "yaml-cpp/yaml.h"
#include <boost/log/utility/setup/file.hpp>

#include "E131.h"
#include "LEDStrip.h"
#include "APA102Strip.h"
#include "WS2811Strip.h"
#include "Playback.h"

namespace po = boost::program_options;
using namespace boost::log;
namespace logging = boost::log;

bool running;
YAML::Node config;
Apa102Strip* apa102_strip;
Playback* playback;

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

void setup_logging(po::variables_map& vm){
    if (vm.count("log")){
        //log specified here, save to file
        logging::add_file_log(vm["log"].as<std::string>());
    }
    
    int verbosity;
    if (vm.count("verbosity")){
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
    }else{
        verbosity = logging::trivial::info;
    }

    logging::core::get()->set_filter(logging::trivial::severity >= verbosity);
}

int main(int argc, char* argv[]) {
    boost::thread_group threads;
    try {
        
        setup_handlers();

        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "Produce help message")
        ("config,c", po::value<std::string>()->default_value("./config.yaml"), "Config file path")
        ("save_location,s", po::value<std::string>()->default_value("./save"), "Save location")
        ("log,l", po::value<std::string>(), "Logging file path")
        ("stats,s", po::value<std::string>(), "Output update stats for E1.31 updates")
        ("verbosity,v", po::value<std::string>()->default_value("info"), "Enable verbosity (optionally specify level)");

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }

        setup_logging(vm);

        BOOST_LOG_TRIVIAL(info) << "Using config file " << vm["config"].as<std::string>();
        config = YAML::LoadFile(vm["config"].as<std::string>());

        E131 e131(config);
        running = true;

        threads.create_thread(boost::bind(&E131::receive_data, &e131, &running));
        
        if(config["strip_type"].as<std::string>() == "APA102"){
            BOOST_LOG_TRIVIAL(info) << "Using APA102 strip";
            apa102_strip = new Apa102Strip(&running);
            threads.add_thread(apa102_strip->thread);
            BOOST_LOG_TRIVIAL(debug) << "APA102 strip set up";
            e131.register_update_fn(boost::bind(&Apa102Strip::push_frame, apa102_strip, _1));
        }else{
           BOOST_LOG_TRIVIAL(info) << "Using WS2811 strip";
           //WS2811Strip ws2811_strip(config);
           //e131.register_update_fn(boost::bind(&WS2811Strip::push, &ws2811_strip));
           BOOST_LOG_TRIVIAL(debug) << "WS2811 strip set up";
        }

        if(vm.count("stats")){
            BOOST_LOG_TRIVIAL(info) << "Stats enabled";
            threads.create_thread(boost::bind(&E131::stats_thread, &e131, &running));
        }

        if (vm.count("save_location")) {
            BOOST_LOG_TRIVIAL(info) << "Save location: " << config["save_location"].as<std::string>();
            //playback = new Playback(config["save_location"].as<std::string>());
        }

        threads.join_all();

    }

    catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Error: " << e.what() << std::endl;
        return 1;
    }

    catch(...) {
        BOOST_LOG_TRIVIAL(fatal) << "Exception of unknown type!" << std::endl;
    }

    return 0;
}
