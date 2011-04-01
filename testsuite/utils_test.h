#ifndef UTILS_TEST_H
#define UTILS_TEST_H

#include <utils.h>

#include <QtTest/QtTest>

using namespace rt;

class Utils_Test : public QObject
{
	Q_OBJECT

private slots:
	void max2();
	void max3();
	void min3();
};

#endif
