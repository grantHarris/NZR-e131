#include "GPIO.h"
#include "pins.h"


#include "../lib/clk.h"
#include "../lib/gpio.h"
#include "../lib/dma.h"
#include "../lib/pwm.h"
#include "../lib/version.h"
#include "../lib/ws2811.h"

#ifndef __BUTTONS_H__
#define __BUTTONS_H__


class Buttons {
    public:
        Buttons();
    private:
        GPIO* m_mode_button_Gpio;
        GPIO* m_rec_play_button_Gpio;
        GPIO* m_next_button_Gpio;
        GPIO* m_hand_trigger_1_Gpio;
        GPIO* m_hand_trigger_2_Gpio;
 };



#endif /* __BUTTONS_H__ */