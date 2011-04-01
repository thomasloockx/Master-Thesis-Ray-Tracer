#include <QtTest/QtTest>

#include <boundingbox_test.h>
#include <camera_test.h>
#include <interval_test.h>
#include <matrix_test.h>
#include <partitionalgorithms_test.h>
#include <point_test.h>
#include <raypacket_test.h>
#include <triangle_test.h>
#include <utils_test.h>
#include <vector_test.h>
#include <cal3dadapter_test.h>
#include <matrix4_test.h>
#include <bboxbvh_test.h>

int main(int argc, char **argv)
{
	Vector_Test vector_test;
	QTest::qExec(&vector_test);

	Point_Test point_test;
	QTest::qExec(&point_test);	

	Matrix_Test matrix_test;
	QTest::qExec(&matrix_test);

	Camera_Test camera_test;
	QTest::qExec(&camera_test);

	Triangle_Test triangle_test;
	QTest::qExec(&triangle_test);

	BoundingBox_Test boundingbox_test;
	QTest::qExec(&boundingbox_test);

	Interval_Test interval_test;
	QTest::qExec(&interval_test);

	Utils_Test utils_test;
	QTest::qExec(&utils_test);

	Partitionalgorithms_Test partitionalgorithms_test;
	QTest::qExec(&partitionalgorithms_test);

	RayPacket_Test raypacket_test;
	QTest::qExec(&raypacket_test);

	/* Take long to exec
	Matrix4_Test matrix4_test;
	QTest::qExec(&matrix4_test);

	Cal3dAdapter_Test cal3dadapter_test;
	QTest::qExec(&cal3dadapter_test);
	*/

	BBoxBVH_Test bboxbvh_test;
	QTest::qExec(&bboxbvh_test);

//	SAHPartitioner_Test sah_test;
//	QTest::qExec(&sah_test);
//	SAH_Test sah_test;
//	QTest::qExec(&sah_test);
}
