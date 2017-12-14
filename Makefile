all: main.cpp
	g++ -pthread main.cpp -o e131tows2811 `pkg-config --cflags --libs yaml-cpp` -le131 -Wall -L lib/ -lws2811
clean: 
	$(RM) e131tows2811

