#include <testcase.h>
#include <simpletimer.h>
#include <stats.h>

#include <iostream>
#include <iomanip>

using namespace rt;

TestCase::TestCase(CalModel *model, 
				   const Camera& camera, 
				   int nbFrames,
				   bool stats)
: camera_(camera),
  renderTimes_( new std::vector<float>(nbFrames) ),
  frameTimes_( new std::vector<float>(nbFrames) ),
  updateTimes_( new std::vector<float>(nbFrames) ),
  adapter_( new Cal3dAdapter(model) ),
  nbFrames_(nbFrames),
  fps_(0),
  stats_(stats),
  avgRenderTime_(-1),
  avgUpdateTime_(-1)
{
}

TestCase::~TestCase()
{
    delete renderTimes_;
    delete frameTimes_;
	delete updateTimes_;
    delete adapter_;
}

void TestCase::run() 
{
	SimpleTimer totalTimer;

	printHeader();
	initRendering();

    for (int frame = 0; frame < nbFrames(); frame++) {

		SimpleTimer frameTimer;
		adapter_->update(0.04);

		SimpleTimer updateTimer;
		updateState();
		(*updateTimes_)[frame] = updateTimer.elapsed();

		Image img(nbHorizontalPixels(),nbVerticalPixels());
		Stats stat;

		SimpleTimer renderTimer;
		render(img, stat);
		(*renderTimes_)[frame] = renderTimer.elapsed();
		(*frameTimes_)[frame] = frameTimer.elapsed();

		printStats(frame, stat);
		saveImg(img, frame);

		finalizeRendering();
	} // end frame loop

	if (!stats()) {
		calcAvgUpdateTime();
		calcFps();
		calcAvgRenderTime();
		printFastTableLine(
			"",
			nbFrames(),
			adapter_->nbTriangles(),
			adapter_->nbBones(),
			avgRenderTime(),
			avgUpdateTime(),
			fps(),
			totalTimer.elapsed());
	}
}

void TestCase::calcAvgRenderTime()
{
    avgRenderTime_ = avg(*renderTimes_);
}

void TestCase::calcFps()
{
    fps_ = 1 / avg(*frameTimes_);
}

void TestCase::calcAvgUpdateTime()
{
	avgUpdateTime_ = avg(*updateTimes_);
}

void TestCase::printHeader() const
{
	if (stats())
		printTableHeader();
	else
		printFastTableHeader();
}

float TestCase::avg(const std::vector<float>& v)
{
    float avg = 0;
    for (std::size_t i = 0; i < v.size(); ++i)
        avg += v[i]; 
    return avg / v.size();
}

void TestCase::printStats(int frame, const Stats& stat) const
{
	if (stats()) {

		assert(stat.nbHits_ > 0);

		printTableLine(frame,
		stat.nbTriangles_,
		stat.nbNodes_,
		nbRaysPerPacket(),
		stat.nbHits_,
		stat.nbBoundingBoxTests_,
		stat.nbTriangleTests_,
		renderTimes_->at(frame),
		updateTimes_->at(frame),
		frameTimes_->at(frame));
	}
}


void TestCase::finalizeRendering()
{
	// override to do cleanup stuff
}
void TestCase::printFastTableHeader()
{
	std::cout << std::setw(1) << std::left << "# ";
	std::cout << std::setw(columnWidth()) << std::left << "Algorithm";
	std::cout << std::setw(columnWidth()) << std::left << "NbFrames";
	std::cout << std::setw(columnWidth()) << std::left << "NbTris";
	std::cout << std::setw(columnWidth()) << std::left << "NbBones";
	std::cout << std::setw(columnWidth()) << std::left << "RenderTime";
	std::cout << std::setw(columnWidth()) << std::left << "BuildTime"; 
	std::cout << std::setw(columnWidth()) << std::left << "FPS";
	std::cout << std::setw(columnWidth()) << std::left << "TotalTime";
	std::cout << std::endl;
}

void TestCase::printFastTableLine(
					const std::string& algorithm,
					int nbFrames,
					int nbTriangles,
					int nbBones,
					float renderTime,
					float buildTime,
					float fps,
					float totalTime)
{
	std::cout << std::setw(1) << std::left << "  ";
	std::cout << std::setw(columnWidth()) << std::left << algorithm;
	std::cout << std::setw(columnWidth()) << std::left << nbFrames;
	std::cout << std::setw(columnWidth()) << std::left << nbTriangles;
	std::cout << std::setw(columnWidth()) << std::left << nbBones;
	std::cout << std::setw(columnWidth()) << std::left << renderTime;
	std::cout << std::setw(columnWidth()) << std::left << buildTime;
	std::cout << std::setw(columnWidth()) << std::left << fps;
	std::cout << std::setw(columnWidth()) << std::left << totalTime;
	std::cout << std::endl;
}


void TestCase::printTableHeader()
{
	std::cout << std::setw(1) << std::left << "#";
	std::cout << std::setw(columnWidth()) << std::left << "Frame";
	std::cout << std::setw(columnWidth()) << std::left << "NbTris";
	std::cout << std::setw(columnWidth()) << std::left << "NbNodes";
	std::cout << std::setw(columnWidth()) << std::left << "PacketSize";
	std::cout << std::setw(columnWidth()) << std::left << "NbHits"; 
	std::cout << std::setw(columnWidth()) << std::left << "BBox/Hit";
	std::cout << std::setw(columnWidth()) << std::left << "Tri/Hit";
	std::cout << std::setw(columnWidth()) << std::left << "RenderTime";
	std::cout << std::setw(columnWidth()) << std::left << "UpdateTime";
	std::cout << std::setw(columnWidth()) << std::left << "FrameTime";
	std::cout << std::setw(columnWidth()) << std::left << "update/render";
	std::cout << std::endl;
}

void TestCase::printTableLine(
						int frameNb,
						int nbTriangles,
						int nbNodes,
						int nbRaysPacket,
						int nbHits,
						int nbBBoxTests,
						int nbTriangleTests,
						float renderTime,
						float buildTime,
						float frameTime)
{
	std::cout << std::setw(1) << std::left << " ";
	std::cout << std::setw(columnWidth()) << std::left << frameNb;
	std::cout << std::setw(columnWidth()) << std::left << nbTriangles ;
	std::cout << std::setw(columnWidth()) << std::left << nbNodes;
	std::cout << std::setw(columnWidth()) << std::left << nbRaysPacket;
	std::cout << std::setw(columnWidth()) << std::left << nbHits;
	std::cout << std::setw(columnWidth()) << std::left << nbBBoxTests;
	std::cout << std::setw(columnWidth()) << std::left << nbTriangleTests;
	std::cout << std::setw(columnWidth()) << std::left << renderTime;
	std::cout << std::setw(columnWidth()) << std::left << buildTime;
	std::cout << std::setw(columnWidth()) << std::left << frameTime;
	std::cout << std::setw(columnWidth()) << std::left << buildTime/renderTime;
	std::cout << std::endl;
}
