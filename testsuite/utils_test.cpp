#include <utils_test.h>

void Utils_Test::max2()
{
	QCOMPARE(max<int>(-1, 2), 2);
	QCOMPARE(max<float>(-1.3f, 2.3f), 2.3f);
}

void Utils_Test::max3()
{
	QCOMPARE(max<int>(1,2,3), 3);
	QCOMPARE(max<float>(-1,1,1), 1.0f);
}

void Utils_Test::min3()
{
	QCOMPARE(min<float>(1,1,-1), -1.0f);
}
