#include "Playback.h"
#include "leveldb/db.h"

Playback::Playback(std::string file_name){
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, file_name, &db);
}

void Playback::cue_record(){
	current_state = State.CUE_RECORD;
}

void Playback::record(){
	current_state = State.RECORDING;
}

void Playback::play(){
	current_state = State.PLAYING;
}

void Playback::pause(){
	current_state = State.PAUSED;
}

void Playback::stop(){
	current_state = State.STOPPED;
}

void Playback::next(){

}

State Playback::get_state(){
	return current_state
}

unsigned int Playback::get_index(){

}

unsigned int Playback::get_total(){

}

double Playback::get_size(unsigned int index){

}

void Playback::sample_from_stream(){

}

void Playback::loop()_{
	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->Seek(1); it->Valid() && it->key().ToString() < limit; it->Next()) {
	  cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
	  usleep();
	}
	delete it;
}

Playback::~Playback(){
	delete db;
}