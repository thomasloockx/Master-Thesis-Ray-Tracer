#include <vector_test.h>

void Vector_Test::init()
{
	v = Vector(1,2,3);
	w = Vector(-1,4,2);
}

void Vector_Test::constructor()
{
	QCOMPARE(1.0F, v.x());
	QCOMPARE(2.0F, v.y());
	QCOMPARE(3.0F, v.z());

	Vector w = v;
	QVERIFY(v == w);
}

void Vector_Test::equality()
{
	v.normalize();
	QVERIFY(v == Vector(0.267261F, 0.534522F ,0.801784F));
}

void Vector_Test::add()
{
	Vector sum = v + w;
	QVERIFY(sum == (Vector(0,6,5)));	
}

void Vector_Test::dif()
{
	Vector dif = v - w;
	QVERIFY(dif == (Vector(2,-2,1)));	
}

void Vector_Test::cross()
{
	Vector cross = v * w;
	QVERIFY(cross == (Vector(-8,-5,6)));	
}

void Vector_Test::dot()
{
	float dot = v % w;
	QCOMPARE(13.0F, dot);	
}

void Vector_Test::getnorm()
{
	QCOMPARE(v.norm(), 3.74165738677394F);
}

void Vector_Test::normalize()
{
	v.normalize();
	QVERIFY(v == Vector(0.267261F, 0.534522F ,0.801784F));
}
