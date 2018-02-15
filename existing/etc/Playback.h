#ifndef __Playback_H__
#define __Playback_H__

enum State {
    STOPPED,
    PAUSED,
    PLAYING,
    RECORDING
};

class Playback {
    public:
        Playback(void) {}
        void record();
        void play();
        void pause();
        void next();
        State get_state();
        unsigned int get_index();
        unsigned int get_total();
        double get_size(unsigned int index);
    private:
        State current_state;
};

#endif /* __Playback_H__ */