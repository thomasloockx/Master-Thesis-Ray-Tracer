#ifndef RT_CAL3DADAPTER_H
#define RT_CAL3DADAPTER_H

#include <matrix.h>
#include <vector.h>

#include <cal3d.h>

#include <vector>

namespace rt {

typedef CalCoreSubmesh::Vertex Vertex;
typedef CalCoreSubmesh::Face Face;
typedef CalCoreSubmesh::Influence Influence;

class BoundingBox;
class Color;
class Matrix4;
class Triangle;
class Cluster;

class Cal3dAdapter
{
public:
	Cal3dAdapter(CalModel* model);	
	~Cal3dAdapter();	

	int nbBones() const;
	int nbMeshes() const;
	int nbSubmeshes(int meshId) const;
	int totalNbSubmeshes() const;
	int nbTriangles() const;

	const Cluster* clusters() const;
	const int * clusterOf() const;
	int filledClusterCount() const;

	void update(float elapsedTime);

	// hack
	CalBone* bone(int boneId) const;
	CalBone* bone(int boneId);

	void current(std::vector<Triangle>& triangles) const;
	void rest(std::vector<Triangle>& triangles) const;
	void residual(std::vector<Triangle>& triangles) const;

	void fillClusterWithRestPose();
	void fillClusterWithCurrentPose();
	void fillClusterWithResidualPose();

private:
	static const int NB_SAMPLES = 10;

	CalModel* model_;

	/* Cluster per bone of the skeleton. */
	Cluster* clusters_;
	/* Color for each bone. */
	Color* colors_;
	/* Cluster of each triangle. */
	int* clusterOf_;
	/* Bone Transformations. */
	Matrix4* boneTransforms_;
	Matrix4* invBoneTransforms_;

	/* duplicate info from Cal3d copied to make life easier. */
	std::vector<Point>& restVertices_;
	std::vector<Point>& currentVertices_;
	std::vector<Point>& residualVertices_;
	std::vector< std::vector<Influence> >& influences_;
	std::vector<Face>& faces_;

	CalSkeleton* skeleton() const;
	CalSkeleton* skeleton();
	CalCoreModel* coreModel() const;
	CalCoreModel* coreModel();
	CalCoreSubmesh* coreSubmesh(int meshId, int submeshId) const;
	CalCoreSubmesh* coreSubmesh(int meshId, int submeshId);

	void initColors();
	void initialize();
    void skinVertices();
	void skinResidual();
	void updateTransformations();
	void fillCluster(const std::vector<Point>& vertices);
	void fillVector(const std::vector<Point>& vertices,
					std::vector<Triangle>& triangles) const;

	void calcFuzzyBoxes();
	#if 0
	int calcBone(const std::vector<Influence> i0, 
				 const std::vector<Influence> i1, 
				 const std::vector<Influence> i2) const;
	#endif

	// prevent evil
	Cal3dAdapter(const Cal3dAdapter&);
	Cal3dAdapter& operator=(const Cal3dAdapter&);
};

} // namespace rt

#endif
