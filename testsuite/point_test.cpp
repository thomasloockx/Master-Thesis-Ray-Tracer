#include <point_test.h>

void Point_Test::init()
{
	p = Point(1,2,3);
	q = Point(5,2,1);
	v = Vector(1,2,3);
}

void Point_Test::constructor()
{
	QCOMPARE(1.0F, p.x());
	QCOMPARE(2.0F, p.y());
	QCOMPARE(3.0F, p.z());
}

void Point_Test::equality()
{
	Point p(1,2,3);
	Point w(p);

	QVERIFY(p == w);
	QVERIFY(p != q);
}

void Point_Test::add()
{
	Point sum = p + v;
	QVERIFY(sum == Point(2,4,6));
}

void Point_Test::dif()
{
	QVERIFY((p-q) == Vector(-4,0,2));
}

void Point_Test::distance()
{
	QCOMPARE(p.distance(q), 4.472135954999F);
}
