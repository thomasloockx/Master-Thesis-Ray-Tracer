#include <matrix_test.h>
#include <utils.h>

void Matrix_Test::init()
{
	m = Matrix( 
		1, 2, 3,
		4, 5, 6,
		7, 8, 9	);

	n = Matrix( 
		1, 2, 3,
		4, 5, 6,
		7, 8, 9	);

	o = Matrix( 
		-3, 2, 3,
		4, 5, 6,
		7, 8, 9	);

	t = Matrix( 
		1, 4, 7,
		2, 5, 8,
		3, 6, 9	);

	r = Matrix( 
		1, 2, 3,
		4, 5, 6,
		7, 8, 10);

	q  = Matrix( 
		2, 4, 6,
		8, 10, 12,
		14, 16, 18	);

	sum = Matrix( 
		2, 4, 6,
		8, 10, 12,
		14, 16, 18);

	prod = Matrix(
	30,    36,    42,
    66,    81,    96,
   102,   126,   150);

	zeros = Matrix(0,0,0,0,0,0,0,0,0);

	v = Vector(1,2,3);
	p = Point(1,2,3);
}

void Matrix_Test::constructor()
{
	QCOMPARE(2.0F, m(0,1));
	QCOMPARE(9.0F, m(2,2));
	
	m(1,1) = 69;
	QCOMPARE(69.0F, m(1,1));
}

void Matrix_Test::getters()
{
	QVERIFY(m.getRow(1) == Vector(4,5,6));
	QVERIFY(m.getColumn(2) == Vector(3,6,9));
};

void Matrix_Test::setters()
{
	m.setRow(1, Vector(1,1,1));
	m.setColumn(2, Vector(1,1,1));

	QVERIFY(m.getRow(1) == Vector(1,1,1));
	QVERIFY(m.getColumn(2) == Vector(1,1,1));
}

void Matrix_Test::add()
{
	QVERIFY(sum==(m + n));
}

void Matrix_Test::dif()
{
	QVERIFY(zeros == (m - n));
}

void Matrix_Test::product()
{
	QVERIFY(prod == (m * n));
}

void Matrix_Test::det()
{
	QCOMPARE(o.det(), 12.0F);
}

void Matrix_Test::solve()
{
	Vector rhs(1,2,3);
	Vector result = o.solve(rhs);
	QVERIFY(result == Vector(0,0,.3333333333F));
}

void Matrix_Test::transpose()
{
	QCOMPARE(t, m.transpose());
}

void Matrix_Test::vector_product()
{
	QCOMPARE(Vector(14, 32, 50), m * v);
	QCOMPARE(Point(14, 32, 50), m * p);
}

void Matrix_Test::scalar()
{
	Matrix doubled = m * 2; 
	QCOMPARE(q, doubled);
}

void Matrix_Test::inverse()  
{
	Matrix inv(
	-6.66666666666666e-01F,  -1.33333333333333e+00F,   1.00000000000000e+00F,
	-6.66666666666667e-01F,   3.66666666666667e+00F,  -2.00000000000000e+00F,
	1.00000000000000e+00F,  -2.00000000000000e+00F,   1.00000000000000e+00F);
	QCOMPARE(r.inverse(), inv);


     Matrix inv2
            (-0.66667F, -0.66667F, 1.00000F,
            -1.33333F,  3.66667F, -2.00000F,
            1.00000F,  -2.00000F,  1.00000F);
    t(2,2) = 10;
    QCOMPARE(t.inverse(), inv2);
}
