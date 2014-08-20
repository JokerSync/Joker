#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install portaudio

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

# Installing portaudio
sudo apt-get install libasound-dev
curl -L http://portaudio.com/archives/pa_stable_v19_20140130.tgz -o pa_stable_v19_20140130.tgz
tar -xvzf pa_stable_v19_20140130.tgz
cd portaudio
./configure
make
sudo make install
cd ..
rm pa_stable_v19_20140130.tgz
rm -rf portaudio

fi
