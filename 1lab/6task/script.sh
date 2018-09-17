#!/bin/bash
FILEcheck=$1
FILE=$2
COMPIL=$3
RUN=$4

if [[ $FILEcheck = '-f' ]]; then
	if [[ -f $FILE && -r $FILE ]]; then
		cat $FILE
	else 
		FILE='.bashrc'
	fi
	
	if [[ $COMPIL = '-c' ]]; then
		gcc -Wall $FILE
		if [[ $RUN = '-r' && -f 'a.out' ]]; then
			./a.out
		fi
	fi
else
	FILE='.bashrc'
fi

