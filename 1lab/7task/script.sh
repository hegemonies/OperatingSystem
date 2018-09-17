#!/bin/bash

ls *.c
read -r -p "Выберите файл для компиляции и выполнения:  " answer
if [[ $answer != "" ]]; then
	gcc -std=c99 -Wall $answer && ./a.out
else 
	echo "Неверно"
fi
