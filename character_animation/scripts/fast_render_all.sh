#! /bin/sh
# Script to render all movies fast with all algorithms
# Usage: render_all_fast.sh Algorithm NbFrames Resolution

CHARACTERS="cally skeleton paladin"
ANIMATIONS="walking jogging freestyle"


# arguments
ALGORITHM=$1
NB_FRAMES=$2
RESOLUTION=$3
STATS=$4

for character in $CHARACTERS
do
	for animation in $ANIMATIONS
	do
		echo ./character_animation $ALGORITHM $character $animation $NB_FRAMES $RESOLUTION $STATS;
		echo "# Rendered animation "$character" "$animation;
		#convert render*png $character\_$animation\_$NB_FRAMES\_$RESOLUTION.gif;
		#echo "# Generated movie "$character" "$animation;
	done
done
