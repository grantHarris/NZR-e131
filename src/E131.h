#ifndef __E131_H__
#define __E131_H__
#include "yaml-cpp/yaml.h"
#include <e131.h>
#include <err.h>
#include <mutex>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "Util.h"
#include "LEDStrip.h"

using namespace boost::log;
namespace logging = boost::log;

class E131 {
public:
    E131(YAML::Node& t_config, LEDStrip& t_led_strip);
    void close();
    void receive_data(bool *running);
private:
    YAML::Node config;
    LEDStrip& led_strip;
    
    std::mutex output_mutex;
    void join_universe(int t_universe);
    int sockfd;
    e131_packet_t packet;
    e131_error_t error;
    uint8_t last_seq = 0x00;
};

#endif /* __E131_H__ */
