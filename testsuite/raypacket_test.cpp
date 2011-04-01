#include <boundingbox.h>
#include <raypacket.h>
#include <raypacket_test.h>

using namespace rt;

void RayPacket_Test::calcIAData()
{
    RayPacket packet(4);
    packet.ray(0) = Ray(Point(-3, 1.5f, 0), Vector(2,1,0));
    packet.ray(1) = Ray(Point(-2, 1, 0),    Vector(1,3,0));
    packet.ray(2) = Ray(Point(-1, 2, 0),    Vector(1,2,0));
    packet.ray(3) = Ray(Point(-1, 2, 0),    Vector(1,2,0));

    packet.calcIAData();

    QCOMPARE(packet.iaData.origin[0], Interval(-3,-1));
    QCOMPARE(packet.iaData.origin[1], Interval(1,2));
    QCOMPARE(packet.iaData.direction[0], Interval(1,2));
    QCOMPARE(packet.iaData.direction[1], Interval(1,3));
}

void RayPacket_Test::bboxMiss()
{
    RayPacket packet(4);
    packet.ray(0) = Ray(Point(-3, 1.5f, 0), Vector(2,1,0));
    packet.ray(1) = Ray(Point(-2, 1, 0),    Vector(1,3,0));
    packet.ray(2) = Ray(Point(-1, 2, 0),    Vector(1,2,0));
    packet.ray(3) = Ray(Point(-1, 2, 0),    Vector(1,2,0));
    packet.calcIAData();

    BoundingBox box(2,3,-1,2,0,0);            
    QVERIFY(box.miss(packet));
    
    box = BoundingBox(0,1,2,5,0,1);
    QVERIFY(!box.miss(packet));
}
