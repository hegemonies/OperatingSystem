#!/bin/bash

mask=""
dest=""
action=""

while [[ -n $1 ]]; do
	arg=$1
	echo "Arg = "$arg
	if [[ $arg = "-m" ]]; then
		mask=$mask" "$2
		shift
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

echo "Action = "$action
echo "Dest = "$dest
echo "Mask = "$mask

if [[ $arg = "rm" ]]; then
	$action $mask
else
	$action $mask $dest
fi