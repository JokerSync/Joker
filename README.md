Joker
=====

[![Build Status](https://travis-ci.org/Phonations/Joker.png?branch=master)](https://travis-ci.org/Phonations/Joker) [![Coverage Status](https://coveralls.io/repos/Phonations/Joker/badge.png?branch=master)](https://coveralls.io/r/Phonations/Joker?branch=master)

This depot hosts the Joker projet components:

* applications
* libraries
* tests

Compilation information:
------------------------

In order to compile the  project you will need to follow the instructions depending on your system:

* [Mac OS](#mac)
* [Linux](#linux) _(coming soon)_
* [Windows](#windows)

The project uses *Qt 5.2.1* available here: http://qt-project.org/downloads

***


## <a name="mac"></a> Mac Os:

You need to have Mac Os 10.8+ installed to make Joker work on your computer. We also recommand you the [5.2.x Qt version](http://download.qt-project.org/official_releases/online_installers/qt-opensource-mac-x64-1.5.0-1-online.dmg).

### Xcode

You need to have [Xcode](https://developer.apple.com/xcode/) installed and the command line tools to compile on Mac Os using Qt.

### Homebrew

[Homebrew](http://brew.sh/) is a very powerful package installer. Let's install it:

    ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"

### The dependencies:

You can install the dependencies automatically or manually:

#### Automatic installation

Just run the *bootstrap* script:

    ./scripts/bootstrap.sh

#### Manual install

You can install all the dependencies via __brew__:

    brew install sdl2 sdl2_ttf sdl2_image libav portaudio

    brew install ffmpeg --with-fdk-aac --with-ffplay --with-freetype --with-frei0r --with-libass --with-libcaca --with-libvo-aacenc --with-libvorbis --with-libvpx --with-openjpeg --with-openssl --with-opus --with-rtmpdump --with-schroedinger --with-speex --with-speex --with-theora --with-tools


Then as __macdeployqt__ doesn't work well with brew, you'll have to change the libs permissions

    find /usr/local/Cellar/ -perm -ugo+r -iname "*dylib*" -exec chmod 644 {} \;

Unfortunatly, the __ltc__ libs are not (yet) installable via brew but this is not a big deal.
Get the latest release:

    wget https://github.com/x42/libltc/releases/download/v1.1.3/libltc-1.1.3.tar.gz

Extract it:

    tar -xvzf libltc-1.1.3.tar.gz

Then configure the environment:

    aclocal; autoheader; glibtoolize –copy; autoconf; automake –gnu –add-missing –copy

And finaly configure and install:

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

### SDL

Download the following SDL libraries:

* https://www.libsdl.org/release/SDL2-devel-2.0.3-mingw.tar.gz
* https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.0-mingw.tar.gz
* https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.12-mingw.tar.gz

Then create the following environment variable pointing to the *i686-w64-mingw32* subfolder of each folder:

* *SDL_PATH*
* *SDL_IMAGE_PATH*
* *SDL_TTF_PATH*

### FFMPEG

There is a *Windows* build of the *FFMPEG* library here: http://ffmpeg.zeranoe.com/builds/

Download the *shared* and *dev* 32 bits library:

* http://ffmpeg.zeranoe.com/builds/win32/shared/ffmpeg-20140222-git-44b22bb-win32-shared.7z
* http://ffmpeg.zeranoe.com/builds/win32/dev/ffmpeg-20140222-git-44b22bb-win32-dev.7z

Extract it with [7zip](http://www.7-zip.org/) in the folder of your choice and set the following environment variable:

* *FFMPEG_DEV_PATH*
* *FFMPEG_SHARED_PATH*

### Portaudio

Download *portaudio* precompiled library here: https://github.com/adfernandes/precompiled-portaudio-windows

Extract it with [7zip](http://www.7-zip.org/) in the folder of your choice and set the following environment variable:

* *PORTAUDIO_PATH*

### libltc

Download *libltc* source code from: https://github.com/x42/libltc/releases/download/v1.1.4/libltc-1.1.4.tar.gz

Extract it with [7zip](http://www.7-zip.org/) in the folder of your choice. Create the following environment variable pointing to the *src* subfolder:

* *LTC_PATH*

### Innosetup

Innosetup is required to build an installer for the *release* version of Joker. If you do not want to install Innosetup, choose to build the *debug* version in *Qt Creator*. Otherwise, download it from: http://www.jrsoftware.org/isdl.php

Add Innosetup installation folder to the *PATH* environment variable.

### Qt

Download a *Qt installer* for Windows built against **MinGW** and for **OpenGL** here: http://qt-project.org/downloads

(*MinGW* and *OpenGL* is the only working combination, because Joker uses C++ constructs that are not available in *Visual Studio*, and OpenGL objects that are not available in *Angle*.)

Install to the folder of your choice.

Then use *Qt Creator* to open and build the Joker project *app.pro*.

Note that you can set all the required environment variables described above directly in the project settings inside Qt Creator.

Test files:
------------

In order to test *Joker* and its subproject, you'll want to work on an example file: http://www.phonations.com/tmwtga

It contains:

* A *DetX* file containing the english transcription
* A video file
* An audio file
