#!/bin/bash

mask=""
dest=""
action=""

while [[ -n $1 ]]; do
	arg=$1
	if [[ $arg = "-m" ]]; then
		shift
		arg=$1
		while [[ $arg != "cp" && $arg != "rm" && $arg != "mv" && $arg != "-d" && $arg != "-m" && -n $arg ]]; do
			mask=$mask" "$1
			shift
			arg=$1
		done
	fi
	if [[ $arg = "-d" ]]; then
		dest=$2
		shift
	fi
	if [[ $arg = "cp" || $arg = "rm" || $arg = "mv" ]]; then
		action=$arg
	fi
	shift
done

if [[ $arg = "rm" ]]; then
	echo $action $mask
	$action $mask
else
	echo $action $mask $dest
	$action $mask $dest
fi