#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install libav ffmpeg

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

# Installing ffmpeg
curl -L http://ffmpeg.org/releases/ffmpeg-2.3.3.tar.bz2 -o ffmpeg-2.3.3.tar.bz2
tar xjf ffmpeg-2.3.3.tar.bz2
cd ffmpeg-2.3.3
./configure --disable-asm
make
sudo make install
cd ..
rm ffmpeg-2.3.3.tar.bz2
rm -rf ffmpeg-2.3.3

fi
