#include <cluster.h>

rt::Cluster::Cluster(int boneId)
: boneId_(boneId)
{
}

void rt::Cluster::addTriangle(const Triangle& triangle)
{
	triangles_.push_back(triangle);
	boundingBox_ += triangle.boundingBox();
}

void rt::Cluster::addFuzzyBox(const BoundingBox& fuzzyBox)
{
	fuzzyBoxes_.push_back(fuzzyBox);
	fuzzyBox_ += fuzzyBox;
}

void rt::Cluster::clearTriangles()
{
	triangles_.clear();
	boundingBox_ = BoundingBox();
}

std::ostream& rt::operator<<(std::ostream& o, const Cluster& c)
{
	o << "Cluster::(";
	o << "boneId = " << c.boneId() << ", ";
	o << "#triangles = " << c.nbTriangles() << ", ";
	o << "#fuzzyBoxes = " << c.nbFuzzyBoxes() << ")";
	return o;
}
