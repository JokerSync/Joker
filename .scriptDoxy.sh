#!/bin/bash          
# This script runs Doxygen an throw an error if
# Doxygen generate an error log

echo "Suppress the old error log";
if [ -f doxygen_error.log ];
then
   rm doxygen_error.log;
fi
echo "Done";

#run the doxygen
echo "Running Doxygen...";
doxygen &> /dev/null;
echo "Done";

if [`more doxygen_error.log | wc -l` -eq 0 ];
then
    exit 0;
else
    cat doxygen_error.log;
    echo "";
    if [`more doxygen_error.log | wc -l` -lt 10 ]
    then
        echo " ----------------";
        echo "| Doxygen founds" `more doxygen_error.log | wc -l` "errors |";
        echo " ----------------";
    elif [`more doxygen_error.log | wc -l` -lt 100 ]
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
