set terminal postscript eps color blacktext 
set output 'plot_all_anims_render_times.eps'

set title 'Render Time Comparison'
set xlabel "frame number"
set ylabel "render time (s)"

plot 'data_cally_walking' using 1:8 title 'cally_walking' with linespoints, \
     'data_cally_freestyle' using 1:8 title 'cally_freestyle' with linespoints, \
     'data_skeleton_freestyle' using 1:8 title 'skeleton_freestyle' with linespoints, \
     'data_paladin_jogging' using 1:8 title 'palading_jogging' with linespoints;
