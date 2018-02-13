#include "WS2811.h"

void WS2811::WS2811(YAML::Node& t_config){
    config = t_config;
    ws2811_return_t ret;
    if ((ret = ws2811_init(&output)) != WS2811_SUCCESS){
        BOOST_LOG_TRIVIAL(fatal) << "ws2811_init failed:" << ws2811_get_return_t_str(ret);
        exit(1);
    }
}

void WS2811::render() {
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

void WS2811::setup_ouput(){
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