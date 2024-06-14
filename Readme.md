# Husbando
The goal of this program is to be able to watch shows on a raspberry pi zero and locally along with simple note and ratings built in. Currently the plan is to ssh into the raspberry pi zero and pass a video url to cvlc or to run mpv locally. This program will be able to search shows (similar to ani-cli) with ncurses as the tui and gtk as the gui.

# Building
```sh
#TODO: rewrite this, also probs have releases
#NOTE: this requires an sshkey with gitlab for now
git clone https://github.com/HerbGlitch/husbando.git
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make

#NOTE: this needs to be run from the root of the project
cd ..
./build/husbando
```

# Dependencies //TODO: have sections for these for major linux distros
libssh

libcurl

ncurses

inih //this one is pulled as a submodule
