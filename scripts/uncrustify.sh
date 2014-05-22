#!/bin/bash          
# This script runs Uncrustify

uncrustify -c .uncrustify.cfg app/*/*.cpp */*/*.h --replace --no-backup
uncrustify -c .uncrustify.cfg libs/*/*.cpp */*/*.h --replace --no-backup
uncrustify -c .uncrustify.cfg tests/*/*.cpp */*/*.h --replace --no-backup

n=$(git diff --dirstat=files | wc -l)
git status
exit $n

