#!/bin/bash
BASEDIR=$(dirname $0)
echo $BASEDIR
$BASEDIR/install_ffmpeg.sh
$BASEDIR/install_portaudio.sh
$BASEDIR/install_ltc.sh
$BASEDIR/install_sdl.sh
exit 0

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
sudo apt-get install -qq qt5-default qtdeclarative5-dev

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

#sudo apt-get install doxygen uncrustify

#sudo pip install cpp-coveralls
fi
