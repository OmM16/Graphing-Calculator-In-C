set title 'Equation Plot'
set xlabel 'x'
set ylabel 'y'
set grid
set yrange [-10:10]
set style line 1 lc rgb '#0060ad' lt 1 lw 2
plot 'data.txt' with lines ls 1 title 'y=f(x)'
pause -1
