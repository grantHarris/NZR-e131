#include "Buttons.h"

Buttons::Buttons(){
	m_mode_button_Gpio = new GPIOClass(MODE_BUTTON);
	m_rec_play_button_Gpio = new GPIOClass(REC_PLAY_BUTTON);
	m_next_button_Gpio = new GPIOClass(NEXT_BUTTON);
	m_hand_trigger_1_Gpio = new GPIOClass(HAND_TRIGGER_1);
	m_hand_trigger_1_Gpio = new GPIOClass(HAND_TRIGGER_2);

    m_mode_button_Gpio->export_gpio();
    m_mode_button_Gpio->setdir_gpio("in");

    m_rec_play_button_Gpio->export_gpio();
    m_rec_play_button_Gpio->setdir_gpio("in");

    m_next_button_Gpio->export_gpio();
    m_next_button_Gpio->setdir_gpio("in");

    m_hand_trigger_1_Gpio->export_gpio();
    m_hand_trigger_1_Gpio->setdir_gpio("in");

    m_hand_trigger_1_Gpio->export_gpio();
    m_hand_trigger_1_Gpio->setdir_gpio("in");
}