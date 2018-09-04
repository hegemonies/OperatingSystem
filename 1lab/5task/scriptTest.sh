#!/bin/bash
printf '%b' 'Введите команду: '
read answer
if [[ $answer = '' ]];
then
	printf '%b\n' 'ЕГГОГ'
else
	eval $answer
fi
