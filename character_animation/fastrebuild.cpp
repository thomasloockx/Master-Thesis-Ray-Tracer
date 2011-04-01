#include <fastrebuild.h>
#include <simplebvh.h>
#include <stats.h>
#include <packetraytracer.h>

#include <iostream>
#include <iomanip>

using namespace rt;

FastRebuild::FastRebuild(CalModel *model, 
						 const Camera& camera, 
						 int nbFrames, 
						 bool stats)
: TestCase(model, camera, nbFrames, stats),
  tree_(NULL)
{
    raytracer_ = new PacketRayTracer(camera, nbRaysPerPacket());

#ifdef RT_PARTIAL_REBUILD
	frameNb_ = 0;
	rebuild_ = true;
#endif 
}

FastRebuild::~FastRebuild()
{
    delete raytracer_;
}

void FastRebuild::initRendering()
{
}

void FastRebuild::updateState()
{
	adapter_->current(triangles_);
	// uncomment to render rest/residual post
	//adapter_->residual(triangles_);
	//adapter_->rest(triangles_);
	
#ifdef RT_PARTIAL_REBUILD
	if (frameNb_ == 13 || frameNb_ == 17)
		rebuild_ = true;

	if (rebuild_) {
		tree_ = new SimpleBVH(triangles_, 513);
		rebuild_ = false;
	}
	else {
		tree_->rebuild();		
	}
	frameNb_++;
#else
	tree_ = new SimpleBVH(triangles_, 16);
#endif
}

void FastRebuild::render(Image& img, Stats& outStats)
{
	PacketRayTracer* rt = static_cast<PacketRayTracer*>(raytracer_);
	if (stats()) 
		rt->render(tree_, img, outStats);
	else 
		rt->render(tree_, img);
}

void FastRebuild::finalizeRendering()
{
#ifdef RT_PARTIAL_REBUILD
	if (rebuild_)
		delete tree_;
#else
	delete tree_;
#endif
}
