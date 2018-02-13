#include "GPIO.h"
#include "pins.h"

#ifndef __BUTTONS_H__
#define __BUTTONS_H__

namespace e131Bridge{

    class Indicators {
    public:
        Indicators();
    private:
        GPIOClass* m_mode_button_Gpio;
        GPIOClass* m_rec_play_button_Gpio;
        GPIOClass* m_next_button_Gpio;
        GPIOClass* m_hand_trigger_1_Gpio;
        GPIOClass* m_hand_trigger_1_Gpio;
    };

}

#endif /* __BUTTONS_H__ */