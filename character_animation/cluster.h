#ifndef RT_CLUSTER_H
#define RT_CLUSTER_H

#include <boundingbox.h>
#include <matrix4.h>
#include <triangle.h>

#include <iostream>

namespace rt {

class Cluster 
{
public:
	Cluster(int boneId = -1);
	
	int boneId() const { return boneId_; }
	void setBoneId(int id) { boneId_ = id; }

	void addTriangleId(int id) { triangleIds_.push_back(id); }
	const std::vector<int>& triangleIds() const { return triangleIds_; }	

	void addTriangle(const Triangle& triangle);
	const std::vector<Triangle>& triangles() const { return triangles_; }

	void addFuzzyBox(const BoundingBox& fuzzyBox);
	const std::vector<BoundingBox>& fuzzyBoxes() const { return fuzzyBoxes_; }

	int nbTriangles() const { return triangles_.size(); }
	int nbFuzzyBoxes() const { return fuzzyBoxes_.size(); }
	bool empty() const { return nbTriangles() == 0; }

	const BoundingBox& boundingBox() const { return boundingBox_; }
	const BoundingBox& fuzzyBox() const { return fuzzyBox_; }
	void clearTriangles();

	Matrix4 world2local() const { return world2local_; }
	Matrix4 local2world() const { return local2world_; }
	void setWorld2Local(const Matrix4& m) { world2local_ = m; }
	void setLocal2World(const Matrix4& m) { local2world_ = m; }

private:
	int boneId_;
	std::vector<int> triangleIds_;
	std::vector<Triangle> triangles_;
	std::vector<BoundingBox> fuzzyBoxes_;

	BoundingBox boundingBox_;
	BoundingBox fuzzyBox_;

	Matrix4 world2local_;
	Matrix4 local2world_;

	// prevent evil
	Cluster(const Cluster&);	
	Cluster& operator=(const Cluster&);
};

std::ostream& operator<<(std::ostream& o, const Cluster& c);

} // namespace rt

#endif
