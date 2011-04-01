#ifndef BBOXBVH_TEST_H
#define BBOXBVH_TEST_H

#include <QtTest/QtTest>

#include <bboxbvh.h>
#include <boundingbox.h>

using namespace rt;

class BBoxBVH_Test : public QObject
{
	Q_OBJECT
#if 0
	BoundingBox* boxes;
	BBoxBVH* tree;

private slots:
	void initTestCase();
	void checkTreeNodes();
	void hit();
	void cleanupTestCase();
#endif
};

#endif
