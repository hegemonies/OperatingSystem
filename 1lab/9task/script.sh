#!/bin/bash

if [[ $# < 1 ]]; then
	echo "Нет опций. ЕГГОГ"
	exit 1
fi

while getopts m:c:n: opt ; do
	case $opt in
		m) 
			let "index = $OPTIND - 1"
			let "arg = $"
			echo $arg
			mask=""
			while [[ -n $arg ]]; do
				mask=$mask" "$arg
				# echo $mask
				shift
			done

			if [[ -e $mask ]]; then
				rm $mask
			fi
		;;
		c)
			if [[ -n $OPTARG ]]; then
				mask=$OPTARG
			fi

			if [[ -e $mask ]]; then
				read -r -p "Напишите путь куда копировать: " copyTo
				cp $mask $copyTo
			fi
		;;
		n)
			if [[ -n $OPTARG ]]; then
				mask=$OPTARG
			fi

			if [[ -e $mask ]]; then
				read -r -p "Напишите новое имя файла: " copyTo
				mv $mask $copyTo
			fi
		;;
	esac
done