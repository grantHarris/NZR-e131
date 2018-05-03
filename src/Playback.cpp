#include "Playback.h"
#include "leveldb/db.h"

Playback::Playback(std::string file_name){
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, file_name, &db);
    if (!s.ok()) BOSST_LOG_TRVIAL(error) << s.ToString();
}

void Playback::cue_record(){
    this->set_state(State.CUE_RECORD);
}

void Playback::record(){
    if (current_state == State.RECORDING){
        return;
    }
    this->set_state(State.RECORDING);
}

void Playback::loop(bool t_loop){
    loop = t_loop
}

void Playback::play(){
    if (current_state == State.PLAYING){
        return;
    }

    if(current_state == State.STOPPED){
        //start new playback thread
        thread = new boost::thread(boost::bind(&Playback::play_loop, this));
    }

    this->set_state(State.PLAYING);
    pause_condition_variable.notify_one();
}

void Playback::pause(){
    this->set_state(State.PAUSED);
}

void Playback::stop(){
    this->set_state(State.STOPPED);
}

void Playback::set_state(State state){
    boost::unique_lock<boost::mutex> lock(the_mutex);
    current_state = state
    lock.unlock();
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

void Playback::register_update_fn(std::function<void(std::vector<Pixel>&)> t_callback){
    callback = t_callback;
}

void Playback::play_loop()_{
    boost::unique_lock<boost::mutex> lock(the_mutex);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    while((loop && current_state == State.PLAYING)){
        for (it->Seek(1); current_state == State.PLAYING, it->Valid() && it->key().ToString() < limit; it->Next()) {
            pause_condition_variable.wait(lock);
            callback(it->value());
        }
    }
    delete it;
}

Playback::~Playback(){
    delete db;
}