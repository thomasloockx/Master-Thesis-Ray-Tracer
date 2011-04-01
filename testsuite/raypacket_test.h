#ifndef RAYPACKET_TEST_H
#define RAYPACKET_TEST_H

#include <QtTest/QtTest>

class RayPacket_Test : public QObject
{
	Q_OBJECT

private slots:
    void calcIAData();
    void bboxMiss();
};

#endif
