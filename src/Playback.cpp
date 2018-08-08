#include "Playback.h"

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param file_name Location of the leveldb directory
 */
Playback::Playback(E131&& t_e131, LEDStrip&& t_strip) : e131(std::move(t_e131)), strip(std::move(t_strip)){
    current_state = PlaybackState::STOPPED;
    playhead = new std::string("0");
    current_state = PlaybackState::STOPPED;
}

void Playback::set_save_location(std::string file_name){
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
        BOOST_LOG_TRIVIAL(info) << "Already recording";
        return;
    }
    
    if(current_state == PlaybackState::STOPPED){
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
    BOOST_LOG_TRIVIAL(info) << "Stopping";
    this->set_state(PlaybackState::STOPPED);
    index.playhead.assign("0");
}

void Playback::live(){
    // std::thread e131_receive_data_thread([&](){
    //     e131.receive_data();
    // });

    // thread_list.push_back(std::move(e131_receive_data_thread));

    // std::thread live_stream_thread([&](){
    //     this->live_stream_thread();
    // });
}

/**
 * @brief Set the current state of playback. eg Playing
 * @details [long description]
 * 
 * @param state [description]
 */
void Playback::set_state(PlaybackState state){
    std::unique_lock<std::mutex> lock(state_mutex);
    current_state = state;
}

void Playback::thread_loop(){
    while(stop_requested() == false)
    {
        switch(current_state){
            case PlaybackState::RECORDING:{
                auto pixels = this->live_stream();
                this->record_to_file(pixels);
                break;
            }

            case PlaybackState::PLAYING:{
                this->play_from_file();
                break;
            }

            case PlaybackState::LIVE:{
                this->live_stream();
                break;
            }
        }
    }

}

/**
 * @brief
 * @details [long description]
 */
void Playback::record_to_file(std::vector<Pixel>& t_pixels){
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> position =  end_time - start_time;

    index.playhead = std::to_string(position.count());

    nzr::Frame frame;
    
    nzr::Pixel* pixel = frame.add_pixels();
    std::vector<Pixel>::iterator it;

    for(it = t_pixels.begin(); it != t_pixels.end(); it++){
        pixel->set_r((*it).r);
        pixel->set_g((*it).g);
        pixel->set_b((*it).b);
    }

    std::string output;
    frame.SerializeToString(&output);
    db->Put(leveldb::WriteOptions(), index.playhead, output);
    frame_queue.pop();
    BOOST_LOG_TRIVIAL(info) << "Record frame at: " << index.playhead;

}

/**
 * @brief Playback loop function
 * @details Loops through the leveldb file with an iterator
 */
void Playback::play_from_file(){
    std::unique_lock<std::mutex> lock(frame_mutex);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->Seek(index.playhead); current_state == PlaybackState::PLAYING, it->Valid(); it->Next()) {
        auto data = it->value().ToString();
        nzr::Frame frame;
   //     try {
            double current = boost::lexical_cast<double>(it->key().ToString());
            double last = boost::lexical_cast<double>(index.playhead);

            std::this_thread::sleep_for(std::chrono::milliseconds((long) (current - last) * 1000));
            index.playhead = current;
            BOOST_LOG_TRIVIAL(debug) << "Play frame at: " << index.playhead;

        // } catch(bad_lexical_cast&) {
        //     //Do your errormagic
        // }

        //frame.ParseFromString(&data)
        //frame_queue.push(frame);
        


        //this needs to be converted to the type used
        //strip.push_frame(frame);
        wait_for_frame.notify_one();
    }
    delete it;
}


std::vector<Pixel> Playback::live_stream(){
    std::unique_lock<std::mutex> mlock(e131.frame_mutex);
    e131.wait_for_frame.wait(mlock);
    strip.push_frame(e131.pixels);
    return e131.pixels;
}

Playback::~Playback(){
    delete db;
}
