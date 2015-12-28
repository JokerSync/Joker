#!/bin/bash

echo "Install RtMidi"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

#sudo apt-get install libasound2-dev

wget http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-2.1.0.tar.gz
tar -xvzf rtmidi-2.1.0.tar.gz
cd rtmidi-2.1.0

./configure
make
cd ..

fi

echo "Done."
