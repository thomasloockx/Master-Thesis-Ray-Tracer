#include <interval_test.h>

#include <iostream>

void Interval_Test::overlap()
{
	i1 = Interval(2.53F, 43.3F);
	i2 = Interval(4.2F, 21.5F);
	i3 = Interval(-3.4F, 2.0F);
	i4 = i1;

	QVERIFY(i1.overlap(i2));
	QVERIFY(!i1.overlap(i3));
	QVERIFY(!i2.overlap(i3));
}

void Interval_Test::diff_interval()
{
    Interval i0(2,3);
    Interval i1(-3,-1);
    Interval diff(3,6);
    QCOMPARE(i0 - i1, diff);
}

void Interval_Test::reciproce_interval()
{
    Interval i(2,4);
    Interval rec(0.25f, 0.5f);
    QCOMPARE(i.reciproce(), rec);
    QCOMPARE(Interval(-1,1).reciproce(), Interval(-FLT_MAX, FLT_MAX));
}

void Interval_Test::interval_mul()
{
    Interval i0(1,2);
    Interval i1(0.25f, 0.5f);
    Interval prod(0.25f, 1.0f);
    QCOMPARE(i0 * i1, prod);
    std::cout << Interval(0,0) * Interval(-FLT_MAX, FLT_MAX) << std::endl;
}

void Interval_Test::union_interval()
{
    Interval i_union(-3.4f, 21.5f);
    QCOMPARE(i2 || i3, i_union);
}

void Interval_Test::intersection_interval()
{
    Interval i0(0,2);
    Interval i1(1,3);
    Interval intersection(1,2);
    QCOMPARE(i0 && i1, intersection);
}

void Interval_Test::equals()
{
	QVERIFY(i1 == i4);
	QVERIFY(i2 != i3);
}
