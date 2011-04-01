set terminal postscript eps color blacktext 
set output 'plot_update_render_ratio.eps'

set title 'Ratio Update Time - Render Time Comparison'
set xlabel "frame number"
set ylabel "ratio"

plot 'data_refit' using 1:11 title 'refit' with linespoints, \
     'data_rebuild' using 1:11 title 'rebuild' with linespoints, \
     'data_fuzzy' using 1:11 title 'fuzzy' with linespoints, \
     'data_twolevelrefit' using 1:11 title 'modecomp/refit' with linespoints;
