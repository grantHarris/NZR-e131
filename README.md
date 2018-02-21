# NZR-e131

RPi e1.31 bridge to WS2811 LED's

## Basic Usage
```
	sudo ./e131bridge
```

## Build Debug 
```
	mkdir Debug
	cd Debug
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	make
```

## Build Release 
```
	mkdir Release
	cd Release
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make
```

## Libraries
https://github.com/jgarff/rpi_ws281x.git
https://github.com/jbeder/yaml-cpp.git
Boost