#include <cstdio>
#include <stdexcept>

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

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "yaml-cpp/yaml.h"

#include "E131.h"
#include "LEDStrip.h"

namespace po = boost::program_options;
using namespace boost::log;
namespace logging = boost::log;

static bool running;
YAML::Node config;

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

        LEDStrip led_strip(config);
        E131 e131(config, led_strip);

        setup_handlers();

        // running = true;

        // std::thread render_thread(led_strip.render);
        // render_thread.join();

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
