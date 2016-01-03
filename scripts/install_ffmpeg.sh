#!/bin/bash

echo "Install FFMpeg"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew update
brew install libav ffmpeg

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

wget http://www.ffmpeg.org/releases/ffmpeg-2.6.2.tar.bz2
tar xf ffmpeg-2.6.2.tar.bz2
cd ffmpeg-2.6.2 && ./configure --prefix=/usr --disable-static --enable-shared --disable-everything --disable-avdevice --disable-doc --disable-htmlpages --disable-manpages --disable-programs --disable-encoders --disable-muxers --disable-decoders --enable-swscale --disable-yasm --enable-protocol=file --enable-protocol=http --enable-iconv  && make -j4 && sudo make install
cd ..

fi

echo "Done."
