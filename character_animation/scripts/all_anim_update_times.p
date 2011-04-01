set terminal postscript eps color blacktext 
set output 'plot_all_anims_update_times.eps'

set title 'Update Time Comparison'
set xlabel "frame number"
set ylabel "update time (s)"

plot 'data_cally_walking' using 1:9 title 'cally_walking' with linespoints, \
     'data_cally_freestyle' using 1:9 title 'cally_freestyle' with linespoints, \
     'data_skeleton_freestyle' using 1:9 title 'skeleton_freestyle' with linespoints, \
     'data_paladin_jogging' using 1:9 title 'palading_jogging' with linespoints;
