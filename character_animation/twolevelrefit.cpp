#include <cal3dconversions.h>
#include <cluster.h>
#include <fuzzybvh.h>
#include <stats.h>
#include <twolevelrefit.h>
#include <twolevelraytracer.h>

using namespace rt;

TwoLevelRefit::TwoLevelRefit(CalModel* model, 
			  				 const Camera& camera, 
							 int nbFrames, 
							 bool stats)
: Fuzzy(model, camera, nbFrames, stats)
{
}

void TwoLevelRefit::initRendering()
{
	adapter_->fillClusterWithRestPose();
    for (int i = 0; i < nbClusters(); ++i) {
        int id = clusterIDs_[i];
        trees_[i] = new FuzzyBVH(clusters_[id].triangles(),
                                  clusters_[id].fuzzyBoxes(),
                                  513);
    }
	TwoLevelRayTracer* rt = static_cast<TwoLevelRayTracer*>(raytracer_);
	rt->setTrees(const_cast<const FuzzyBVH**>(trees_));
	rt->setNbClusters(nbClusters_);
}

void TwoLevelRefit::updateState()
{
	// rebuild the trees
	adapter_->fillClusterWithResidualPose();
	for (int t = 0; t < nbClusters_; ++t) 
		trees_[t]->rebuild();

	// update the state
	adapter_->fillClusterWithCurrentPose();
	updateWorld2Locals();
	updateBoundingBoxes();

	// set new state in trees
	TwoLevelRayTracer* rt = static_cast<TwoLevelRayTracer*>(raytracer_);
	rt->setBoundingBoxes(boundingBoxes_);
}
