#include <packetraytracer.h>
#include <refit.h>
#include <simplebvh.h>
#include <simpletimer.h>
#include <stats.h>
#include <triangle.h>

#include <sys/time.h>
#include <unistd.h>

using namespace rt;

Refit::Refit(CalModel* model, const Camera& camera,  int nbFrames, bool stats)
: TestCase(model, camera, nbFrames, stats)
{
  raytracer_ = new PacketRayTracer(camera, nbRaysPerPacket());
}

Refit::~Refit()
{
    delete raytracer_;
    delete tree_;
}

void Refit::initRendering()
{
#if 0 
    adapter_->current(triangles_);
#else
	adapter_->rest(triangles_);
#endif

    tree_ = new SimpleBVH(triangles_, 513);
}

void Refit::updateState()
{
	adapter_->current(triangles_);
	tree_->rebuild();
}

void Refit::render(Image& img, Stats& outStats)
{
	PacketRayTracer* rt = static_cast<PacketRayTracer*>(raytracer_);
	if (stats()) 
		rt->render(tree_, img, outStats);
	else 
		rt->render(tree_, img);
}
