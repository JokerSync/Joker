# Joker

[![Build Status](https://travis-ci.org/JokerSync/Joker.svg?branch=master)](https://travis-ci.org/JokerSync/Joker)
[![Build status](https://ci.appveyor.com/api/projects/status/6w22uq5t7hwwaj8n/branch/master?svg=true)](https://ci.appveyor.com/project/JokerSync/joker/branch/master)
[![Coverage Status](https://coveralls.io/repos/JokerSync/Joker/badge.svg?branch=master)](https://coveralls.io/r/JokerSync/Joker?branch=master)
[![Download](https://img.shields.io/github/downloads/JokerSync/Joker/total.svg)](https://github.com/JokerSync/Joker/releases/latest)

Welcome to the *Joker* project.

![Screenshot](data/img/screenshot.jpg)

Joker installers are available for *macos* and *Windows* in the [release section](https://github.com/JokerSync/Joker/releases/latest).

Manual build instructions can be found below.

## Project structure

This repository hosts the Joker projet components:

- Applications
  - *Joker*: The main project.
  - *SonyTool*: A Sony 9 pin master/slave
  - *LTCTool*: A LTC generator/reader.
  - *MidiTool*: A midi timecode (MTC) generator/reader.
- Libraries
  - *PhTools*: Common basic utilities.
  - *PhCommonUI*: Common user interface component.
  - *PhStrip*: The strip document model.
  - *PhGraphic*: Basic library for displaying graphical object and texts.
  - *PhGraphicStrip*: Specific strip related display routine.
  - *PhAudio*: Basic library for audio playback/recording.
  - *PhVideo*: Video playback library.
  - *PhSync*: Generic sync component.
  - *PhSony*: Sony 9 pin synchronisation interface.
  - *PhLtc*: LTC synchronisation interface.
  - *PhMidi*: Midi synchronisation interface.
* Various test projects for testing the library modules independently or integrated with each others. It contains also unit testing modules in the *specs* directory.

## Compilation information

In order to compile the  project you will need to follow the instructions depending on your system:

* [Mac OS](#mac)
* [Windows](#windows)
* [Linux](#linux) _(to do)_

The project uses *Qt 5.5.1* available here: http://www.qt.io/download/

### <a name="mac"></a> Mac Os:

You need to have Mac Os 10.8+ installed to make Joker work on your computer. We also recommand you the [5.3.x Qt version](http://download.qt-project.org/official_releases/online_installers/qt-opensource-mac-x64-1.5.0-1-online.dmg).

#### Xcode

You need to have [Xcode](https://developer.apple.com/xcode/) installed and the command line tools to compile on Mac Os using Qt.

#### Homebrew

[Homebrew](http://brew.sh/) is a very powerful package installer. Let's install it:

    ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"

#### The dependencies:

You can install the dependencies automatically or manually:

##### Automatic installation

Just run the *bootstrap* script:

    ./scripts/bootstrap.sh

##### Manual install

You can install most of the dependencies via *brew*:

    brew install libav portaudio libltc

    brew install ffmpeg --with-fdk-aac --with-ffplay --with-freetype --with-frei0r --with-libass --with-libcaca --with-libvo-aacenc --with-libvorbis --with-libvpx --with-openjpeg --with-openssl --with-opus --with-rtmpdump --with-schroedinger --with-speex --with-speex --with-theora --with-tools


### <a name="linux"></a> Linux:

Just run the *bootstrap* script:

    ./scripts/bootstrap.sh

### <a name="windows"></a> Windows:

#### FFMPEG

There is a *Windows* build of the *FFMPEG* library here: http://ffmpeg.zeranoe.com/builds/

Download the *shared* and *dev* 32 bits library:

- [http://ffmpeg.zeranoe.com/builds/win32/shared/ffmpeg-20140222-git-44b22bb-win32-shared.7z]()
- [http://ffmpeg.zeranoe.com/builds/win32/dev/ffmpeg-20140222-git-44b22bb-win32-dev.7z]()

Extract it with [7zip](http://www.7-zip.org/) in the folder of your choice and set the following environment variable:

- *FFMPEG_DEV_PATH*
- *FFMPEG_SHARED_PATH*

#### Portaudio

Download *portaudio* precompiled library here: https://github.com/adfernandes/precompiled-portaudio-windows

Extract it with [7zip](http://www.7-zip.org/) in the folder of your choice and set the following environment variable:

- *PORTAUDIO_PATH*

#### libltc

Download *libltc* source code from: https://github.com/x42/libltc/releases/download/v1.1.4/libltc-1.1.4.tar.gz

Extract it with [7zip](http://www.7-zip.org/) in the folder of your choice. Create the following environment variable pointing to the *src* subfolder:

- *LTC_PATH*

#### Innosetup

Innosetup is required to build an installer for the *release* version of Joker. If you do not want to install Innosetup, choose to build the *debug* version in *Qt Creator*. Otherwise, download it from: http://www.jrsoftware.org/isdl.php

Add Innosetup installation folder to the *PATH* environment variable.

#### Qt

Download a *Qt installer* for Windows built against **MinGW** and for **OpenGL** here: http://qt-project.org/downloads

(*MinGW* and *OpenGL* is the only working combination, because Joker uses C++ constructs that are not available in *Visual Studio*, and OpenGL objects that are not available in *Angle*.)

Install to the folder of your choice.

Then use *Qt Creator* to open and build the Joker project *app.pro*.

Note that you can set all the required environment variables described above directly in the project settings inside Qt Creator.
