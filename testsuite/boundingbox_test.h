#ifndef BOUNDINGBOX_TEST_H
#define BOUNDINGBOX_TEST_H

#include <boundingbox.h>
#include <ray.h>

#include <QtTest/QtTest>

using namespace rt;

class BoundingBox_Test : public QObject
{
	Q_OBJECT

	BoundingBox box1;
	BoundingBox box2;
	BoundingBox box3;
	Ray ray1;
	Ray ray2;
	Ray ray3;

private slots:
	void init();
	void hit() const;
	void add() const;
	void equals() const;
	void area() const;
	void center() const;
	void longestSideLength() const;
	void longestAxis() const;
	void shortestSideLength() const;
	void shortestAxis() const;
	void contains() const;
	void globalBox() const;
};

#endif
