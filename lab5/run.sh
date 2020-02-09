#!/usr/bin/env bash

file=${1%.cpp}
if [[ -z $file ]];then file='main';fi

echo "main:" > Makefile 
echo "	g++ ${file}.cpp -lGL -lglut -o main.exe -I /usr/lib/x86_64-linux-gnu/libGL.so.1 /usr/lib/x86_64-linux-gnu/libGLU.so.1" >> Makefile 

make
chmod +x main.exe
./main.exe