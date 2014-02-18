Joker
=====

This depot hosts the Joker projet components:

* applications
* libraries
* tests

Compilation information :
-------------------------

[![Build Status](https://travis-ci.org/Phonations/Joker.png?branch=master)](https://travis-ci.org/Phonations/Joker)

In order to compile the  project you will need to follow the instructions depending on your system :

* [Mac OS](#mac)
* [Linux](#linux) _(soon coming)_
* [Windows](#windows) _(soon coming)_

***


## <a name="mac"></a> Mac Os:

You need to have Mac Os 10.8+ installed to make Joker work on your computer. We also recommand you the [5.2.x Qt version](http://download.qt-project.org/official_releases/online_installers/qt-opensource-mac-x64-1.5.0-1-online.dmg).

### Xcode

You need to have [Xcode](https://developer.apple.com/xcode/) installed and the command line tools to compile on Mac Os using Qt.

### Homebrew

[Homebrew](http://brew.sh/) is a very powerful package installer. Let's install it :

    ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
    
### The dependencies :

You can install the dependencies automatically or manually:

#### Automatic installation

Just run the *bootstrap* script:

    ./scripts/bootstrap.sh
    
#### Manual install

You can install all the dependencies via __brew__ :

    brew install sdl2 sdl2_ttf sdl2_image libav portaudio
    
    brew install ffmpeg --with-fdk-aac --with-ffplay --with-freetype --with-frei0r --with-libass --with-libcaca --with-libvo-aacenc --with-libvorbis --with-libvpx --with-openjpeg --with-openssl --with-opus --with-rtmpdump --with-schroedinger --with-speex --with-speex --with-theora --with-tools
    
    
Then as __macdeployqt__ doesn't work well with brew, you'll have to change the libs permissions

    find /usr/local/Cellar/ -perm -ugo+r -iname "*dylib*" -exec chmod 644 {} \;

Unfortunatly, the __ltc__ libs are not (yet) installable via brew but this is not a big deal.
Get the latest release:

    wget https://github.com/x42/libltc/releases/download/v1.1.3/libltc-1.1.3.tar.gz
    
Extract it:

    tar -xvzf libltc-1.1.3.tar.gz
    
Then configure the environment :

    aclocal; autoheader; glibtoolize –copy; autoconf; automake –gnu –add-missing –copy
    
And finaly configure and install :

    ./configure && make && sudo make install

### The Leap SDK _(optionnal)_

Before any download or installation, you must know that the **leap** have been skipped for the moment.

Available [here](https://developer.leapmotion.com).

You must then define the following global variable in order to compile with the LEAP: 

    launchctl setenv LEAP_SDK /path/to/sdk/root


***
## <a name="linux"></a> Linux:

![Coming soon](http://openclipart.org/image/300px/svg_to_png/118519/Red_Coming_Soon_Stamp.png)
    
***
## <a name="windows"></a> Windows:

![Coming soon](http://openclipart.org/image/300px/svg_to_png/118519/Red_Coming_Soon_Stamp.png)


	


