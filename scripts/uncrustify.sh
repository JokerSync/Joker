#!/bin/bash          
# This script runs Uncrustify

uncrustify -c .uncrustify.cfg */*/*.cpp */*/*.h --replace --no-backup

n=$(git diff --dirstat=files | wc -l)
git status
exit $n

