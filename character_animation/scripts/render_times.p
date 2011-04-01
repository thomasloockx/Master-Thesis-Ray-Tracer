set terminal postscript eps color blacktext 
set output 'plot_render_times.eps'

set title 'Render Time Comparison'
set xlabel "frame number"
set ylabel "render time (s)"

plot 'data_refit' using 1:8 title 'refit' with linespoints, \
     'data_rebuild' using 1:8 title 'rebuild' with linespoints, \
     'data_fuzzy' using 1:8 title 'fuzzy' with linespoints, \
     'data_twolevelrefit' using 1:8 title 'modecomp/refit' with linespoints;
