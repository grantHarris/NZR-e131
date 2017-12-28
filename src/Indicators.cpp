#include "Indicators.h"

class Indicators{
	enum class RecPlayback{
		Stopped,
		Recording,
		Playing,
		RecordWait
		RecordingFinished
	}

	enum class Mode{
		Record,
		Playback,
		Live
	}

}

std::mutex m;

GPIOClass* gpio4;

Indicators::Indicators(){

	wifiLedGpio = new GPIOClass(WIFI_LED_GPIO);
	e131DataLedGpio = new GPIOClass(E131_DATA_LED_GPIO);
	recplayRedLedGpio = new GPIOClass(RECPLAY_RED_LED_GPIO);
	recplayGreenLedGpio = new GPIOClass(RECPLAY_GREEN_LED_GPIO);
	modeRedLedGpio = new GPIOClass(MODE_RED_LED_GPIO);
	modeGreenLedGpio = new GPIOClass(MODE_GREEN_LED_GPIO);

    wifiLedGpio->export_gpio();
    wifiLedGpio->setdir_gpio("out");

    e131DataLedGpio->export_gpio();
    e131DataLedGpio->setdir_gpio("out");

    recplayRedLedGpio->export_gpio();
    recplayRedLedGpio->setdir_gpio("out");

    recplayGreenLedGpio->export_gpio();
    recplayGreenLedGpio->setdir_gpio("out");
    
    modeRedLedGpio->export_gpio();
    modeRedLedGpio->setdir_gpio("out");

    modeGreenLedGpio->export_gpio();
    modeGreenLedGpio->setdir_gpio("out");


	std::thread record_playback_thread(Indicators::update_record_playback_led);
	std::thread wifi_thread(Indicators::update_wifi_led);
	record_playback_thread.join();
	wifi_thread.join();
}

void Indicators::set_e131_data_recieved(){
	//set the led on for a minimum of 100ms, turn off automatically if not called again

}

void Indicators::set_rec_playback_led(RecPlayback state){
	this->m.lock();
	this->rec_playback_state = state;
	this->m.unlock();
	//do a thread for controlling the led

	//mutex

}

void Indicators::teardown(){
	this->running = false;
}

void Indicators::update_wifi_led(){
	//This LED should be active when the wifi is active (connected to a )
	while(this->running){
		//check to see if there is a ssid connected to
		//check once a second until connected, then switch to once every 5 seconds
		usleep(5 * 1000000);
	}

	if(!this->running){
		//turn off led on teardown
	}
}

void Indicators::update_record_playback_led(){
	//update the record playback LED's
	while(this->running){
		this->m.lock();
		switch(rec_playback_state){
			case RecPlayback.Stopped:
				//turn off led
				recplayGreenLedGpio->setval_gpio("0");
				recplayRedLedGpio->setval_gpio("0");
				usleep(1000000);
			break;
			case RecPlayback.Recording:
				//turn led RED
				recplayGreenLedGpio->setval_gpio("0");
				recplayRedLedGpio->setval_gpio("1");
				usleep(1000000);
			break;
			case RecPlayback.RecordWait:
				//slow blink, waiting for data to begin recording
				recplayGreenLedGpio->setval_gpio("0");
				recplayRedLedGpio->setval_gpio("1");
				usleep(250000);
				recplayRedLedGpio->setval_gpio("0");
				usleep(500000);
			break;

			case RecPlayback.RecordingFinished:
				//three quick blinks, indicates the recording has stopped
				recplayGreenLedGpio->setval_gpio("0");
				for(int i = 0; i < 3; i++){
					recplayRedLedGpio->setval_gpio("0");
					usleep(125000);
					recplayRedLedGpio->setval_gpio("1");
					usleep(250000);
				}
				rec_playback_state = RecPlayback.Stopped;
			break;
			case RecPlayback.Playing:
				//three quick blinks, indicates the recording has stopped
				recplayGreenLedGpio->setval_gpio("1");
				recplayRedLedGpio->setval_gpio("0");
				usleep(1000000);
			break;
		}
		this->m.unlock();
	}

	if(!this->running){
		//turn all led's off before shutting down
	}
}