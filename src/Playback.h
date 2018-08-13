#define _HAS_ITERATOR_DEBUGGING 0

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
#include "leveldb/comparator.h"
#include "frame.pb.h"

#include "LEDStrip.h"
#include "Stoppable.h"
#include "LEDStrip.h"

#include "E131.h"

using namespace boost::log;
namespace logging = boost::log;

class Comparator : public leveldb::Comparator {
 public:
  int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
    double a1 = boost::lexical_cast<double>(a.ToString());
    double b1 = boost::lexical_cast<double>(b.ToString());

    if (a1 < b1) return -1;
    if (a1 > b1) return +1;
    return 0;
  }

  // Ignore the following methods for now:
  const char* Name() const { return "Comparator"; }
  void FindShortestSeparator(std::string*, const leveldb::Slice&) const {}
  void FindShortSuccessor(std::string*) const {}
};


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
        Comparator cmp;
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
        PlaybackState current_state;
        leveldb::DB* db;
        std::condition_variable wait_for_frame;
};

#endif /* __Playback_H__ */
