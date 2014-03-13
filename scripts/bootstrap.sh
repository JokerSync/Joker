#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update > /dev/null
brew install qt5
#brew install sdl2 sdl2_image sdl2_ttf
brew install ffmpeg libav portaudio doxygen uncrustify node npm
find /usr/local/Cellar/ -perm -ugo+r -iname "*dylib*" -exec chmod +w {} \;

# Installing appdmg for deployment
npm install -g appdmg


elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

yes | sudo add-apt-repository ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install build-essential g++
sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl-ttf2.0-0
sudo apt-get install -qq qt5-default qtdeclarative5-dev

fi
