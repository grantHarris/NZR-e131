#ifndef __WS2811_H__
#define __WS2811_H__

#define DMA             10
#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      WS2811_STRIP_GRB

class WS2811 {
    public:
    	WS2811();
    	void render();
    	void setup_ouput();
    private:
        ws2811_t output;
        YAML::Node* config;
};

#endif /* __WS2811_H__ */