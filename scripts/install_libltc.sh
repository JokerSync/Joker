#!/bin/bash

echo "Install libltc"
if [ "$(uname)" == "Darwin" ]; then
echo "Mac OS X detected"

git clone https://github.com/x42/libltc.git
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
echo "Removing libltc..."
rm -rf libltc

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
echo "Linux detected"

curl -L https://github.com/x42/libltc/releases/download/v1.1.3/libltc-1.1.3.tar.gz -o libltc-1.1.3.tar.gz 
tar -xvzf libltc-1.1.3.tar.gz
cd libltc-1.1.3
./configure
make
sudo make install
cd ..
rm libltc-1.1.3.tar.gz
rm -rf libltc-1.1.3

fi

echo "Done."
