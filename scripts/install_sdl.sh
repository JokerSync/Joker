#!/bin/bash

set -e

echo "Installing SDL libraries"

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

brew install sdl2 sdl2_ttf 

#wget --no-check-certificate http://libsdl.org/release/SDL2-2.0.1.dmg
#hdiutil attach SDL2-2.0.1.dmg
#sudo cp -r /Volumes/SDL2/SDL2.framework /Library/Frameworks
#hdiutil detach /Volumes/SDL2
#rm SDL2-2.0.1.dmg

#wget --no-check-certificate https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.12.dmg
#hdiutil attach SDL2_ttf-2.0.12.dmg
#sudo cp -r /Volumes/SDL2_ttf/SDL2_ttf.framework /Library/Frameworks
#hdiutil detach /Volumes/SDL2_ttf
#rm SDL2_ttf-2.0.12.dmg

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

sudo add-apt-repository -y ppa:zoogie/sdl2-snapshots
sudo apt-get update -qq
sudo apt-get -y install libsdl2-dev libsdl2-ttf-dev

fi


