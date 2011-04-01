#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include <matrix.h>
#include <vector.h>

#include <QtTest/QtTest>

using namespace rt;

class Matrix_Test : public QObject
{
	Q_OBJECT

	Matrix m;
	Matrix n;
	Matrix o;
	Matrix q;
	Matrix t;
	Matrix r;
	Matrix sum;
	Matrix zeros;
	Matrix prod;
	Vector v;
	Point p;
	
private slots:
	void init();
	void constructor();
	void getters();
	void setters();
	void add();
	void dif();
	void product();
	void det();
	void solve();
	void transpose();
	void vector_product();
	void scalar();
	void inverse();
};

#endif
