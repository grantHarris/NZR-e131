# NZR-e131

RPi e1.31 bridge to WS2811 LEDs

## Useage examples
### Basic Usage
```bash
sudo ./e131bridge
```

### Playing back for the robot config from folder foo
```bash
sudo ./e131bridge --config=../config/config-robot.yml --level=trace --mode=playback --file=foo
```

### Logging for poi 
```bash
sudo ./e131bridge --config=../config/config-poi.yml --level=info --stats=true
```

## Flags

* --level=[log level]
Toggle level of logging verbosity. trace, debug, info, warning, error, fatal. Defaults to info.

* --mode=[mode type]
One of record, playback, or live. Defaults to live.
 - Playback plays back from an existing sequence stored in a leveldb folder
 - Record records from a live e131 stream to the folder specified in the --file flag
 - Live. Plays the live e131 stream. No recording.

* --file=path/to/leveldb/folder
The location where the saved show is stored. This is a leveldb folder. If the folder already exists you can play it back by setting the mode flag to playback. If you set the mode flag to record and the folder doesn't already exist it is created. Recording records from the live stream.

* --stats=true
Toggle stats display on or off

* --config=path/to/config.yaml
Path to config file

* --log=path/to/logfile
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

## Library Builds
### Install gcc git
    sudo apt-get install gcc git vim

### Install cmake
    wget https://cmake.org/files/v3.12/cmake-3.12.0.tar.gz
    tar -xvzf cmake-3.12.0.tar.gz
    cd cmake-3.12.0/
    sudo ./bootstrap
    sudo make
    sudo make install

### Install boost
    wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
    tar -xvzf boost_1_67_0.tar.gz
    cd boost_1_67_0
    ./bootstrap.sh --prefix=/usr/

### Install yaml-cpp
    wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.6.2.tar.gz
    tar -xvzf yaml-cpp-0.6.2.tar.gz
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

### Install BCM2835
    wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz
    tar zxvf bbcm2835-1.56.tar.gz
    cd bcm2835-1.xx
    ./configure
    make
    sudo make check
    sudo make install

### Install libe131
    sudo apt-get install automake
    git clone git@github.com:hhromic/libe131.git
    cd libe131
    ./autogen.sh
    ./configure
    make
    sudo make install

### Install LevelDB
    git clone git@github.com:google/leveldb.git
    cd leveldb
    mkdir -p build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

### Install protobuf
    sudo apt-get install autoconf automake libtool curl make g++ unzip
    git clone https://github.com/google/protobuf.git
    cd protobuf
    git submodule update --init --recursive
    ./autogen.sh
    ./configure
    make
    make check
    sudo make install
    sudo ldconfig # refresh shared library cache.

### Install curses
    sudo apt-get install libncurses5-dev libncursesw5-dev

### Build rpi_ws281x.git
    git clone git@github.com:jgarff/rpi_ws281x.git
    cd rpi_ws281x
    apt-get install scons
    scons
    # move built a file into the lib directory

## Libraries
* https://github.com/jgarff/rpi_ws281x.git
* https://github.com/jbeder/yaml-cpp.git
* Boost 1.66
* http://www.airspayce.com/mikem/bcm2835/
