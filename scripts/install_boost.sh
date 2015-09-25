#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew install boost

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

sudo apt-get install libboost-all-dev

fi
