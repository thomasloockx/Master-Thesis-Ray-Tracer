#ifndef MATRIX4_TEST_H
#define MATRIX4_TEST_H

#include <cal3dadapter.h>
#include <cfgparser.h>
#include <matrix4.h>

#include <QtTest/QtTest>

using namespace rt;

class Matrix4_Test : public QObject
{
	Q_OBJECT;

	CFGParser* parser;
	CalModel* model;
	Cal3dAdapter* adapter;

private slots:
	void initTestCase();
	void matrix4inverse();
	void matrix4mul();
	void cleanupTestCase();
};

#endif
