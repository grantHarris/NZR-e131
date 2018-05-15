#ifndef __Playback_H__
#define __Playback_H__

#include <stdint.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <queue>
#include <string>

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
        Playback(std::string file_name);
        ~Playback();
        void push_frame(std::vector<Pixel>& t_pixels);
        void record();
        void play();
        void stop();
        void pause();
        void toggle_loop(bool t_loop);
        std::queue<nzr::Frame> frame_queue;
        void set_state(PlaybackState state);
    private:
        mutable std::mutex frame_mutex;
        mutable std::mutex state_mutex;
        void play_thread();
        void record_thread();
        void live_thread();
        bool loop;
        Index index;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
        std::string* playhead;
        PlaybackState current_state;
        leveldb::DB* db;
        std::thread* record_thread;
        std::thread* playback_thread;
        std::condition_variable wait_for_frame;
};

#endif /* __Playback_H__ */
