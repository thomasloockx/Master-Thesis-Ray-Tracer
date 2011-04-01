#ifndef RT_FAST_REBUILD_H
#define RT_FAST_REBUILD_H

#include <testcase.h>

namespace rt {

class SimpleBVH;

class FastRebuild : public TestCase
{
public:
    FastRebuild(CalModel *model, 
			    const Camera& camera, 
				int nbFrames, 
				bool stats);
    ~FastRebuild();

protected:
	void initRendering();
	void updateState();
	void render(Image& img, Stats& outStats);
	void finalizeRendering();
	
private:
	std::vector<Triangle> triangles_;
	SimpleBVH* tree_;

#ifdef RT_PARTIAL_REBUILD
	int frameNb_;
	bool rebuild_;
#endif
};

} // namespace rt
#endif
