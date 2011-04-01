CHARACTERS="cally paladin skeleton"
ANIMATIONS="walking jogging freestyle"

# arguments
ALGORITHM=$1
NB_FRAMES=$2
RESOLUTION=$3

./character_animation $ALGORITHM skeleton freestyle $NB_FRAMES $RESOLUTION stats > data_skeleton_freestyle
echo "Rendered Animation";

./character_animation $ALGORITHM paladin jogging $NB_FRAMES $RESOLUTION stats > data_paladin_jogging
echo "Rendered Animation";

./character_animation $ALGORITHM cally walking $NB_FRAMES $RESOLUTION stats > data_cally_walking
echo "Rendered Animation";

./character_animation $ALGORITHM cally freestyle $NB_FRAMES $RESOLUTION stats > data_cally_freestyle
echo "Rendered Animation";

echo "Done";

gnuplot all_anim_frame_times.p
mv plot_all_anims_frame_times.eps $ALGORITHM\_frame_times.eps

gnuplot all_anim_render_times.p
mv plot_all_anims_render_times.eps $ALGORITHM\_render_times.eps

gnuplot all_anim_update_times.p
mv plot_all_anims_update_times.eps $ALGORITHM\_update_times.eps

gnuplot all_anim_ratio_ur.p
mv plot_all_anims_ratio_ur.eps $ALGORITHM\_ratio_ur.eps

#rm data_*
