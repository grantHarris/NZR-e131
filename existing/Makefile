all: main.cpp
	g++ -pthread main.cpp -o e131tows2811 `pkg-config --cflags --libs yaml-cpp` -le131 -lboost_log -lboost_program_options -Wall -DBOOST_LOG_DYN_LINK -L lib/ -lws2811
clean: 
	$(RM) e131tows2811
test:
	

