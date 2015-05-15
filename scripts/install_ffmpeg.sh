#!/bin/bash

echo "Install FFMpeg"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install libav ffmpeg

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

udo apt-get install yasm
curl -L http://ffmpeg.org/releases/ffmpeg-2.6.2.tar.bz2 -o ffmpeg-2.6.2.tar.bz2 
tar -xvjf ffmpeg-2.6.2.tar.bz2 
cd ffmpeg-2.6.2
./configure
make
sudo make install
cd ..
rm ffmpeg-2.6.2.tar.bz2 
rm -rf ffmpeg-2.6.2

fi

echo "Done."
