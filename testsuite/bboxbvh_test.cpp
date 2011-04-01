#include <bboxbvh_test.h>
#include <vector>

#if 0
void BBoxBVH_Test::initTestCase()
{
	boxes = new BoundingBox[3];
	boxes[0] = BoundingBox(0,1,0,1,0,1);
	boxes[1] = BoundingBox(1,2,1,2,1,2);
	boxes[2] = BoundingBox(2,3,2,3,2,3);

	tree = new BBoxBVH(boxes, 3);
}

void BBoxBVH_Test::checkTreeNodes()
{
	QCOMPARE(tree->nbNodes(), 5);
}

void BBoxBVH_Test::hit()
{
	Ray ray(Point(-1,-1,-1), Vector(0,0,-1));
	std::vector<int> hits;
	QVERIFY(!tree->traverse(ray, hits));
	QVERIFY(hits.empty());

	ray = Ray(Point(-1,-1,-1), Vector(1,1,1));
	hits.clear();
	QVERIFY(tree->traverse(ray, hits));
	QVERIFY(hits.size() == 3);
	QCOMPARE(hits[0], 0);
	QCOMPARE(hits[1], 1);
	QCOMPARE(hits[2], 2);

	ray = Ray(Point(0.5f, 0.5f, -1), Vector(0,0,1));
	hits.clear();
	QVERIFY(tree->traverse(ray, hits));
	QVERIFY(hits.size() == 1);
	QCOMPARE(hits[0], 0);
}

void BBoxBVH_Test::cleanupTestCase()
{
	delete[] boxes;
	delete tree;
}
#endif
