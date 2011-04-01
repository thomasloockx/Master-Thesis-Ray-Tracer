#ifndef POINT_TEST_H
#define POINT_TEST_H

#include <point.h>
#include <vector.h>

#include <QtTest/QtTest>

using namespace rt;

class Point_Test : public QObject
{
	Q_OBJECT

	Point p;
	Point q;
	Vector v;

private slots:
	void init();
	void constructor();
	void equality();
	void add();
	void dif();
	void distance();
};

#endif
