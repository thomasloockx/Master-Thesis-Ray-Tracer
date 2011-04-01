#include <cal3dconversions.h>
#include <cluster.h>
#include <fuzzy.h>
#include <packetraytracer.h>
#include <fuzzybvh.h>    
#include <simplebvh.h>
#include <stats.h>
#include <triangle.h>
#include <twolevelraytracer.h>

using namespace rt;

Fuzzy::Fuzzy(CalModel* model, 
             const Camera& camera, 
             int nbFrames, 
			 bool stats,
             MOTION_TYPE type)
: TestCase(model, camera, nbFrames, stats), 
  type_(type),
  nbBones_(adapter_->nbBones()),
  nbClusters_(adapter_->filledClusterCount()),
  clusters_(adapter_->clusters()),
  trees_(new FuzzyBVH*[adapter_->filledClusterCount()]),
  boundingBoxes_(new BoundingBox[adapter_->filledClusterCount()])
{
    raytracer_ = new TwoLevelRayTracer(camera, nbRaysPerPacket());

    for (int i = 0; i < nbBones(); ++i) {
        if (!clusters_[i].empty())
            clusterIDs_.push_back(i);
    }
    assert(nbClusters() == int(clusterIDs_.size()));
}

Fuzzy::~Fuzzy()
{
    delete raytracer_;

    for (int t = 0; t < nbClusters(); ++t)
        delete trees_[t];
	delete[] trees_;
	delete[] boundingBoxes_;
}

void Fuzzy::initRendering()
{
	adapter_->fillClusterWithResidualPose();

    for (int i = 0; i < nbClusters(); ++i) {
        int id = clusterIDs_[i];
        trees_[i] = new FuzzyBVH(clusters_[id].triangles(),
                                  clusters_[id].fuzzyBoxes(),
                                  513);
    }

	TwoLevelRayTracer* rt = static_cast<TwoLevelRayTracer*>(raytracer_);
	rt->setTrees(const_cast<const FuzzyBVH**>(trees_));
	rt->setNbClusters(nbClusters());
}

void Fuzzy::updateState()
{
	adapter_->fillClusterWithCurrentPose();
	updateWorld2Locals();
	updateBoundingBoxes();
	TwoLevelRayTracer* rt = static_cast<TwoLevelRayTracer*>(raytracer_);
	rt->setBoundingBoxes(boundingBoxes_);
}

void Fuzzy::render(Image& img, Stats& outStats)
{
	TwoLevelRayTracer* rt = static_cast<TwoLevelRayTracer*>(raytracer_);
	if (stats())
		rt->render(img, outStats);
	else 
		rt->render(img);
}

void rt::Fuzzy::updateWorld2Locals()
{
    for (int i = 0; i < nbClusters(); ++i) {
        int id = clusterIDs_[i];
        trees_[i]->setTransformation(clusters_[id].world2local());
    }
}

void rt::Fuzzy::updateBoundingBoxes()
{
    for (int i = 0; i < nbClusters(); ++i) {
        int id = clusterIDs_[i];
        boundingBoxes_[i] = clusters_[id].boundingBox();
    }
}

void Fuzzy::startResidual()
{
    std::cout << "Starting Fuzzy: Residual" << std::endl;

	PacketRayTracer rt(raytracer_->camera(), nbRaysPerPacket());

    for (int frame = 0; frame < nbFrames(); ++frame) {
        adapter_->update(0.04);
		std::vector<Triangle> triangles;
		adapter_->residual(triangles);

        Image img(nbHorizontalPixels(), nbVerticalPixels());
        SimpleBVH tree(triangles);
		rt.render(&tree, img);

        saveImg(img, frame);
    }
}

void Fuzzy::startFuzzyBoxes() 
{
    // TODO fix this
    std::cout << "Starting Fuzzy: Fuzzy Boxes" << std::endl;

	const Cluster* clusters = adapter_->clusters();
	std::vector<BoundingBox> fuzzyBoxes;
	for (int b = 0; b < nbBones_; ++b) {								
		if (!clusters[b].empty()) {
			fuzzyBoxes.push_back(clusters[b].fuzzyBox());
		}
	}

	//Raytracer rt(raytracer_->camera());
	for (int frame = 0; frame < nbFrames(); ++frame) {

        Image img(nbHorizontalPixels(), nbVerticalPixels());
	//	rt.render(fuzzyBoxes, img);
        saveImg(img, frame);
    }
}
