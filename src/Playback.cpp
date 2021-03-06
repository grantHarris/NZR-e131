#include "Playback.h"


/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param file_name Location of the leveldb directory
 */
Playback::Playback(E131&& t_e131, LEDStrip&& t_strip) : e131(std::move(t_e131)), strip(std::move(t_strip)){
    current_state = PlaybackState::STOPPED;
    index.playhead = std::string("0");
}

void Playback::set_file_location(std::string file_name){
    leveldb::Options options;
    options.create_if_missing = true;
    options.comparator = &cmp;

    leveldb::Status status = leveldb::DB::Open(options, file_name, &db);
    if (!status.ok()) BOOST_LOG_TRIVIAL(debug)<< status.ToString();

    //eventually load from config/saved state
    index.playhead = std::string("0");
    current_state = PlaybackState::STOPPED;
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



// Playhead controls //

/**
 * @brief Starts recording
 * @details [long description]
 */
void Playback::record_from_live(){
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

void Playback::play_live(){
    BOOST_LOG_TRIVIAL(info) << "Start live playback";
    this->set_state(PlaybackState::LIVE);
}

/**
 * @brief Starts playback
 * @details [long description]
 */
void Playback::start_playback(){
    BOOST_LOG_TRIVIAL(info) << "Starting playing";
    this->set_state(PlaybackState::PLAYING);
}

/**
 * @brief Pauses playback
 * @details [long description]
 */
void Playback::pause_playback(){
    BOOST_LOG_TRIVIAL(info) << "Pausing";
    this->set_state(PlaybackState::PAUSED);
}

/**
 * @brief Stops playback
 * @details [long description]
 */
void Playback::stop_playback(){
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
    std::lock_guard<std::mutex> lock(state_mutex);
    current_state = state;
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
    std::vector<Pixel>::iterator it;

    for(it = t_pixels.begin(); it != t_pixels.end(); it++){
        nzr::Pixel* pixel = frame.add_pixels();
        pixel->set_r((*it).r);
        pixel->set_g((*it).g);
        pixel->set_b((*it).b);
    }

    std::string output;
    frame.SerializeToString(&output);
    db->Put(leveldb::WriteOptions(), index.playhead, output);
    BOOST_LOG_TRIVIAL(trace) << "Record frame at: " << index.playhead;
}

/**
 * @brief Playback loop function
 * @details Loops through the leveldb file with an iterator
 */
void Playback::play_from_file(){
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->Seek("0"); current_state == PlaybackState::PLAYING && stop_requested() == false, it->Valid(); it->Next()) {
        
        double current = boost::lexical_cast<double>(it->key().ToString());
        double last = boost::lexical_cast<double>(index.playhead);

        std::this_thread::sleep_for(std::chrono::milliseconds(std::max(0, (int) ((current - last) * 1000))));
        index.playhead = std::to_string(current);        
        nzr::Frame frame;
        auto data = it->value().ToString();
        frame.ParseFromString(data);
        
        std::vector<Pixel> pixels;
        pixels.resize(frame.pixels_size());

        for(int i = 0; i < frame.pixels_size(); i++){
            const nzr::Pixel& proto_pixel = frame.pixels(i);
            Pixel pixel;
            pixel.r = proto_pixel.r();
            pixel.g = proto_pixel.g();
            pixel.b = proto_pixel.b();
            pixels[i] = pixel;
        }
        strip.push_frame(pixels);

    }
    delete it;
}



void Playback::thread_loop(){
    while(stop_requested() == false)
    {
        switch(current_state){
            case PlaybackState::RECORDING:{
                std::unique_lock<std::mutex> mlock(e131.frame_mutex);
                e131.wait_for_frame.wait(mlock);
                strip.push_frame(e131.pixels);
                this->record_to_file(e131.pixels);
                break;
            }

            case PlaybackState::PLAYING:{
                this->play_from_file();
                break;
            }

            case PlaybackState::LIVE:{
                std::unique_lock<std::mutex> mlock(e131.frame_mutex);
                e131.wait_for_frame.wait(mlock);
                strip.push_frame(e131.pixels);
                break;
            }
        }
    }
}

Playback::~Playback(){
    delete db;
}
