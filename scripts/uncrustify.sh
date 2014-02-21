#!/bin/bash          
# This script runs Uncrustify

uncrustify -c .uncrustify.cfg */*/*.cpp */*/*.h --replace --no-backup

n=$(git diff | wc -l)
git diff --shortstat
exit $n

