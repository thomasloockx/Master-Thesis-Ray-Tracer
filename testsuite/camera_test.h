#ifndef CAMERA_TEST_H
#define CAMERA_TEST_H

#include <camera.h>
#include <point.h>
#include <vector.h>

#include <QtTest/QtTest>

using namespace rt;

class Camera_Test : public QObject
{
	Q_OBJECT

	Camera cam1;
	Camera cam2;
	Point eye;
	Vector gaze;
	Vector up;
	Vector u;
	Vector v;
	Vector w;
	float hfovy;
	float vfovy;
	float distance;
	int nx;
	int ny;
	
private slots:
	void init();
	void contructor();	
	void createRay();

};

#endif
