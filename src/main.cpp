#define _HAS_ITERATOR_DEBUGGING 0

#include <utility>
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

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "yaml-cpp/yaml.h"
#include <boost/log/utility/setup/file.hpp>

#include <condition_variable>
#include <thread>
#include <functional>
#include <mutex>
#include <future>

#include "E131.h"
#include "LEDStrip.h"
#include "APA102Strip.h"
#include "WS2811Strip.h"

#include "Playback.h"

#include <ncurses.h>

namespace po = boost::program_options;
using namespace boost::log;
namespace logging = boost::log;

YAML::Node config;
bool running = true;

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
    
    int level;
    if (vm.count("level")){
        std::string levelStr = vm["level"].as<std::string>();

        if(levelStr == "trace"){
            level = logging::trivial::trace;
        }else if(levelStr == "debug"){
            level = logging::trivial::debug;
        }else if(levelStr == "info"){
            level = logging::trivial::info;
        }else if(levelStr == "warning"){ 
            level = logging::trivial::warning;
        }else if(levelStr == "error"){
            level = logging::trivial::error;
        }else if(levelStr == "fatal"){
            level = logging::trivial::error;
        }else{
            level = logging::trivial::info;
        }
    }else{
        level = logging::trivial::info;
    }

    logging::core::get()->set_filter(logging::trivial::severity >= level);
}

void boostrap_strip(po::variables_map& vm, YAML::Node& config, LEDStrip&& strip){
    E131 e131(config);
    Playback playback(std::move(e131), std::move(strip));
    std::vector<std::thread> thread_list;


    if (vm.count("file")) {
        BOOST_LOG_TRIVIAL(info) << "File location: " << vm["file"].as<std::string>();
        playback.set_file_location(vm["file"].as<std::string>());
    }

    // Strip render thread
    std::thread strip_pop_and_display_frame_thread([&](){
        strip.pop_and_display_frame();
    });
    thread_list.push_back(std::move(strip_pop_and_display_frame_thread));

    // Playback thread. Handles live frame, recording, playback
    std::thread playback_loop_thread([&](){
        playback.thread_loop();
    });
    thread_list.push_back(std::move(playback_loop_thread));


    //Todo : clean below up and put into functions
    if (vm.count("mode")){
        std::string levelStr = vm["mode"].as<std::string>();

        if(levelStr == "record"){
            e131.start();
            BOOST_LOG_TRIVIAL(info) << "Starting recording from live";
            // Stats thread
            if(vm.count("stats")){
                BOOST_LOG_TRIVIAL(info) << "Stats enabled";
                std::thread e131_stats_thread([&](){
                    e131.stats_thread();
                });
                thread_list.push_back(std::move(e131_stats_thread));
            }
            std::thread e131_receive_data_thread([&](){
                e131.receive_data();
            });
            thread_list.push_back(std::move(e131_receive_data_thread));
            playback.record_from_live();
        }else if(levelStr == "playback"){
            BOOST_LOG_TRIVIAL(info) << "Starting playback";
            playback.start_playback();
        }else{
            BOOST_LOG_TRIVIAL(info) << "Starting live";
            // Stats thread
            if(vm.count("stats")){
                BOOST_LOG_TRIVIAL(info) << "Stats enabled";
                std::thread e131_stats_thread([&](){
                    e131.stats_thread();
                });
                thread_list.push_back(std::move(e131_stats_thread));
            }
            std::thread e131_receive_data_thread([&](){
                e131.receive_data();
            });
            thread_list.push_back(std::move(e131_receive_data_thread));
            playback.play_live();
        }
    }else{
        BOOST_LOG_TRIVIAL(info) << "Starting live";
        // Stats thread
        if(vm.count("stats")){
            BOOST_LOG_TRIVIAL(info) << "Stats enabled";
            std::thread e131_stats_thread([&](){
                e131.stats_thread();
            });
            thread_list.push_back(std::move(e131_stats_thread));
        }
        std::thread e131_receive_data_thread([&](){
            e131.receive_data();
        });
        thread_list.push_back(std::move(e131_receive_data_thread));
        playback.play_live();
    }


    //initscr();
    while(running == true){
        usleep(5000000);
        // char c = getch();
        // switch(c){
        //     case 'r':{
        //         playback.record_from_live();
        //         break;
        //     }
        //     case 'p':{
        //         playback.start_playback();
        //         break;
        //     }

        //     case 'a':{
        //         playback.pause_playback();
        //         break;
        //     }
            
        //     case 's':{
        //         playback.stop_playback();
        //         break;
        //     }
        //     case 'l':{
        //         playback.play_live();
        //         break;
        //     }
        // }
    }
    //endwin();

    playback.stop();
    e131.stop();
    strip.stop();

    std::for_each(thread_list.begin(), thread_list.end(), std::mem_fn(&std::thread::join));
}

int main(int argc, char* argv[]) {
        
    try {
        
        setup_handlers();

        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,h", "Produce help message")
        ("config,c", po::value<std::string>()->default_value("./config.yaml"), "Config file path")
        ("log,l", po::value<std::string>(), "Logging file path")
        ("mode,m", po::value<std::string>(), "Mode to start in")
        ("stats,s", po::value<std::string>(), "Output update stats for E1.31 updates")
        ("file,f", po::value<std::string>(), "File name where the replays are stored")
        ("level,v", po::value<std::string>()->default_value("info"), "Enable level (optionally specify level)");

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
    
        if(vm.count("apa102")){
            BOOST_LOG_TRIVIAL(info) << "Using APA102 strip";
            Apa102Strip apa102_strip;
            boostrap_strip(vm, config, std::move(apa102_strip));
        }else{
            BOOST_LOG_TRIVIAL(info) << "Using WS2811 Strip";
            WS2811Strip ws2811_strip(config);
            boostrap_strip(vm, config, std::move(ws2811_strip));
        }

    }

    catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch(...) {
        BOOST_LOG_TRIVIAL(fatal) << "Exception of unknown type!" << std::endl;
        return 1;
    }

    return 0;
}

