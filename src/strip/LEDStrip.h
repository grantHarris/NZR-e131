#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__

#include <stdint.h>
#include <unistd.h>
#include <mutex>
#include <err.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
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
        virtual void LEDStrip::render(bool *running);
        virtual void LEDStrip::write_pixels_to_buffer(Pixel * pixels, int len);
};

#endif /* __LEDSTRIP_H__ */