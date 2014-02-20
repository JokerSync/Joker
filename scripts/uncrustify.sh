#!/bin/bash          
# This script runs Uncrustify

uncrustify -c .uncrustify.cfg */*/*.cpp */*/*.h --replace --no-backup

n=$(git diff | wc -l)
echo "diff line number detected: $n"
exit $n

