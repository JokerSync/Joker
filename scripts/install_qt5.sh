#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install qt5

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

yes | sudo add-apt-repository ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install build-essential g++ curl
sudo apt-get install -qq qt5-default qtdeclarative5-dev

fi
