#!/bin/bash

echo "Install FFMpeg"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install libav ffmpeg

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

sudo echo deb http://www.deb-multimedia.org testing main non-free >> /etc/apt/sources.list
sudo apt-get update
sudo apt-get install ffmpeg
#curl -L http://ffmpeg.org/releases/ffmpeg-2.6.2.tar.bz2 -o ffmpeg-2.6.2.tar.bz2 
#curl -L http://johnvansickle.com/ffmpeg/releases/ffmpeg-release-64bit-static.tar.xz -o ffmpeg-release-64bit-static.tar.xz
#tar -xvjf ffmpeg-release-64bit-static.tar.xz
#cd ffmpeg-release-64bit-static
#./configure
#make
#sudo make install
#cd ..
#rm -rf ffmpeg-release-64bit-static
#rm ffmpeg-release-64bit-static.tar.xz

fi

echo "Done."
