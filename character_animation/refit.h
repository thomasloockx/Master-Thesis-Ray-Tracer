#ifndef RT_REFIT_H
#define RT_REFIT_H

#include <testcase.h>

namespace rt {

class SimpleBVH;

class Refit : public TestCase
{
public:
    Refit(CalModel* model, const Camera& camera,  int nbFrames, bool stats);
    ~Refit();

protected:
	void initRendering();
	void updateState();
	void render(Image& img, Stats& outStats);

private:
	std::vector<Triangle> triangles_;
	SimpleBVH* tree_;
};

} // namespace rt

#endif
