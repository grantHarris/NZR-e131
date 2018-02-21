#include "Playback.h"


Playback::Playback(std::string file_name){

}

void Playback::record(){
	std::ofstream ofs("filename");
	boost::archive::text_oarchive oa(ofs);

}

void Playback::play(){

}

void Playback::pause(){

}

void Playback::next(){

}

State Playback::get_state(){

}

unsigned int Playback::get_index(){

}

unsigned int Playback::get_total(){

}

double Playback::get_size(unsigned int index){

}