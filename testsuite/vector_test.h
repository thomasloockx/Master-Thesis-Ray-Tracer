#ifndef VECTOR_TEST_H
#define VECTOR_TEST_H

#include <vector.h>

#include <QtTest/QtTest>

using namespace rt;

class Vector_Test : public QObject
{
	Q_OBJECT

	Vector v;
	Vector w;

private slots:
	void init();
	void constructor();
	void equality();
	void add();
	void dif();
	void cross();
	void dot();
	void getnorm();
	void normalize();
};

#endif
