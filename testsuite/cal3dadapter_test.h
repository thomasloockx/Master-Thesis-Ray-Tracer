#ifndef CAL3DADAPTER_TEST_H
#define CAL3DADAPTER_TEST_H

#include <cal3dadapter.h>
#include <cfgparser.h>
#include <cal3d.h>
#include <triangle.h>

#include <QtTest/QtTest>

using namespace rt;

class Cal3dAdapter_Test : public QObject
{
	Q_OBJECT

	CFGParser* parser;
	CalModel* model;
	Cal3dAdapter* adapter;

	Triangle triangle0;
	HitRecord hitRecord;

private slots:
	void initTestCase();
	void transformToSpace();
	void transformToSpaceWithBB();
	void checkTriangleInFuzzyBox();
	void cleanupTestCase();
};

#endif
