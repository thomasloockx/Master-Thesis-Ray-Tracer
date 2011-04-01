#ifndef INTERVAL_TEST_H
#define INTERVAL_TEST_H

#include <interval.h>

#include <QtTest/QtTest>

using namespace rt;

class Interval_Test : public QObject
{
	Q_OBJECT

	Interval i1;
	Interval i2;
	Interval i3;
	Interval i4;

private slots:
	void overlap();
    void diff_interval();
    void reciproce_interval();
    void interval_mul();
    void union_interval();
    void intersection_interval();
	void equals();
};

#endif
