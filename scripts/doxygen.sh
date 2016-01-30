#!/bin/bash
# This script runs Doxygen an throw an error if
# Doxygen generate an error log

if [ $# -gt 1 ]
then
    echo "Unproper usage, you should give me only one argument"
    exit 1
fi

mkdir -p doc

#run the doxygen
pwd
echo "Running Doxygen...";

if [ $# -eq 1 ]
then
    doxygen
else
    doxygen .doxygen
fi

echo "Done";

if [ ! -f doxygen_error.log ];
then
  echo " ----------------------------------------------------------";
  echo "| doxygen_error.log not found!!!";
  echo " ----------------------------------------------------------";
  exit 1;
else
  if [ `more doxygen_error.log | wc -l` -eq 0 ];
  then
    echo ".__   __.  __    ______  _______     __  ";
    echo "|  \ |  | |  |  /      ||   ____|   |  | ";
    echo "|   \|  | |  | |  ,----'|  |__      |  | ";
    echo "|  .    | |  | |  |     |   __|     |  | ";
    echo "|  |\   | |  | |   ----.|  |____    |__| ";
    echo "|__| \__| |__|  \______||_______|   (__) ";
    exit 0;
  else
    cat doxygen_error.log;
    echo "";
    if [ `more doxygen_error.log | wc -l` -lt 10 ]
    then
        echo " ----------------------------------------------------------";
        echo "| Doxygen founds" `more doxygen_error.log | wc -l` "errors (not bad, but you can do better !)|";
        echo " ----------------------------------------------------------";
    elif [ `more doxygen_error.log | wc -l` -lt 100 ]
    then
        echo " ---------------------------------------";
        echo "| Doxygen founds" `more doxygen_error.log | wc -l` "errors (this is bad) |";
        echo " ---------------------------------------";
    else
        echo " --------------------------------------------";
        echo "| Doxygen founds" `more doxygen_error.log | wc -l` "errors (this is very bad) |";
        echo " --------------------------------------------";
    fi
    exit 1;
  fi
fi
