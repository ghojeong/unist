#!/bin/bash

cd build

pintos-mkdisk filesys.dsk --filesys-size=2

pintos -f -q

pintos -p tests/userprog/$1 -a $1 -- -q 2>/dev/null | grep  "Putting"

pintos -p ../../tests/userprog/sample.txt -a sample.txt -- -q 2>/dev/null | grep  "Putting"

# If you need putting more files, then add lines like below 
# pintos -p tests/userprog/child-simple -a child-simple -- -q 2>/dev/null | grep  "Putting"

pintos  run "$1"


