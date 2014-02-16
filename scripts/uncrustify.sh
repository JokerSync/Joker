#!/bin/bash          
# This script runs Uncrustify

uncrustify -c .uncrustify.cfg */*/*.cpp */*/*.h --replace --no-backup

