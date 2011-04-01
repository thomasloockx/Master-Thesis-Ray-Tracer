#include <triangle_test.h>

void Triangle_Test::init()
{

	triangle = Triangle(Point(1,0,0), Point(0,1,0), Point(0,0,1));
	ray1 = Ray(Point(0,0,0), Vector(1,1,1));
	ray2 = Ray(Point(0,0,0), Vector(-1,-1,-1));
}

void Triangle_Test::boundingBox()
{
	triangle = Triangle(Point(1,0,0), Point(0,1,0), Point(0,0,1));
	QCOMPARE(triangle.boundingBox(), BoundingBox(0,1,0,1,0,1));
	
	// was a bug
	Triangle t(Point(-1, 1, 1), Point(1, 1, 1), Point(-1, 1, -1));
	QCOMPARE(t.boundingBox(), BoundingBox(-1,1,1,1,-1,1));
}

void Triangle_Test::hit()
{
	QVERIFY(triangle.hit(ray1, h));	
	QVERIFY(!triangle.hit(ray2, h));
}

void Triangle_Test::center()
{
	triangle = Triangle(Point(1,0,0), Point(0,1,0), Point(0,0,1));
	QCOMPARE(Point((float) 1/3, (float) 1/3, (float) 1/3), triangle.center());
}
