#ifndef __E131_H__
#define __E131_H__
#include "yaml-cpp/yaml.h"
#include <e131.h>
#include <err.h>
#include <mutex>
#include <vector>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

//#include "leveldb/db.h"
#include "LEDStrip.h"
#include "Util.h"

using namespace boost::log;
namespace logging = boost::log;

struct UniverseStats {
    unsigned int out_of_sequence;
    unsigned int updates;
};

enum State {
    GOOD,
    OUT_OF_SEQUENCE
};

class E131 {
public:
    E131(YAML::Node& t_config);
    void receive_data(bool *running);
    void stats_thread(bool *running);
    void map_to_buffer(e131_packet_t &packet);
    void read_from_file();
    void save_to_file(e131_packet_t &packet);
    void register_update_fn(std::function<void()> t_callback);
private:
    std::function<void()> callback;
    std::map<unsigned int, UniverseStats> universe_stats;
    std::map<unsigned int, unsigned int> sequence_numbers;
    YAML::Node config;
    std::mutex log_mutex;
    int sockfd;
    e131_packet_t packet;
    e131_error_t error;
    uint8_t last_seq = 0x00;
    bool recording = false;
    std::vector<Pixel> pixels;
    void register_universe_for_stats(unsigned int t_universe);
    void join_universe(int t_universe);
    void log_universe_packet(unsigned int t_universe, State state);
};

#endif /* __E131_H__ */
