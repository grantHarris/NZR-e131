# NZR-e131

RPi e1.31 bridge to WS2811 LED's

## Basic Usage
```bash
sudo ./e131bridge
```

### Logginf for poi 
```bash
sudo ./e131bridge --config=../config/config-poi.yml --verbosity=info --stats=true
```

### Flags

* --verbosity="[log level]"
Toggle level of logging verbosity. 

* --stats=true
Toggle stats display on or off

* --config="path/to/config.yaml"
Path to config file

* --config="path/to/logfile"
Path to logging file



## Build Debug 
```bash
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Build Release 
```bash
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Libraries
* https://github.com/jgarff/rpi_ws281x.git
* https://github.com/jbeder/yaml-cpp.git
* Boost 1.66
* http://www.airspayce.com/mikem/bcm2835/