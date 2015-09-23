#!/bin/bash

echo "Install FFMpeg"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install libav ffmpeg

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

#sudo echo deb http://www.deb-multimedia.org testing main non-free >> /etc/apt/sources.list
#udo add-apt-repository ppa:mc3man/trusty-media
sudo apt-get update
sudo apt-get dist-upgrade
sudo apt-get install libavformat-dev libswscale-dev

fi

echo "Done."
