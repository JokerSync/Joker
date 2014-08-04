#!/bin/bash          
# This script runs Uncrustify

uncrustify -c .uncrustify.cfg app/*/*.cpp app/*/*.h --replace --no-backup
uncrustify -c .uncrustify.cfg libs/*/*.cpp libs/*/*.h --replace --no-backup
uncrustify -c .uncrustify.cfg tests/*/*.cpp tests/*/*.h --replace --no-backup

n=$(git diff --dirstat=files | wc -l)
git status
exit $n

