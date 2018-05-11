#include "Playback.h"

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param file_name Location of the leveldb directory
 */
Playback::Playback(std::string file_name, bool *t_running) : running(t_running) {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, file_name, &db);
    if (!status.ok()) BOOST_LOG_TRIVIAL(debug)<< status.ToString();

    //eventually load from config/saved state
    playhead = new std::string("0");
    current_state = PlaybackState::STOPPED;

    BOOST_LOG_TRIVIAL(info) << "info!: " << *playhead;
}

void Playback::push_frame(std::vector<Pixel> const& t_pixels){
    boost::unique_lock<boost::mutex> lock(frame_mutex);
    nzr::Frame frame;
    
    nzr::Pixel* pixel = frame.add_pixels();
    
    for(std::vector<Pixel>::iterator it = t_pixels->begin(); it != t_pixels->end(); it++){
        pixel->set_r(*it.r);
        BOOST_LOG_TRIVIAL(debug) << "Set r: " << *it.r;
        pixel->set_g(*it.g);
        BOOST_LOG_TRIVIAL(debug) << "Set g: " << *it.g;
        pixel->set_b(*it.b);
        BOOST_LOG_TRIVIAL(debug) << "Set b: " << *it.b;
    }

    frame_queue.push(frame);
    lock.unlock();
    wait_for_frame.notify_one();
}

/**
 * @brief Starts recording
 * @details [long description]
 */
void Playback::record(){
    if (current_state == PlaybackState::RECORDING){
        BOOST_LOG_TRIVIAL(info) << "Already recording";
        return;
    }
    
    if(current_state == PlaybackState::STOPPED){
        //start new playback thread
        record_thread = new boost::thread(boost::bind(&Playback::record_loop, this));
        start_time = std::chrono::steady_clock::now();
    }
    BOOST_LOG_TRIVIAL(info) << "Starting recording";
    this->set_state(PlaybackState::RECORDING);
}

/**
 * @brief Sets if playback should loop on end
 * @details [long description]
 * 
 * @param t_loop boolean
 */
void Playback::toggle_loop(bool t_loop){
    BOOST_LOG_TRIVIAL(info) << "Toggle looping" << t_loop;
    loop = t_loop;
}

/**
 * @brief Starts playback
 * @details [long description]
 */
void Playback::play(){
    if (current_state == PlaybackState::PLAYING){
        BOOST_LOG_TRIVIAL(info) << "Already playing";
        return;
    }

    if(current_state == PlaybackState::STOPPED){
        //start new playback thread
        playback_thread = new boost::thread(boost::bind(&Playback::play_loop, this));
    }

    BOOST_LOG_TRIVIAL(info) << "Starting playing";
    this->set_state(PlaybackState::PLAYING);
}

/**
 * @brief Pauses playback
 * @details [long description]
 */
void Playback::pause(){
    BOOST_LOG_TRIVIAL(info) << "Pausing";
    this->set_state(PlaybackState::PAUSED);
}

/**
 * @brief Stops playback
 * @details [long description]
 */
void Playback::stop(){
    if (current_state == PlaybackState::RECORDING){
        BOOST_LOG_TRIVIAL(info) << "Stopping recording";
        record_thread->join();
    }
    if (current_state == PlaybackState::PLAYING){
        BOOST_LOG_TRIVIAL(info) << "Stopping playing";
        playback_thread->join();
    }
    BOOST_LOG_TRIVIAL(info) << "Stopping";
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
    BOOST_LOG_TRIVIAL(info) << "Record loop starting";
    boost::unique_lock<boost::mutex> lock(frame_mutex);
    while(current_state == PlaybackState::RECORDING)
    {
        BOOST_LOG_TRIVIAL(info) << "Recording 1";

        while(frame_queue.empty())
        {
            BOOST_LOG_TRIVIAL(info) << "Recording 2";
            if(current_state == PlaybackState::RECORDING){
                BOOST_LOG_TRIVIAL(info) << "Recording 3";
                wait_for_frame.wait(lock);
                BOOST_LOG_TRIVIAL(info) << "Recording 4";
            }else{
                BOOST_LOG_TRIVIAL(info) << "Recording 5";
                lock.unlock();
                break;
            }
        }
        BOOST_LOG_TRIVIAL(info) << "Recording 6";

        auto end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> position =  end_time - start_time;

        index.playhead = std::to_string(position.count());
        auto frame = frame_queue.front();
        std::string output;
        frame.SerializeToString(&output);
        db->Put(leveldb::WriteOptions(), index.playhead, output);
        frame_queue.pop();
        BOOST_LOG_TRIVIAL(info) << "Record frame at: " << index.playhead;
    }

}

/**
 * @brief Playback loop function
 * @details Loops through the leveldb file with an iterator
 */
void Playback::play_loop(){
    BOOST_LOG_TRIVIAL(info) << "Play loop starting";
    boost::unique_lock<boost::mutex> lock(frame_mutex);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    while((loop && current_state == PlaybackState::PLAYING)){
        for (it->Seek(index.playhead); current_state == PlaybackState::PLAYING, it->Valid(); it->Next()) {
            auto data = it->value().ToString();
            nzr::Frame frame;
       //     try {
                double current = boost::lexical_cast<double>(it->key().ToString());
                double last = boost::lexical_cast<double>(index.playhead);

                usleep((current - last) * 1000000);
                index.playhead = current;
                BOOST_LOG_TRIVIAL(debug) << "Play frame at: " << index.playhead;

            // } catch(bad_lexical_cast&) {
            //     //Do your errormagic
            // }

            //frame.ParseFromString(&data)
            frame_queue.push(frame);
            lock.unlock();
            wait_for_frame.notify_one();
        }
    }
    delete it;
}

Playback::~Playback(){
    delete db;
}
