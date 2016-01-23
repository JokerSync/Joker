#!/bin/bash

set -e

echo "Install FFMpeg"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install ffmpeg --without-qtkit

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

wget http://www.ffmpeg.org/releases/ffmpeg-2.6.3.tar.bz2
tar xf ffmpeg-2.6.3.tar.bz2
cd ffmpeg-2.6.3 && ./configure --prefix=/usr/local --disable-static --enable-shared --disable-everything --disable-avdevice --disable-doc --disable-htmlpages --disable-manpages --disable-programs --disable-encoders --disable-muxers --disable-decoders --enable-swscale --disable-yasm --enable-protocol=file --enable-protocol=http --enable-iconv  && make -j4 && sudo make install
cd ..
rm -rf ffmpeg-2.6.3
rm ffmpeg-2.6.3.tar.bz2

fi

echo "Done."
