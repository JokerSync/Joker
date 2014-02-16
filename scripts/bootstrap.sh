#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update > /dev/null
brew install qt5 sdl2 sdl2_image sdl2_ttf ffmpeg libav
find /usr/local/Cellar/ -perm -ugo+r -iname "*dylib*" -exec chmod +w {} \;

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

yes | sudo add-apt-repository ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install build-essential g++
sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl-ttf2.0-0
sudo apt-get install -qq qt5-default qtdeclarative5-dev

fi

echo "Install libltc"
#wget https://github.com/x42/libltc/releases/download/v1.1.3/libltc-1.1.3.tar.gz
#tar -xvzf libltc-1.1.3.tar.gz
#cd libltc-1.1.3
#./configure
#make
#sudo make install
#cd ..
#rm libltc-1.1.3.tar.gz
#rm -rf libltc-1.1.3
git clone https://github.com/Phonations/libltc.git
cd libltc
aclocal
autoheader
glibtoolize --copy
autoconf
automake --gnu --add-missing --copy
./configure
make
sudo make install
cd ..
