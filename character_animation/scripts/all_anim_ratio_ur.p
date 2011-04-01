set terminal postscript eps color blacktext 
set output 'plot_all_anims_ratio_ur.eps'

set title 'Ratio Update Time / Render Time'
set xlabel "frame number"
set ylabel "update time / render time"

plot 'data_cally_walking' using 1:10 title 'cally_walking' with linespoints, \
     'data_cally_freestyle' using 1:10 title 'cally_freestyle' with linespoints, \
     'data_skeleton_freestyle' using 1:10 title 'skeleton_freestyle' with linespoints, \
     'data_paladin_jogging' using 1:10 title 'palading_jogging' with linespoints;
