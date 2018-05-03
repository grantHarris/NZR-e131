#ifndef __Playback_H__
#define __Playback_H__
#include <fstream>
#include <chrono>

enum State {
    STOPPED,
    PAUSED,
    PLAYING,
    CUE_RECORD
    RECORD_WAIT
};

struct Index{
    unsigned int chapter;
    unsigned int playhead;
}

class Playback {
    public:
        Playback(void) {}
        void record();
        void play();
        void pause();
        void loop(bool t_loop);
        std::queue<std::vector<Pixel>> frame_queue;
    private:
        bool loop;
        Index index;
        auto start_time;
        int playhead;
        std::function<void(std::vector<Pixel>&)> callback;
        State current_state;
        leveldb::DB* db;
};

#endif /* __Playback_H__ */