#ifndef RT_TWOLEVELRAYTRACER_H
#define RT_TWOLEVELRAYTRACER_H

#include <packetraytracer.h>

namespace rt {

class FuzzyBVH;
struct Stats;

class TwoLevelRayTracer : public PacketRayTracer
{
public:
	TwoLevelRayTracer(const Camera& camera, int packetSize);

	void render(Image& img) const;
	void render(Image& img, Stats& stats) const;

	void setNbClusters(int nbClusters);
	void setTrees(const FuzzyBVH** trees);
	void setBoundingBoxes(const BoundingBox* boundingBoxes);
	int nbClusters() const { return nbClusters_; }
	const FuzzyBVH** trees() const { return trees_; }
	const BoundingBox* boundingBoxes() const { return boundingBoxes_; }

private:
	int nbClusters_;
	const FuzzyBVH** trees_;
	const BoundingBox* boundingBoxes_;

    struct ClusterCmp {

        ClusterCmp(Point o, const BoundingBox* bxs)
        : orig(o), boxes(bxs)
        {
        }

        bool operator()(int i, int j) const
        {
            float disti = (boxes[i].center() - orig).norm();
            float distj = (boxes[j].center() - orig).norm();
            return disti < distj;
        }

        Point orig;
        const BoundingBox* boxes;
    };
};

} // namespace rt

#endif
