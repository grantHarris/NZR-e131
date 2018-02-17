#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__

#define DMA             10
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      WS2811_STRIP_GRB

#include <stdint.h>
#include <mutex>
#include <err.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include "../lib/ws2811.h"
#include "../lib/clk.h"
#include "../lib/gpio.h"
#include "../lib/dma.h"
#include "../lib/pwm.h"
#include "../lib/version.h"
#include "yaml-cpp/yaml.h"

using namespace boost::log;
namespace logging = boost::log;

struct Pixel {
	uint32_t r;
	uint32_t g;
	uint32_t b;
};

class LEDStrip {
    public:
    	LEDStrip(YAML::Node& t_config);
    	void render();
    	void write_to_buffer(int strip_channel, int index, Pixel pixel);
    private:
        ws2811_t output;
        YAML::Node config;
        std::mutex output_mutex;
        void setup_ouput();
        bool running = false;
};

#endif /* __LEDSTRIP_H__ */