#include "WS2811Strip.h"

WS2811Strip::WS2811Strip(YAML::Node& t_config) {
    output.freq = TARGET_FREQ;
    output.dmanum = DMA;

    int gpio_pin = it->t_config["gpionum"].as<int>();
    int count = it->t_config["count"].as<int>();
    int invert = it->t_config["invert"].as<int>();
    int brightness = it->t_config["brightness"].as<int>();

    output.channel[ch].gpionum = gpio_pin;
    output.channel[ch].count = count;
    output.channel[ch].invert = invert;
    output.channel[ch].brightness = brightness;
    output.channel[ch].strip_type = STRIP_TYPE;

    BOOST_LOG_TRIVIAL(info) 
    << "Output on Channel: " << ch 
    << ", GPIO: " << gpio_pin
    << ", Total LED's: " << count
    << ", Invert: " << invert
    << ", Brightness: " << brightness;

    ws2811_return_t ret;
    if ((ret = ws2811_init(&output)) != WS2811_SUCCESS){
        BOOST_LOG_TRIVIAL(fatal) << "ws2811 init failed:" << ws2811_get_return_t_str(ret);
        exit(1);
    }
}


void WS2811Strip::write_pixels_to_strip(std::vector<Pixel>& t_pixels){
	for(uint16_t i = 0; i < t_pixels.size(); i++){
		this->write_to_output_buffer(0i, t_pixels[i]);
	}

    if ((ret = ws2811_render(&output)) != WS2811_SUCCESS){
    	BOOST_LOG_TRIVIAL(error) << "ws2811 render frame failed:" << ws2811_get_return_t_str(ret);
    }
}


void WS2811Strip::write_to_output_buffer(int strip_channel, int index, Pixel pixel){
    output.channel[strip_channel].leds[index] = 
    static_cast<uint32_t>(pixel.r << 16) |
    static_cast<uint32_t>(pixel.g << 8) |
    static_cast<uint32_t>(pixel.b);
    
    BOOST_LOG_TRIVIAL(trace) 
    << "Ch: " << strip_channel 
    << ", Index: " << index
    << ", R: " << static_cast<int>(pixel.r)
    << ", G: " << static_cast<int>(pixel.g)
    << ", B: " << static_cast<int>(pixel.b);
}

