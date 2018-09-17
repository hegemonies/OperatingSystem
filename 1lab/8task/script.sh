#!/bin/bash
# archivNAME=$1


# while [ -n $2 ]; do
# 	# array+=($2)
# 	dirs=$dirs" "$2
# 	# echo ${array[@]}
# 	shift
# done

# # echo ${array[@]}
# echo $dirs

archivNAME=$1
arr=()

while [[ $# > 1 ]]; do
	echo $#
	arr+=" "$2
	shift
done

echo "Массив: "${arr[@]}

if [ -e $archivNAME ] ; then
	echo "Такой файл уже существует"
else
	tar -czf $archivNAME $arr
fi