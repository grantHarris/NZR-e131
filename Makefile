all: main.cpp
	g++ main.cpp -o e131tows2811 `pkg-config --cflags --libs yaml-cpp` -L lib/ -lws2811
clean: 
	$(RM) e131tows2811

