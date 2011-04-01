#ifndef RT_FUZZY_H
#define RT_FUZZY_H

#include <testcase.h>

namespace rt {

class FuzzyBVH;
class Matrix4;

typedef enum 
{
    NORMAL,
    RESIDUAL,
    AFFINE,
	FUZZY_BOXES
} MOTION_TYPE;

class Fuzzy : public TestCase
{
public:
    Fuzzy(CalModel* model, 
          const Camera& camera, 
          int nbFrames, 
		  bool stats,
          MOTION_TYPE type = NORMAL); 
	~Fuzzy();

    MOTION_TYPE motionType() const { return type_; }
    int nbClusters() const { return nbClusters_; }
    int nbBones() const { return nbBones_; }

    void startResidual();

protected:
    MOTION_TYPE type_;
	int nbBones_;

	const int nbClusters_;
	const Cluster* clusters_;
    std::vector<int> clusterIDs_;

	FuzzyBVH** trees_;
	BoundingBox* boundingBoxes_;

	void updateWorld2Locals();
	void updateBoundingBoxes();

	void initRendering();
	void updateState();
	void render(Image& img, Stats& outStats);

private:
    void startNormal();
	void startFuzzyBoxes();
};

} // namespace rt

#endif
