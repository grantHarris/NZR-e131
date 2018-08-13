#ifndef __Playback_H__
#define __Playback_H__

#include <stdint.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <queue>
#include <string>
#include <utility>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <boost/lexical_cast.hpp>

#include <condition_variable>
#include <thread>
#include <functional>
#include <mutex>
#include <future>

#include "leveldb/db.h"
#include "frame.pb.h"

#include "LEDStrip.h"
#include "Stoppable.h"
#include "LEDStrip.h"

#include "E131.h"

using namespace boost::log;
namespace logging = boost::log;

enum class PlaybackState {
    STOPPED,
    PAUSED,
    PLAYING,
    LIVE,
    RECORDING
};

struct Index{
    unsigned int chapter;
    std::string playhead;
};

class Playback : public Stoppable {
    public:
        Playback(E131&& t_e131, LEDStrip&& t_strip);
        ~Playback();
        void set_file_location(std::string file_name);
        void toggle_loop(bool t_loop);
        void record_from_live();
        void play_live();
        void start_playback();
        void pause_playback();
        void stop_playback();
        void set_state(PlaybackState state);
        void record_to_file(std::vector<Pixel>& t_pixels);
        void play_from_file();
        void thread_loop();
        void live_stream();

    private:
        E131&& e131;
        LEDStrip&& strip;

        mutable std::mutex frame_mutex;
        mutable std::mutex state_mutex;
        void play_from_file_thread();
        void record_to_file_thread();
        void live_stream_thread();
        bool loop;
        Index index;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        std::string* playhead;
        PlaybackState current_state;
        leveldb::DB* db;
        std::condition_variable wait_for_frame;
};

#endif /* __Playback_H__ */
