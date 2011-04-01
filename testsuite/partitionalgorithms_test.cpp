#include <partitionalgorithms_test.h>
#include <constants.h>

#if 0
void Partitionalgorithms_Test::init()
{
	triangleIDs.clear();
	triangles.clear();

	Triangle t1(Point(1,0,0), Point(0,1,0), Point(0,0,1));
	Triangle t2(Point(2,0,0), Point(0,2,0), Point(0,0,2));
	Triangle t3(Point(3,0,0), Point(0,3,0), Point(0,0,3));
	Triangle t4(Point(3,0,0), Point(4,0,0), Point(3,1,0));

	triangles.push_back(t4);
	triangles.push_back(t1);
	triangles.push_back(t3);
	triangles.push_back(t2);

	for (std::size_t i = 0; i < triangles.size(); ++i)
		triangleIDs.push_back(i);
}

void Partitionalgorithms_Test::partition()
{
	int bound = partitionMedian(triangles, 
								triangleIDs, 0, 4, 0);
	
	QCOMPARE(bound, 3);

	// 0 is after split
	QCOMPARE(triangleIDs[3], 0);
}

void Partitionalgorithms_Test::partitionPart()
{
	int bound = partitionMedian(triangles,
								triangleIDs, 0, 2, 0);

	QCOMPARE(bound, 1);

	QCOMPARE(triangleIDs[0], 1);
	QCOMPARE(triangleIDs[1], 0);
}

#endif
