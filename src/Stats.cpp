namespace Stats {

	std::mutex log_mutex;

	struct UniverseStats {
	    unsigned int out_of_sequence {0};
	    unsigned int updates {0};
	};

	enum State {
		GOOD,
		OUT_OF_SEQUENCE
	};

	std::std::map<unsigned int, UniverseStats> universe_stats;

	void register_universe(unsigned int t_universe){
		universe_stats[t_universe] = new UniverseStats{0, 0};
	}

	void log_universe_packet(unsigned int t_universe, State state){

		switch(state){
			case State.GOOD;
				universe_stats[t_universe].updates++;
			break;
			case OUT_OF_SEQUENCE:
				universe_stats[t_universe].out_of_sequence++;
			break;
		}
	}

	void log_thread(){

		log_mutex.lock();
			BOOST_LOG_TRIVIAL(error) << << e131_strerror(error);

		log_mutex.unlock();
	}

}