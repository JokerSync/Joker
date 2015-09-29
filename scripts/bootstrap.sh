#!/bin/bash
BASEDIR=$(dirname $0)
echo $BASEDIR
$BASEDIR/install_ffmpeg.sh
$BASEDIR/install_audio.sh
$BASEDIR/install_ltc.sh
$BASEDIR/install_sdl.sh
exit 0

