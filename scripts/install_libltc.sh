#!/bin/bash

echo "Install libltc"
#wget https://github.com/x42/libltc/releases/download/v1.1.3/libltc-1.1.3.tar.gz
#tar -xvzf libltc-1.1.3.tar.gz
#cd libltc-1.1.3
#./configure
#make
#sudo make install
#cd ..
#rm libltc-1.1.3.tar.gz
#rm -rf libltc-1.1.3
git clone https://github.com/Phonations/libltc.git
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
