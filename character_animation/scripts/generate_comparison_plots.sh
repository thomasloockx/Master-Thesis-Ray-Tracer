character=$1
animation=$2

NB_FRAMES=50
RESOLUTION=512

echo "# Generating Comparison Plots for $character $animation";
./clean_all.sh;

nice -n 19 ./character_animation rebuild $character $animation $NB_FRAMES $RESOLUTION stats > data_rebuild;
nice -n 19 ./character_animation refit $character $animation $NB_FRAMES $RESOLUTION stats > data_refit;
nice -n 19 ./character_animation fuzzy $character $animation $NB_FRAMES $RESOLUTION stats > data_fuzzy;
nice -n 19 ./character_animation twolevelrefit $character $animation $NB_FRAMES $RESOLUTION stats > data_twolevelrefit;

gnuplot frame_times.p
gnuplot update_times.p
gnuplot render_times.p 
gnuplot triangle_tests.p
gnuplot bbox_tests.p
gnuplot update_render_ratio.p

# rename all the files
mv plot_frame_times.eps $character\_$animation\_frame_times.eps
mv plot_update_times.eps $character\_$animation\_update_times.eps
mv plot_render_times.eps $character\_$animation\_render_times.eps
mv plot_triangle_tests.eps $character\_$animation\_triangle_tests.eps
mv plot_bbox_tests.eps $character\_$animation\_bbox_tests.eps
mv plot_update_render_ratio.eps $character\_$animation\_update_render_ratio.eps
#mv *eps pics/

rm data_fuzzy data_refit data_rebuild;
