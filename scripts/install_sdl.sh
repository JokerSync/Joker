#!/bin/bash

echo "Installing SDL libraries"

if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

#brew install sdl2 sdl2_image sdl2_ttf 

wget --no-check-certificate http://libsdl.org/release/SDL2-2.0.1.dmg
hdiutil attach SDL2-2.0.1.dmg
sudo cp -r /Volumes/SDL2/SDL2.framework /Library/Frameworks
hdiutil detach /Volumes/SDL2
rm SDL2-2.0.1.dmg

wget --no-check-certificate https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.0.dmg
hdiutil attach SDL2_image-2.0.0.dmg
sudo cp -r /Volumes/SDL2_image/SDL2_image.framework /Library/Frameworks
hdiutil detach /Volumes/SDL2_image
rm SDL2_image-2.0.0.dmg

wget --no-check-certificate https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.12.dmg
hdiutil attach SDL2_ttf-2.0.12.dmg
sudo cp -r /Volumes/SDL2_ttf/SDL2_ttf.framework /Library/Frameworks
hdiutil detach /Volumes/SDL2_ttf
rm SDL2_ttf-2.0.12.dmg

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

#sudo apt-get update -qq
#sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl-ttf2.0-0

echo "Install SDL2"
wget http://libsdl.org/release/SDL2-2.0.1.tar.gz
tar -xvzf SDL2-2.0.1.tar.gz
cd SDL2-2.0.1
./configure
make
sudo make install
cd ..
rm SDL2-2.0.1.tar.gz
rm -rf SDL2-2.0.1

echo "Install SDL2-image"
wget http://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.0.tar.gz
tar -xvzf SDL2_image-2.0.0.tar.gz
cd SDL2_image-2.0.0
./configure
make
sudo make install
cd ..
rm SDL2_image-2.0.0.tar.gz
rm -rf SDL2_image-2.0.0

echo "Install freetype2"
sudo apt-get install libpng3 libpng++-dev
wget http://skylink.dl.sourceforge.net/project/freetype/freetype2/2.5.2/freetype-2.5.2.tar.gz
tar -xvzf freetype-2.5.2.tar.gz
cd freetype-2.5.2
./configure
make
sudo make install
cd ..
rm freetype-2.5.2.tar.gz
rm -rf freetype-2.5.2

echo "Install SDL2-ttf"
wget http://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.12.tar.gz
tar -xvzf SDL2_ttf-2.0.12.tar.gz
cd SDL2_ttf-2.0.12
./configure
make
sudo make install
cd ..
rm SDL2_ttf-2.0.12.tar.gz
rm -rf SDL2_ttf-2.0.12

fi


