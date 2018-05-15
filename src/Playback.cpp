#include "Playback.h"

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param file_name Location of the leveldb directory
 */
Playback::Playback(E131&& t_e131, Apa102Strip&& t_apa102_strip) : e131(std::move(t_e131)), apa102_strip(std::move(t_apa102_strip)){
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


void Playback::push_frame(std::vector<Pixel>& t_pixels){
    std::unique_lock<std::mutex> lock(frame_mutex);
    nzr::Frame frame;
    
    nzr::Pixel* pixel = frame.add_pixels();
    std::vector<Pixel>::iterator it;

    for(it = t_pixels.begin(); it != t_pixels.end(); it++){
        pixel->set_r((*it).r);
        pixel->set_g((*it).g);
        pixel->set_b((*it).b);
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
        record_thread = new std::thread([&](){
            this->record_to_file_thread();
        });
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
        record_thread = new std::thread([&](){
            this->play_from_file_thread();
        });
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
    this->stop();
    if (current_state == PlaybackState::RECORDING){
        BOOST_LOG_TRIVIAL(info) << "Stopping recording";
        if(record_thread->joinable()){
            record_thread->join();
        }
    }
    if (current_state == PlaybackState::PLAYING){
        BOOST_LOG_TRIVIAL(info) << "Stopping playing";
        if(playback_thread->joinable()){
            playback_thread->join();
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Stopping";
    this->set_state(PlaybackState::STOPPED);
    index.playhead.assign("0");
}

void Playback::live(){
    std::thread e131_receive_data_thread([&](){
        e131.receive_data();
    });

    std::thread live_stream_thread([&](){
        this->live_stream_thread();
    });

    //thread_list.push_back(std::move(e131_receive_data_thread));
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


/**
 * @brief Record loop function
 * @details [long description]
 */
void Playback::record_to_file_thread(){
    BOOST_LOG_TRIVIAL(info) << "Record loop starting";
    std::unique_lock<std::mutex> lock(frame_mutex);
    while(stop_requested() == false && current_state == PlaybackState::RECORDING)
    {
        while(frame_queue.empty())
        {
            if(stop_requested() == false && current_state == PlaybackState::RECORDING){
                wait_for_frame.wait(lock);
            }else{
                break;
            }
        }

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
void Playback::play_from_file_thread(){
    BOOST_LOG_TRIVIAL(info) << "Play loop starting";
    std::unique_lock<std::mutex> lock(frame_mutex);
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    while((stop_requested() == false && current_state == PlaybackState::PLAYING)){
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
            frame_queue.push(frame);
            wait_for_frame.notify_one();
        }
    }
    delete it;
}


void Playback::live_stream_thread(){
    while (stop_requested() == false){
        std::unique_lock<std::mutex> mlock(e131.frame_mutex);
        e131.wait_for_frame.wait(mlock);
        apa102_strip.push_frame(e131.pixels);
    }
}

Playback::~Playback(){
    delete db;
}
