#
# Script for rendering all the scenes for benchmark purpose.
#

SCENES="ulmBox classroom office cabin armadillo Atrium2 conference cruiser asian_dragon"
SCENES_DIR="scenes"
OUT_FILE=bench.txt

echo "-- BENCHMARK -- " > $OUT_FILE

for SCENE in $SCENES
do
	echo $SCENE >> $OUT_FILE
	./rtstatic $SCENES_DIR/$SCENE.ra2 $SCENES_DIR/$SCENE.cam render.png >> $OUT_FILE
	echo "------------" >> $OUT_FILE
	convert render.png $SCENE.eps
	cp render.png renders/$SCENE.png
	echo "rendered $SCENE "
done
