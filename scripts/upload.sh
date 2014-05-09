#!/bin/bash

# This script upload file to our server

for i
do
  echo "Sending $i..."
  curl --form "file=@$i" http://www.phonations.com/feedback/serv.php
done
