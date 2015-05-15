#!/bin/bash

echo "Install portaudio"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"
brew install portaudio

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

sudo apt-get install yasm
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

echo "Done."
