#ifndef __Playback_H__
#define __Playback_H__

#include <fstream>
#include <chrono>
#include <vector>
#include <queue>
#include <string>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "LEDStrip.h"
#include "leveldb/db.h"
#include "frame.pb.h"

using namespace boost::log;
namespace logging = boost::log;
using namespace nzr;

enum class PlaybackState {
    STOPPED,
    PAUSED,
    PLAYING,
    RECORDING
};

struct Index{
    unsigned int chapter;
    std::string playhead;
};

class Playback {
    public:
        Playback(std::string file_name);
        ~Playback();
        void record();
        void play();
        void stop();
        void pause();
        void toggle_loop(bool t_loop);
        std::queue<nzr::Frame> frame_queue;
        void set_state(PlaybackState state);
        void push_frame(std::vector<Pixel> const& t_pixels);
    private:
        mutable boost::mutex frame_mutex;
        mutable boost::mutex state_mutex;
        void play_loop();
        void record_loop();
        bool loop;
        Index index;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        std::string* playhead;
        PlaybackState current_state;
        leveldb::DB* db;
        boost::thread* record_thread;
        boost::thread* playback_thread;
        boost::condition_variable wait_for_frame;
};

#endif /* __Playback_H__ */
