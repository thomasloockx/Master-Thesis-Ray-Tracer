#include <cal3dadapter_test.h>
#include <cluster.h>

void Cal3dAdapter_Test::initTestCase()
{
	parser = new CFGParser("skeleton", 
						   "../character_animation/characters/skeleton/",
						   "skeleton.cfg");
	model = parser->model();
	adapter = new Cal3dAdapter(model);
	triangle0 = Triangle(Point(1,0,0), Point(0,1,0), Point(0,0,1));
}

/*
 * Transform both triangle and ray to the local space of 
 * a particular bone and do the intersection in that space.
 */
void Cal3dAdapter_Test::transformToSpace()
{
	const Cluster* cluster = adapter->clusters();
	Ray ray0(Point(), Vector(1,1,1));
	QVERIFY(triangle0.hit(ray0, hitRecord));
	float t0 = hitRecord.t;
	int boneId = 21;	

	// transform both triangle and ray
	Ray rayTransformed(cluster[boneId].world2local() * ray0.origin,
					   cluster[boneId].world2local() * ray0.direction);

	Triangle triangleTransformed(triangle0);	
	triangleTransformed.setA(cluster[boneId].world2local() * triangleTransformed.getA());
	triangleTransformed.setB(cluster[boneId].world2local() * triangleTransformed.getB());
	triangleTransformed.setC(cluster[boneId].world2local() * triangleTransformed.getC());

	hitRecord.reset();
	QVERIFY(triangleTransformed.hit(rayTransformed, hitRecord));
	QCOMPARE(t0, hitRecord.t);
}

/*
 * Check the simple traversal scheme by first checking with the world
 * coord bbox, then transforming in the local coordinate system and
 * checking the bbox there.
 */
void Cal3dAdapter_Test::transformToSpaceWithBB()
{
	int boneId = 12;
	int triangleId = 43;

	adapter->update(1);
	adapter->fillClusterWithCurrentPose();
	const Cluster* cluster = adapter->clusters();

	QVERIFY(!cluster[boneId].empty());
	QVERIFY(cluster[boneId].nbTriangles() > triangleId);

	Triangle worldTriangle = cluster[boneId].triangles()[triangleId];
	BoundingBox fuzzyBox = cluster[boneId].fuzzyBoxes()[triangleId];
	
	adapter->fillClusterWithResidualPose();
	Triangle localTriangle = cluster[boneId].triangles()[triangleId];	

	QVERIFY(localTriangle != worldTriangle);	
	//QVERIFY(fuzzyBox.contains(localTriangle));

	Ray ray(Point(), worldTriangle.center() - Point());
	QVERIFY(worldTriangle.hit(ray, hitRecord));

	// transform ray to local coords of box
	Ray tray(cluster[boneId].world2local() * ray.origin,
			 cluster[boneId].world2local() * ray.direction);

	QVERIFY(localTriangle.hit(tray, hitRecord));
	QVERIFY(fuzzyBox.hit(tray));
}

/*
 * Checks for each triangle in a cluster whether it is whithin the fuzzy
 * box of his cluster. In other words are the fuzzy boxes conservative?
 */
void Cal3dAdapter_Test::checkTriangleInFuzzyBox()
{
	adapter->update(0.3f);
	adapter->fillClusterWithResidualPose();
	const Cluster* cluster = adapter->clusters();

	for (int b = 0; b < adapter->nbBones(); ++b) {
		for (int t = 0; t < cluster[b].nbTriangles(); ++t) {
			Triangle triangle = cluster[b].triangles()[t];
			BoundingBox fuzzyBox = cluster[b].fuzzyBox();

			bool isIn = fuzzyBox.contains(triangle);
			QVERIFY(isIn);
		}
	}
}

void Cal3dAdapter_Test::cleanupTestCase()
{
	delete parser;
	delete model;
	delete adapter;
}
