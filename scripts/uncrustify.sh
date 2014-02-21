#!/bin/bash          
# This script runs Uncrustify

uncrustify -c .uncrustify.cfg */*/*.cpp */*/*.h --replace --no-backup &>/dev/null

n=$(git diff --dirstat=files | wc -l)
git diff --shortstat
exit $n

