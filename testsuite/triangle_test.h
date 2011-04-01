#ifndef TRIANGLE_TEST_H
#define TRIANGLE_TEST_H

#include <triangle.h>
#include <ray.h>
#include <hitrecord.h>

#include <QtTest/QtTest>

using namespace rt;

class Triangle_Test : public QObject
{
	Q_OBJECT

	Triangle triangle;
	Ray ray1;
	Ray ray2;
	HitRecord h;

private slots:
	void init();
	void boundingBox();
	void hit();
	void center();
};

#endif
