#!/bin/bash
while [[ $# > 0 ]]; do
    ./main 100 $1 >> res_100.txt
    shift
done
echo "Finish"

# let "threshold = 1"
# let "size = 100"

for (( i = 0; i < 5; i++ )); do
	let "size = 10 ** ($i + 2)"
	for (( j = 0; j < 10; j++ )); do
		let "threshold = 10 ** $j"
		./main $size $threshold >> res_$size.txt
	done
done