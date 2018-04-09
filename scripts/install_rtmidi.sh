#!/bin/bash

set -e

echo "Install RtMidi"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

wget http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-2.1.0.tar.gz
tar -xvzf rtmidi-2.1.0.tar.gz
cd rtmidi-2.1.0

# this requires libasound2-dev, installed as an apt addon in .travis.yml
./configure --prefix=$HOME/usr/local
make
# allow error for make install
set +e
make install

cd ..

fi

echo "Done."
