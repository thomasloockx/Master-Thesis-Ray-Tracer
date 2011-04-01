#include <camera_test.h>

void Camera_Test::init()
{
	eye = Point(1,1,1);
	gaze = Vector(-1,-1,-1);
	up = Vector(0,0,1);
	w = Vector(5.77350269189626e-01F, 5.77350269189626e-01F, 5.77350269189626e-01F);
	u = Vector(-7.07106781186548e-01F, 7.07106781186548e-01F, 0);
	v = Vector(-4.08248290463863e-01F, -4.08248290463863e-01F, 8.16496580927726e-01F);
	hfovy = 45;
	vfovy = 45;
	distance = 1;
	nx = 100;
	ny = 100;
	
	cam1 = Camera(eye, gaze, up, hfovy, vfovy, distance, nx, ny);	
	cam2 = Camera(Point(1,0,0), Vector(-1,0,0), Vector(0,0,1), 45, 45, 1, 2, 2);
}

void Camera_Test::contructor()
{
	QCOMPARE(cam1.u(), u);
	QCOMPARE(cam1.v(), v);
	QCOMPARE(cam1.w(), w);
	QCOMPARE(cam1.width(), float(2));	
	QCOMPARE(cam1.height(), float(2));	
}


void Camera_Test::createRay()
{
	Ray r0 = cam2.createRay(0,0);	
	Ray r1 = cam2.createRay(1,0);	
	Ray r2 = cam2.createRay(1,1);	
	Ray r3 = cam2.createRay(0,1);	

	QVERIFY(r0.direction == Vector(-1,-0.5F,-0.5F));
	QVERIFY(r1.direction == Vector(-1, 0.5F,-0.5F));
	QVERIFY(r2.direction == Vector(-1,0.5F,0.5F));
	QVERIFY(r3.direction == Vector(-1,-0.5F,0.5F));
}
