set terminal postscript eps color blacktext 
set output 'plot_update_times.eps'

set title 'Update Time Comparison'
set xlabel "frame number"
set ylabel "update time (s)"

plot 'data_refit' using 1:9 title 'refit' with linespoints, \
     'data_rebuild' using 1:9 title 'rebuild' with linespoints, \
     'data_fuzzy' using 1:9 title 'fuzzy' with linespoints, \
     'data_twolevelrefit' using 1:9 title 'modecomp/refit' with linespoints;
