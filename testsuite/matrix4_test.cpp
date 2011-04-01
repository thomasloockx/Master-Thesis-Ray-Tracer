#include <matrix4_test.h>
#include <cal3dconversions.h>
#include <cluster.h>

void Matrix4_Test::initTestCase()
{
	parser = new CFGParser("skeleton", 
						   "../character_animation/characters/skeleton/",
						   "skeleton.cfg");
	model = parser->model();
	adapter = new Cal3dAdapter(model);

}

void Matrix4_Test::matrix4inverse()
{
	int boneId = 3;
	const Cluster* cluster = adapter->clusters();
	Matrix4 m = cluster[boneId].world2local();
	Matrix4 mInv = m.inverse();

	Matrix rotInv(0.99910, -0.00541, -0.04197,
	  		      0.00960,  0.99491,  0.10028, 
		          0.04122, -0.10060,  0.99407);
	Vector transInv(-5.36200, 8.39254, 0.88334);

	QVERIFY(mInv.rotation() == rotInv);
	QVERIFY(mInv.translation() == transInv);
}

void Matrix4_Test::matrix4mul()
{
	int boneId = 3;
	const Cluster* cluster = adapter->clusters();
	Matrix4 mat = cluster[boneId].world2local();

	Point p(1,2,3);
	Vector v(1,2,3);

	#if 0
	// matrix point mul
	Point p1 = mat * p;
	Point p2 = mat.rot * p + trans;
	QVERIFY(p1 == p2);

	// matrix vector mul
	Vector v1 = mat * v;
	Vector v2 = rot * v;
	QVERIFY(v1 == v2);

	// matrix inv mul
	p1 = mat.inverse() * p;
	p2 = rot.inverse() * (p + (-1 * trans));
	QVERIFY(p1 == p2);

	v1 = mat.inverse() * v;
	v2 = rot.inverse() * v;
	QVERIFY(v1 == v2);

	// check Cal3d matrices
	Point cp1 = rot * p;
	CalVector cp2 = toCalVector(p);
	cp2 *= adapter->bone(boneId)->getTransformMatrix();
	QVERIFY(cp1 == toPoint(cp2));
	#endif
}

void Matrix4_Test::cleanupTestCase()
{
	delete parser;
	delete model;
	delete adapter;

}
