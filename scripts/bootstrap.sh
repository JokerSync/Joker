#!/bin/bash

set -e

BASEDIR=$(dirname $0)
echo $BASEDIR
source $BASEDIR/install_qt.sh
$BASEDIR/install_ffmpeg.sh
$BASEDIR/install_portaudio.sh
$BASEDIR/install_rtmidi
$BASEDIR/install_ltc.sh
exit 0

