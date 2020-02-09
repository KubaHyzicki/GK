#!/usr/bin/env bash

sleepTime=${1}

for x in `ls | grep -E '^[0-9]'`;do
	g++ ${x} -lGL -lglut -o main.exe
	chmod +x main.exe
	./main.exe &
	sleep ${sleepTime}
	kill `ps -e | grep main.exe | sed 's/ .*//g'`
done
make
./main.exe &
sleep ${sleepTime}
kill `ps -e | grep main.exe | sed 's/ .*//g'`

