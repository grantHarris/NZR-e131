#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include <iostream>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <thread>
#include <mutex>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <e131.h>
#include <err.h>
#include "lib/clk.h"
#include "lib/gpio.h"
#include "lib/dma.h"
#include "lib/pwm.h"
#include "lib/version.h"
#include "lib/ws2811.h"
#include "yaml-cpp/yaml.h"

#define TARGET_FREQ 	WS2811_TARGET_FREQ
#define DMA 			5
#define STRIP_TYPE      WS2811_STRIP_GRB

ws2811_t output;
static bool running;
YAML::Node config;

int sockfd;
e131_packet_t packet;
e131_error_t error;
uint8_t last_seq = 0x00;
std::mutex m;

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	
	if (!pipe){
		throw std::runtime_error("popen() failed!");
	}

	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr){
			result += buffer.data();
		}
	}
	return result;
}

void setup_ouput_channels(){
	YAML::Node strip_channel = config["strip_channel"];
	for(YAML::const_iterator it=strip_channel.begin(); it!=strip_channel.end(); ++it) {
		int ch = it->first.as<int>();

		int gpio_pin = it->second["gpionum"].as<int>();
		int count = it->second["count"].as<int>();
		int invert = it->second["invert"].as<int>();
		int brightness = it->second["brightness"].as<int>();

		output.channel[ch].gpionum = gpio_pin;
		output.channel[ch].count = count;
		output.channel[ch].invert = invert;
		output.channel[ch].brightness = brightness;
		output.channel[ch].strip_type = STRIP_TYPE;
	}
}

static void sig_handler(int signum){
	(void)(signum);
	running = false;
}

static void setup_handlers(void){
	struct sigaction action;
	action.sa_handler = &sig_handler;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
}

void receive_data() {
	int strip_channel, start_address, total_rgb_channels, end_address, start_address_offset;
	while(running == true) {
		if (e131_recv(sockfd, &packet) < 0)
			err(EXIT_FAILURE, "e131_recv");

		if ((error = e131_pkt_validate(&packet)) != E131_ERR_NONE) {
			fprintf(stderr, "e131_pkt_validate: %s\n", e131_strerror(error));
			continue;
		}
		if (e131_pkt_discard(&packet, last_seq)) {
			fprintf(stderr, "warning: packet out of order received\n");
			last_seq = packet.frame.seq_number;
			continue;
		}
		m.lock();

		YAML::Node universe = config["mapping"][ntohs(packet.frame.universe)];
		
		//std::cout << "Got data for universe: " << universe << std::endl;
		for(YAML::const_iterator it = universe.begin(); it != universe.end(); ++it) {

			const YAML::Node& entry = *it;
			YAML::Node output_params = entry["output"];
			YAML::Node input_params = entry["input"];

			strip_channel = output_params["strip_channel"].as<int>();
			start_address = std::max(1, std::min(input_params["start_address"].as<int>(), 511));
			total_rgb_channels = std::max(0, std::min(input_params["total_rgb_channels"].as<int>() - 1, 511));
			end_address = std::max(1, std::min(total_rgb_channels - start_address, 511));
			start_address_offset = output_params["start_address"].as<int>();
			//e131_pkt_dump(stderr, &packet);
			for(int i = start_address - 1; i < end_address; i++){
				
				//std::cout<< "Value: " << (int) packet.dmp.prop_val[i*3 + 1] << "for" << i*3 + 1 << std::endl;
				output.channel[strip_channel].leds[i + start_address_offset] = 
				(uint32_t) packet.dmp.prop_val[i * 3 + 1] << 16 |
				(uint32_t) packet.dmp.prop_val[i * 3 + 2] << 8 |
				(uint32_t) packet.dmp.prop_val[i * 3 + 3];
			}
		}
		last_seq = packet.frame.seq_number;
		m.unlock();
	}
}

void render_ws2811() {
	ws2811_return_t ret;
	std::cout<<"render thread"<<std::endl;
	while(running == true){
		m.lock();
		if ((ret = ws2811_render(&output)) != WS2811_SUCCESS){
			std::cout << "ws2811_render failed:" << ws2811_get_return_t_str(ret);
		}
		m.unlock();
		usleep(1000000 / 30);
	}
	if(running == false){
		std::cout << "Quit" << std::endl;
		ws2811_fini(&output);
	}

}

int main() {

	running = true;
	config = YAML::LoadFile("config.yaml");
	ws2811_return_t ret;
	
	output.freq = TARGET_FREQ;
	output.dmanum = DMA;

	setup_ouput_channels();
	
	if ((ret = ws2811_init(&output)) != WS2811_SUCCESS){
		std::cout << "ws2811_init failed:" << ws2811_get_return_t_str(ret);
		exit(1);
	}

	setup_handlers();

	// create a socket for E1.31
	if ((sockfd = e131_socket()) < 0)
		err(EXIT_FAILURE, "e131_socket");

	// bind the socket to the default E1.31 port
	if (e131_bind(sockfd, E131_DEFAULT_PORT) < 0)
		err(EXIT_FAILURE, "e131_bind");

	for(YAML::const_iterator it=config["mapping"].begin(); it!=config["mapping"].end(); ++it) {
		int universe = it->first.as<int>();
		// join multicast group for universes
		std::cout<< "Joining as universe: " << universe<<std::endl;
		std::stringstream ss;
		ss << "sudo ip maddr add 239.255.0." << universe << " dev wlan0";
		std::cout << "Executing shell command: " << ss.str() << std::endl;
		std::cout << exec(ss.str().c_str()) << std::endl;

		if (e131_multicast_join(sockfd, universe) < 0)
			err(EXIT_FAILURE, "e131_multicast_join");
	}

	std::thread thread1(render_ws2811);
	std::thread thread2(receive_data);

	thread1.join();
	thread2.join();

}
