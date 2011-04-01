#ifndef RT_TESTCASE_H
#define RT_TESTCASE_H

#include <cal3dadapter.h>
#include <image.h>
#include <raytracerbase.h>
#include <simpletimer.h>
#include <utils.h>

#include <cal3d.h>

#include <vector>
#include <string>

#include <boost/timer.hpp>

namespace rt {

struct Stats;
class Camera;

class TestCase
{
public:
    TestCase(CalModel *model, const Camera& camera, int nbFrames, bool stats);
    virtual ~TestCase();

	/* Template method. */
	virtual void run();

    int nbFrames() const { return nbFrames_; }
    float fps() const { return fps_; }
    float avgRenderTime() const { return avgRenderTime_; }
	float avgUpdateTime() const { return avgUpdateTime_; }
	bool stats() const { return stats_; }
    static float avg(const std::vector<float>& v);
	int nbRaysPerPacket() const { return 64; }

protected:
    Camera camera_;

    std::vector<float>* renderTimes_;
    std::vector<float>* frameTimes_;
	std::vector<float>* updateTimes_;

    Cal3dAdapter* adapter_;
    RayTracerBase* raytracer_;
    
    void calcAvgRenderTime();
    void calcFps();
	void calcAvgUpdateTime();

	virtual void printHeader() const;
	virtual void initRendering() = 0;
	virtual void updateState() = 0;
	virtual void render(Image& img, Stats& outStats) = 0;
	virtual void finalizeRendering();
	virtual void printStats(int frame, const Stats& stats) const;
	
	int nbHorizontalPixels() const 
	{ 
		return camera().nbHorizontalPixels();
	}
	int nbVerticalPixels() const 
	{ 
		return camera().nbVerticalPixels();
	}

    const Camera& camera() const
    {
        return camera_;
    }

private:
    int nbFrames_;
    float fps_;
	bool stats_;
    float avgRenderTime_;
	float avgUpdateTime_;

	/* Stuff for the fast rendering stats. */
	static void printFastTableHeader();
	static void printFastTableLine(
					const std::string& algorithm,
					int nbFrames,
					int nbTriangles,
					int nbBones,
					float renderTime,
					float buildTime,
					float fps,
					float totalTime);

	/* Stuff for the statistics table. */
	static void printTableHeader();
	static void printTableLine(
					int frameNb,
					int nbTriangles,
					int nbNodes,
					int nbRaysPacket,
					int nbHits,
					int nbBBoxTests,
					int nbTriangleTests,
					float renderTime,
					float updateTime,
					float frameTime);

	static int columnWidth() { return 12; }	

    DISALLOW_COPY_AND_ASSIGN(TestCase);
};

/******************************************************************************
 * HELPER FUNCTIONS
 ******************************************************************************/

inline
std::string createFrameName(const std::string& name, int frameNb)
{
	std::stringstream ss;

	ss << name;
	if (frameNb >= 0 && frameNb < 10) 
		ss << "00" << frameNb;	
	else if (frameNb >= 10 && frameNb < 100)
		ss << "0" << frameNb;
	else 
		ss << frameNb;

	return ss.str();
}

inline
void saveImg(const Image& img, int frameNb)
{
	img.save(createFrameName("render", frameNb) + ".png");
}

} // namespace rt

#endif
