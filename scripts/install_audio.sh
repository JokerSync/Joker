#!/bin/bash

echo "Install audio"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"
brew install portaudio

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

echo "Install portaudio"
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

sudo apt-get install libasound2-dev

echo "Install RtMidi"
cd vendor/rtmidi
sudo apt-get install autoconf libtool
#libtoolize
#aclocal
#automake --add-missing
autoconf
./configure
make
sudo make install
cd ../..

fi

echo "Done."
