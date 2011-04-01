#! /bin/sh
# Script to generate all the gif movies of the animations
# Usage: generate_movies.sh NbFrames Resolution

CHARACTERS="skeleton"
ANIMATIONS="walking jogging freestyle"

# arguments
ALGORITHM=$1
NB_FRAMES=$2
RESOLUTION=$3

for character in $CHARACTERS
do
	for animation in $ANIMATIONS
	do
		./character_animation $ALGORITHM $character $animation $NB_FRAMES $RESOLUTION;
		convert render*png $character\_$animation\_res\_$RESOLUTION.gif;
		echo "Generated movie "$character" "$animation;
	done
done
