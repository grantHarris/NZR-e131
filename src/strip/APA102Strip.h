#ifndef __APA102_H__
#define __APA102_H__
#include <bcm2835.h>
#include <stdio.h>

using namespace boost::log;
namespace logging = boost::log;

class Apa102Strip {
public:
	Apa102Strip()
	~Apa102Strip();
	static void write(Pixel * pixels, uint16_t count, uint8_t brightness = 31);
	static void send_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 31);
	static void send_color(Pixel pixel, uint8_t brightness = 31);
private:
	static void send_start_frame();
	static void send_end_frame(uint16_t count);
	uint8_t brightness;
	uint16_t count;
};

#endif /* __APA102_H__ */
