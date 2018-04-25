#!/bin/bash

#set -e

SHORT_VER=`echo $QTVER | cut -b1-2`

echo "Install Qt5"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

if [[ $QTVER != 532 ]]; then brew update; fi

brew install qt5 --force-bottle
brew link --force qt5
export QMAKESPEC=macx-clang

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

# qt5 is installed with Travis apt addon
#sudo add-apt-repository -y ppa:beineri/opt-qt${QTVER}
#sudo apt-get -qy update
#sudo apt-get -qy install qt${SHORT_VER}base qt${SHORT_VER}xmlpatterns libboost-all-dev

. /opt/qt${SHORT_VER}/bin/qt${SHORT_VER}-env.sh
export QMAKESPEC=linux-clang

fi

echo "Done."
