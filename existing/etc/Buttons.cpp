#include "Buttons.h"

Buttons::Buttons()
{
    m_mode_button_Gpio = new GPIO(MODE_BUTTON_GPIO);
    m_rec_play_button_Gpio = new GPIO(REC_PLAY_BUTTON_GPIO);
    m_next_button_Gpio = new GPIO(NEXT_BUTTON_GPIO);
    m_hand_trigger_1_Gpio = new GPIO(HAND_TRIGGER_1_GPIO);
    m_hand_trigger_2_Gpio = new GPIO(HAND_TRIGGER_2_GPIO);

    m_mode_button_Gpio->export_gpio();
    m_mode_button_Gpio->setdir_gpio("in");

    m_rec_play_button_Gpio->export_gpio();
    m_rec_play_button_Gpio->setdir_gpio("in");

    m_next_button_Gpio->export_gpio();
    m_next_button_Gpio->setdir_gpio("in");

    m_hand_trigger_1_Gpio->export_gpio();
    m_hand_trigger_1_Gpio->setdir_gpio("in");

    m_hand_trigger_2_Gpio->export_gpio();
    m_hand_trigger_2_Gpio->setdir_gpio("in");
}