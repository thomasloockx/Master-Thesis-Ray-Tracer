set terminal postscript eps color blacktext 
set output 'plot_triangle_tests.eps'

set title 'Triangle Tests Comparison'
set xlabel "frame number"
set ylabel "# triangle tests"

plot 'data_refit' using 1:7 title 'refit' with linespoints, \
     'data_rebuild' using 1:7 title 'rebuild' with linespoints, \
     'data_fuzzy' using 1:7 title 'fuzzy' with linespoints, \
     'data_twolevelrefit' using 1:7 title 'modecomp/refit' with linespoints;
