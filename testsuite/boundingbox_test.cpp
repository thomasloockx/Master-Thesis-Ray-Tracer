#include <boundingbox_test.h>
#include <constants.h>
#include <triangle.h>

void BoundingBox_Test::init()
{
	box1 = BoundingBox(-1, 1, -1, 1, -1, 1);
	box2 = BoundingBox(-1, 1, -1, 1, -1, 1);
	box3 = BoundingBox(-1, 4, -3, 1, 0, 1);

	ray1 = Ray(Point(2,0,0), Vector(-1, 0, 1));
	ray2 = Ray(Point(2,0,0), Vector(-1, -0.1F, -0.1F));
	ray3 = Ray(Point(2,0,0), Vector(0, 0, 10));
}

void BoundingBox_Test::hit() const
{
	QVERIFY(box1.hit(ray1));
	QVERIFY(box1.hit(ray2));
	QVERIFY(!box1.hit(ray3));

	// with extra info
	//QVERIFY(!box1.hit(ray2,0,0.5));
	//QVERIFY(box1.hit(ray2,0,2));
}

void BoundingBox_Test::equals() const
{
	QVERIFY(box1 == box2);	
	QVERIFY(box1 != box3);	
	QVERIFY(box2 != box3);	
}

void BoundingBox_Test::add() const
{
	BoundingBox b1(-1, 0, -1, 0, -1, 0);
	BoundingBox b2(0, 1, 0, 1, 0, 1);
    	
	BoundingBox join;
	join += box1;

	QCOMPARE(box1, join);
	QCOMPARE(box1, b1 + b2);
}

void BoundingBox_Test::area() const
{
	QCOMPARE(box1.area(), float(24));
	QCOMPARE(BoundingBox(0,1,0,1,0,1).area(), float(6));
	QCOMPARE(BoundingBox(0,1,0,1,0,0).area(), float(1));
}

void BoundingBox_Test::center() const
{
	QCOMPARE(box1.center(), Point());
	QCOMPARE(box3.center(), Point(1.5F,-1.0F,0.5F));
}

void BoundingBox_Test::longestSideLength() const
{
	QCOMPARE(box3.longestSideLength(), box3.width());
	BoundingBox bugbox(0,1.5F,-0.5F,0.46194F, -0.232419F, 1.03375F);
	QCOMPARE(bugbox.longestSideLength(), 1.5F);

}

void BoundingBox_Test::longestAxis() const
{
	QCOMPARE(box3.longestAxis(), (int) X_AXIS);
}

void BoundingBox_Test::shortestSideLength() const
{
	QCOMPARE(box3.shortestSideLength(), float(1));
}

void BoundingBox_Test::shortestAxis() const
{
	QCOMPARE(box3.shortestAxis(), (int) Z_AXIS);
}

void BoundingBox_Test::contains() const
{
	QVERIFY(box1.contains(Point(0,-0.5F,0.5F)));
	QVERIFY(!box1.contains(Point(0.4F,0,1.2F)));
}

void BoundingBox_Test::globalBox() const
{
	Point p1(1,0,0);
	Point p2(0,1,0);
	Point p3(0,0,1);

	std::vector<Point> points;
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);

	BoundingBox global = BoundingBox::globalBox(points);
	QCOMPARE(BoundingBox(0,1,0,1,0,1), global);

	points.push_back(Point(-1,-1,0));	
	global = BoundingBox::globalBox(points);	
	QCOMPARE(BoundingBox(-1,1,-1,1,0,1), global);

	points.push_back(Point(-2,-2,-2));	
	global = BoundingBox::globalBox(points);	
	QCOMPARE(BoundingBox(-2,1,-2,1,-2,1), global);


	points.push_back(Point(0,0,10));	
	global = BoundingBox::globalBox(points);	
	QCOMPARE(BoundingBox(-2,1,-2,1,-2,10), global);

	std::vector<Triangle*> triangles;
	Triangle a(Point(-1, 1, -1), Point(1, 1, -1), Point(-1, -1, -1));
	Triangle b(Point(1, 1, -1), Point(1, -1, -1),Point(-1, -1, 1)); 
	triangles.push_back(&a);
	triangles.push_back(&b);
	BoundingBox box(BoundingBox::globalBox(triangles));
	QCOMPARE(BoundingBox(-1,1,-1,1,-1,1), box);
}
