#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install ffmpeg libav portaudio
brew install python doxygen uncrustify node npm pyenv wget
eval "$(pyenv init -)"
pyenv install 2.7.6
pyenv global 2.7.6
pyenv rehash
pip install cpp-coveralls
pyenv rehash

brew install qt5
find /usr/local/Cellar/ -perm -ugo+r -iname "*dylib*" -exec chmod +w {} \;

# Installing appdmg for deployment
npm install -g appdmg


elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

yes | sudo add-apt-repository ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install build-essential g++ curl
sudo apt-get install ffmpeg portaudio
sudo apt-get install -qq qt5-default qtdeclarative5-dev
sudo apt-get install doxygen uncrustify

sudo pip install cpp-coveralls
fi
