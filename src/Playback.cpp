#include "Playback.h"

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
    if (!status.ok()) BOOST_LOG_TRIVIAL(debug)<< status.ToString();

    //eventually load from config/saved state
    playhead = new std::string("0");
    current_state = PlaybackState::STOPPED;
}

/**
 * @brief Starts recording
 * @details [long description]
 */
void Playback::record(){
    if (current_state == PlaybackState::RECORDING){
        return;
    }
    
    if(current_state == PlaybackState::STOPPED){
        //start new playback thread
        record_thread = new boost::thread(boost::bind(&Playback::record_loop, this));
        start_time = std::chrono::steady_clock::now();
    }

    this->set_state(PlaybackState::RECORDING);
}

/**
 * @brief Sets if playback should loop on end
 * @details [long description]
 * 
 * @param t_loop boolean
 */
void Playback::toggle_loop(bool t_loop){
    loop = t_loop;
}

/**
 * @brief Starts playback
 * @details [long description]
 */
void Playback::play(){
    if (current_state == PlaybackState::PLAYING){
        return;
    }

    if(current_state == PlaybackState::STOPPED){
        //start new playback thread
        playback_thread = new boost::thread(boost::bind(&Playback::play_loop, this));
    }

    this->set_state(PlaybackState::PLAYING);
}

/**
 * @brief Pauses playback
 * @details [long description]
 */
void Playback::pause(){
    this->set_state(PlaybackState::PAUSED);
}

/**
 * @brief Stops playback
 * @details [long description]
 */
void Playback::stop(){
    if (current_state == PlaybackState::RECORDING){
        record_thread->join();
    }
    if (current_state == PlaybackState::PLAYING){
        playback_thread->join();
    }
    this->set_state(PlaybackState::STOPPED);
    index.playhead.assign("0");
}

/**
 * @brief Set the current state of playback. eg Playing
 * @details [long description]
 * 
 * @param state [description]
 */
void Playback::set_state(PlaybackState state){
    boost::unique_lock<boost::mutex> lock(state_mutex);
    current_state = state;
    lock.unlock();
}


/**
 * @brief Record loop function
 * @details [long description]
 */
void Playback::record_loop(){
    boost::unique_lock<boost::mutex> lock(frame_mutex);
    while(current_state == PlaybackState::RECORDING)
    {

        while(frame_queue.empty())
        {
            if(current_state == PlaybackState::RECORDING){
                wait_for_frame.wait(lock);
            }else{
                lock.unlock();
                break;
            }
        }

        auto end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> position = start_time - end_time;

        index.playhead = std::to_string(position.count());
        flatbuffers::FlatBufferBuilder builder(1024);
        auto frame_vector = builder.CreateVectorOfStructs(&frame_queue.front());
        auto frame = CreateFrame(builder, frame_vector);
        builder.Finish(frame);

        db->Put(leveldb::WriteOptions(), index.playhead, (const char*) builder.GetBufferPointer());
        frame_queue.pop();
      //  BOOST_LOG_TRIVIAL(debug) << "Record frame at: " << index.playhead;
    }

}

/**
 * @brief Playback loop function
 * @details Loops through the leveldb file with an iterator
 */
void Playback::play_loop(){
    boost::unique_lock<boost::mutex> lock(frame_mutex);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    while((loop && current_state == PlaybackState::PLAYING)){
        for (it->Seek(index.playhead); current_state == PlaybackState::PLAYING, it->Valid(); it->Next()) {
            auto data = it->value().ToString();
            char * writable = new char[data.size() + 1];
            std::copy(data.begin(), data.end(), writable);
            auto frame = GetFrame(writable);
            
            boost::unique_lock<boost::mutex> lock(frame_mutex);
            frame_queue.push(*frame->pixels());
            lock.unlock();
            wait_for_frame.notify_one();

            index.playhead = it->key().ToString();
            delete[] writable;
        //    BOOST_LOG_TRIVIAL(debug) << "Play frame at: " << index.playhead;
        }
    }
    delete it;
}

Playback::~Playback(){
    delete db;
}
