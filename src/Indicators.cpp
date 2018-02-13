#include "Indicators.h"

Indicators::Indicators()
{

    m_wifiLedGpio = new GPIOClass(WIFI_LED_GPIO);
    m_e131DataLedGpio = new GPIOClass(E131_DATA_LED_GPIO);
    m_recplayRedLedGpio = new GPIOClass(RECPLAY_RED_LED_GPIO);
    m_recplayGreenLedGpio = new GPIOClass(RECPLAY_GREEN_LED_GPIO);
    m_modeRedLedGpio = new GPIOClass(MODE_RED_LED_GPIO);
    m_modeGreenLedGpio = new GPIOClass(MODE_GREEN_LED_GPIO);

    m_wifiLedGpio->export_gpio();
    m_wifiLedGpio->setdir_gpio("out");

    m_e131DataLedGpio->export_gpio();
    m_e131DataLedGpio->setdir_gpio("out");

    m_recplayRedLedGpio->export_gpio();
    m_recplayRedLedGpio->setdir_gpio("out");

    m_recplayGreenLedGpio->export_gpio();
    m_recplayGreenLedGpio->setdir_gpio("out");
    
    m_modeRedLedGpio->export_gpio();
    m_modeRedLedGpio->setdir_gpio("out");

    m_modeGreenLedGpio->export_gpio();
    m_modeGreenLedGpio->setdir_gpio("out");


    std::thread record_playback_thread(Indicators::update_record_playback_led);
    std::thread wifi_thread(Indicators::update_wifi_led);
    record_playback_thread.join();
    wifi_thread.join();
}

void Indicators::set_e131_data_recieved()
{
    m_e131_data_mutex.lock();
    m_e131_data = true;
    m_e131_data_mutex.unlock();
}

void Indicators::set_rec_playback_led(RecPlayback t_state)
{
    m_rec_playback_state_mutex.lock();
    m_rec_playback_state = t_state;
    m_rec_playback_state_mutex.unlock();
}

void Indicators::set_mode_led(Mode t_state)
{
    switch(t_state){
        case Mode.Record:
        m_modeRedLedGpio->setval_gpio("1");
        m_modeGreenLedGpio->setval_gpio("0");
        break;

        case Mode.Playback:
        m_modeRedLedGpio->setval_gpio("0");
        m_modeGreenLedGpio->setval_gpio("1");
        break;

        case Mode.Live:
        m_modeRedLedGpio->setval_gpio("1");
        m_modeGreenLedGpio->setval_gpio("1");
        break;
    }
}

void Indicators::teardown()
{
    m_running = false;
}

void Indicators::update_wifi_led()
{
    //Todo move to main

    //This LED should be active when the wifi is active (connected to a )
    bool connected = false; 
    while(m_running){
        //check to see if there is a ssid connected to
        //check once a second until connected, then switch to once every 5 seconds
        if(@connected){
            usleep(1000000);
        }else{
            usleep(5 * 1000000);
        }

        std::string response = Util::exec("iwgetid -r");

        if(response.length() > 0){
            //Some ssid, is connected
            m_wifiLedGpio->setval_gpio("1");
        }else{
            m_wifiLedGpio->setval_gpio("0");
        }
    }

    if(!m_running){
        //turn off led on teardown
        m_wifiLedGpio->setval_gpio("0");
    }
}

void Indicators::update_e131_data_received_led()
{
    //set the led on for a minimum of 100ms, turn off automatically if not called again
    while(m_running){

    }

    if(!m_running){
        m_e131DataLedGpio->setval_gpio("0");
    }
}

void Indicators::update_record_playback_led()
{
    //update the record playback LED's
    while(m_running){
        m_rec_playback_state_mutex.lock();
        switch(rec_playback_state){
            case RecPlayback.Stopped:
                //Turn off led
                m_recplayGreenLedGpio->setval_gpio("0");
                m_recplayRedLedGpio->setval_gpio("0");
                //Only update every 100ms
                usleep(1000000);
            break;
            case RecPlayback.Recording:
                //turn led RED
                m_recplayGreenLedGpio->setval_gpio("0");
                m_recplayRedLedGpio->setval_gpio("1");
                //Only update every 100ms
            usleep(1000000);
            break;
            case RecPlayback.RecordWait:
                //slow blink, waiting for data to begin recording
                m_recplayGreenLedGpio->setval_gpio("0");
                m_recplayRedLedGpio->setval_gpio("1");
                usleep(250000);
                m_recplayRedLedGpio->setval_gpio("0");
                usleep(500000);
            break;

            case RecPlayback.RecordingFinished:
                //three quick blinks, indicates the recording has stopped
                m_recplayGreenLedGpio->setval_gpio("0");
                for(int i = 0; i < 3; i++){
                    m_recplayRedLedGpio->setval_gpio("0");
                    usleep(125000);
                    m_recplayRedLedGpio->setval_gpio("1");
                    usleep(250000);
                }
                m_rec_playback_state = RecPlayback.Stopped;
            break;
            case RecPlayback.Playing:
                //three quick blinks, indicates the recording has stopped
                m_recplayGreenLedGpio->setval_gpio("1");
                m_recplayRedLedGpio->setval_gpio("0");
                usleep(1000000);
            break;
        }
        m_rec_playback_state_mutex.unlock();
    }

    if(!m_running){
        //turn all led's off before shutting down
        m_recplayGreenLedGpio->setval_gpio("0");
        m_recplayRedLedGpio->setval_gpio("0");
    }
}