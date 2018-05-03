#include "Playback.h"
#include "leveldb/db.h"

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param file_name Location of the leveldb directory
 */
Playback::Playback(std::string file_name){
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, file_name, &db);
    if (!s.ok()) BOSST_LOG_TRVIAL(error) << s.ToString();

    //eventually load from config/saved state
    playhead = 0;
    current_state = State.STOPPED;
}

/**
 * @brief Starts recording
 * @details [long description]
 */
void Playback::record(){
    if (current_state == State.RECORDING){
        return;
    }
    
    if(current_state == State.STOPPED){
        //start new playback thread
        record_thread = new boost::thread(boost::bind(&Playback::record_loop, this));
        start_time = std::chrono::steady_clock::now();
    }

    this->set_state(State.RECORDING);
}

/**
 * @brief Sets if playback should loop on end
 * @details [long description]
 * 
 * @param t_loop boolean
 */
void Playback::toggle_loop(bool t_loop){
    loop = t_loop
}

/**
 * @brief Starts playback
 * @details [long description]
 */
void Playback::play(){
    if (current_state == State.PLAYING){
        return;
    }

    if(current_state == State.STOPPED){
        //start new playback thread
        playback_thread = new boost::thread(boost::bind(&Playback::play_loop, this));
    }

    this->set_state(State.PLAYING);
}

/**
 * @brief Pauses playback
 * @details [long description]
 */
void Playback::pause(){
    this->set_state(State.PAUSED);
}

/**
 * @brief Stops playback
 * @details [long description]
 */
void Playback::stop(){
    if (current_state == State.RECORDING){
        record_thread->join();
    }
    if (current_state == State.PLAYING){
        playback_thread->join();
    }
    this->set_state(State.STOPPED);
    index.playhead = 0;
}

/**
 * @brief Set the current state of playback. eg Playing
 * @details [long description]
 * 
 * @param state [description]
 */
void Playback::set_state(State state){
    boost::unique_lock<boost::mutex> lock(the_mutex);
    current_state = state
    lock.unlock();
}


void Playback::register_update_fn(std::function<void(std::vector<Pixel>&)> t_callback){
    callback = t_callback;
}

/**
 * @brief Record loop function
 * @details [long description]
 */
void Playback::record_loop(){
    boost::unique_lock<boost::mutex> lock(frame_mutex);
    while(current_state == State.RECORDING)
    {
        while(frame_queue.empty())
        {
            if(current_state == State.RECORDING){
                wait_for_frame.wait(lock);
            }else{
                lock.unlock();
                break;
            }
        }

        index.playhead = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - start_time);
        db->Put(leveldb::WriteOptions(), index.playhead, frame_queue.front());
        frame_queue.pop();
        BOOST_LOG_TRIVIAL(debug) << "Record frame at: " << index.playhead;
    }

}

/**
 * @brief Playback loop function
 * @details Loops through the leveldb file with an iterator
 */
void Playback::play_loop(){
    boost::unique_lock<boost::mutex> lock(the_mutex);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    while((loop && current_state == State.PLAYING)){
        for (it->Seek(index.playhead); current_state == State.PLAYING, it->Valid(); it->Next()) {
            callback(it->value());
            index.playhead = it->key();
            BOOST_LOG_TRIVIAL(debug) << "Play frame at: " << index.playhead;
        }
    }
    delete it;
}

Playback::~Playback(){
    delete db;
}
