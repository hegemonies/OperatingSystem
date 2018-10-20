set term png\
    size 1500, 500\
    enhanced\
    font 'Arial, 10'

set xzeroaxis lt -1
set yzeroaxis lt -1

set grid xtics lc rgb  '#555555' lw 1 lt 0
set grid ytics lc rgb  '#555555' lw 1 lt 0

set xtics axis
set ytics axis

set key left top

# set output 'result100.png'
# set title "Quick Sort 100"
# plot 'res_100.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

# set output 'result1000.png'
# set title "Quick Sort 1000"
# plot 'res_1000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

# set output 'result10000.png'
# set title "Quick Sort 10000"
# plot 'res_10000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

# set output 'result100000.png'
# set title "Quick Sort 100000"
# plot 'res_100000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

# set output 'result1000000.png'
# set title "Quick Sort 1000000"
# plot 'res_1000000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

set output 'result100.png'
set title "Quick Sort 100"
plot 'speedup100.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

set output 'result1000.png'
set title "Quick Sort 1000"
plot 'speedup1000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

set output 'result10000.png'
set title "Quick Sort 10000"
plot 'speedup10000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

set output 'result100000.png'
set title "Quick Sort 100000"
plot 'speedup100000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'

set output 'result1000000.png'
set title "Quick Sort 1000000"
plot 'speedup1000000.txt' using 1:2 with linespoints lw 1 lt rgb 'red'