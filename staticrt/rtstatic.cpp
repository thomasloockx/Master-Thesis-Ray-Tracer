#include <camera.h>
#include <image.h>
#include <packetraytracer.h>
#include <ra2parser.h>
#include <simplebvh.h>
#include <simpletimer.h>
#include <stats.h>

#include <iostream>

#include <boost/timer.hpp>

using namespace rt;

/*
 * Ray Tracer for static scenes in the .ra2 format.
 */
int main(int argc, char **argv)
{
	// STATIC SCENES //
	boost::timer ttiTimer;

 	std::string ra2File(argv[1]); 
	std::string camFile(argv[2]);
    std::string imgFile(argv[3]);

	Ra2Parser parser(ra2File, camFile);

	Camera camera = parser.camera();
	std::vector<Triangle>& triangles = parser.triangles();

	SimpleTimer buildTimer;
	SimpleBVH tree(triangles);
	double buildTime = buildTimer.elapsed();

    PacketRayTracer raytracer(camera, 64);
	Image img(camera.nbHorizontalPixels(), camera.nbVerticalPixels());

	SimpleTimer renderTimer;
	//Stats stats;
	//raytracer.render(tree, img);
	raytracer.render(&tree, img);
	double renderTime = renderTimer.elapsed();

	img.save(imgFile);

	double timeToImg = ttiTimer.elapsed();
	std::cout << "#triangles = " << triangles.size() << std::endl;
	std::cout << "build time = " << buildTime << std::endl;
	std::cout << "render time = " << renderTime << "" << std::endl;
	std::cout << "time to image = " << timeToImg << "" << std::endl;
	return 0;
}
