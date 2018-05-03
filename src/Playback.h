#ifndef __Playback_H__
#define __Playback_H__
#include <fstream>
#include <chrono>
#include <vector>
#include <queue>
#include <string>
#include "LEDStrip.h"
#include "leveldb/db.h"

enum class PlaybackState {
    STOPPED,
    PAUSED,
    PLAYING,
    RECORDING
};

struct Index{
    unsigned int chapter;
    unsigned int playhead;
};

class Playback {
    public:
        Playback(void) {}
        void record();
        void play();
        void stop();
        void pause();
        void toggle_loop(bool t_loop);
        std::queue<std::vector<Pixel>> frame_queue;
        void set_state(PlaybackState state);
    private:
    	mutable boost::mutex frame_mutex;
	mutable boost::mutex state_mutex;
    	void play_loop();
        void record_loop();
        bool loop;
        Index index;
        auto start_time;
        int playhead;
        std::function<void(std::vector<Pixel>&)> callback;
        PlaybackState current_state;
        leveldb::DB* db;
        boost::thread* record_thread;
        boost::thread* playback_thread;
};

#endif /* __Playback_H__ */
