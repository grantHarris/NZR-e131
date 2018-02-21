#include "E131.h"

E131::E131(YAML::Node& t_config, LEDStrip& t_led_strip) : config(t_config), led_strip(t_led_strip)
{
    do {
        sockfd = e131_socket();
        if(sockfd < 0){
            BOOST_LOG_TRIVIAL(error) << "E1.31 socket creation failed.";
            usleep(1000000);
        }
    }
    while (sockfd < 0);

    BOOST_LOG_TRIVIAL(info) << "E1.31 socket created.";

    while (e131_bind(sockfd, E131_DEFAULT_PORT) < 0){
        BOOST_LOG_TRIVIAL(error) << "E1.31 failed binding to port: " << E131_DEFAULT_PORT;
        usleep(1000000);       
    }

    BOOST_LOG_TRIVIAL(info) << "E1.31 client bound to port: " << E131_DEFAULT_PORT;

    for(YAML::const_iterator it=config["mapping"].begin(); it != config["mapping"].end(); ++it) {
        int universe = it->first.as<int>();
        this->join_universe(universe);
        this->register_universe_for_stats(universe);
    }

}

void E131::join_universe(int t_universe)
{

    BOOST_LOG_TRIVIAL(info) << "Joining universe: " << t_universe;

    std::stringstream ss;
    ss << "sudo ip maddr add 239.255.0." << t_universe << " dev wlan0";
    
    BOOST_LOG_TRIVIAL(debug) << "Executing shell command: " << ss.str();
    Util::exec(ss.str().c_str());

    if (e131_multicast_join(sockfd, t_universe) < 0)
        err(EXIT_FAILURE, "E1.31 Multicast join failed for universe ");   
}

void E131::receive_data(bool *running)
{
    int strip_channel, start_address, total_rgb_channels, end_address, start_address_offset;

    while(*running == true) {
        if (e131_recv(sockfd, &packet) < 0)
            err(EXIT_FAILURE, "E1.31 receivex failed");

        if ((error = e131_pkt_validate(&packet)) != E131_ERR_NONE) {
            BOOST_LOG_TRIVIAL(error) << "E1.31 packet validation error" << e131_strerror(error);
            continue;
        }
        
        int universe = ntohs(packet.frame.universe);

        if (e131_pkt_discard(&packet, last_seq)) {
            BOOST_LOG_TRIVIAL(warning) << "E1.31 packet received out of sequence. Last: " 
            << static_cast<int>(last_seq) << ", Seq: " << static_cast<int>(packet.frame.seq_number);
            this->log_universe_packet(universe, State::OUT_OF_SEQUENCE);
            last_seq = packet.frame.seq_number;
            continue;
        }

        this->log_universe_packet(universe, State::GOOD);
        BOOST_LOG_TRIVIAL(debug) << "Packet for universe: " << universe << "Seq: " << static_cast<int>(packet.frame.seq_number);

        YAML::Node universe_config = config["mapping"][universe];
        for(YAML::const_iterator it = universe_config.begin(); it != universe_config.end(); ++it) {
            const YAML::Node& entry = *it;
            YAML::Node output_params = entry["output"];
            YAML::Node input_params = entry["input"];

            strip_channel = output_params["strip_channel"].as<int>();
            start_address = std::max(1, std::min(input_params["start_address"].as<int>(), 511));
            total_rgb_channels = std::max(0, std::min(input_params["total_rgb_channels"].as<int>() - 1, 511));
            end_address = std::max(1, std::min(total_rgb_channels - start_address, 511));
            start_address_offset = output_params["start_address"].as<int>();
            
            for(int i = start_address - 1; i < end_address; i++){
                int index = i * 3 + 1;
                Pixel pixel;
                pixel.r = packet.dmp.prop_val[index];
                pixel.g = packet.dmp.prop_val[index + 1];
                pixel.b = packet.dmp.prop_val[index + 2];
                led_strip.write_to_buffer(strip_channel, i + start_address_offset, pixel);
            }
        }
        last_seq = packet.frame.seq_number;
    }
}

void E131::register_universe_for_stats(unsigned int t_universe){
    UniverseStats universe {0, 0};
    universe_stats[t_universe] = universe;
}

void E131::log_universe_packet(unsigned int t_universe, State state){
    log_mutex.lock();
    switch(state){
            case State::GOOD:
                universe_stats[t_universe].updates++;
            break;
            case State::OUT_OF_SEQUENCE:
                universe_stats[t_universe].out_of_sequence++;
            break;
        }
    log_mutex.unlock();
}

void E131::stats_thread(bool *running){
    while(*running == true){
        log_mutex.lock();
        for(std::map<unsigned int, UniverseStats>::iterator iter = universe_stats.begin(); iter != universe_stats.end(); ++iter)
        {
            unsigned int universe_number =  iter->first;
            UniverseStats stats = iter->second;
            BOOST_LOG_TRIVIAL(info) << "Universe: " << universe_number << ", Good: " << stats.updates << "FPS, Dropped: " << stats.out_of_sequence << "FPS";
            UniverseStats universe {0, 0};
            iter->second = universe;
        }

        log_mutex.unlock();
        usleep(1000000);
    }
}

