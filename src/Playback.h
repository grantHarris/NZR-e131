#ifndef __Playback_H__
#define __Playback_H__
#include <fstream>

enum State {
    STOPPED,
    PAUSED,
    PLAYING,
    CUE_RECORD
    RECORD_WAIT
};

class Playback {
    public:
        Playback(void) {}
        void record();
        void play();
        void pause();
        void next();
        void loop(bool t_loop);
        State get_state();
        unsigned int get_index();
        unsigned int get_total();
        double get_size(unsigned int index);
    private:
        std::function<void(std::vector<Pixel>&)> callback;
        State current_state;
        leveldb::DB* db;
        int framerate
        bool loop;
};

#endif /* __Playback_H__ */