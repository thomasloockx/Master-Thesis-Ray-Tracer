#ifndef RT_STATS_H
#define RT_STATS_H

#include <iostream>

namespace rt {

/*
 * Data Structure keeping statistics about the rendering process.
 *
 * @author	Thomas Loockx
 * @date	18/12/2008
 */
struct Stats
{
	Stats() : 
	nbNodes_(0), nbTriangles_(0), nbHits_(0), nbBoundingBoxTests_(0), 
	nbTriangleTests_(0), nbRaysPacket_(0), nbTopLevelBBoxTests_(0),
	nbLocalBBoxTests_(0), topLevelTime_(0), localLevelTime_(0),
	nbClusters_(0), in2(false)
	{
	}

	long long nbNodes_; 
	long long nbTriangles_;
	long long nbHits_; 
	long long nbBoundingBoxTests_;
	long long nbTriangleTests_;
	int nbRaysPacket_;

	/* Extra stuff for 2 level rt */
	long long nbTopLevelBBoxTests_;
	long long nbLocalBBoxTests_;
	double topLevelTime_;
	double localLevelTime_;
	int nbClusters_;
	double FuzzyBoxArea_;

    // debugging stuff
    bool in2;
};

std::ostream& operator<<(std::ostream& o, const Stats& s);

} // namespace rt

inline
std::ostream& rt::operator<<(std::ostream& o, const rt::Stats& s) {
	o << "** Statistics **" << "\n";
	o << "#nodes = " << s.nbNodes_ << "\n";
	o << "#triangles = " << s.nbTriangles_ << "\n";
	o << "#hit = " << s.nbHits_ << "\n";
	o << "#BoundingBox tests = " << s.nbBoundingBoxTests_ << "\n";
	o << "#Triangle tests = " << s.nbTriangleTests_ << "\n";
	o << "****************";
	return o;
}

inline
void printExtendedStats(const rt::Stats& s, int nbRays)
{
	std::cout << "** Statistics **" << "\n";
	std::cout << "#nodes = " << s.nbNodes_ << "\n";
	std::cout << "#triangles = " << s.nbTriangles_ << "\n";
	std::cout << "#hit = " << s.nbHits_ << "\n";
	std::cout << "#BoundingBox tests = " << s.nbBoundingBoxTests_ << "\n";
	std::cout << "#Triangle tests = " << s.nbTriangleTests_ << "\n";
	std::cout << "BoundingBox Test / Triangle Test ratio = " << s.nbBoundingBoxTests_ / s.nbTriangleTests_ << "\n";
	std::cout << "#rays = " << nbRays << "\n";
	std::cout << "#Triangle tests / #rays = " << s.nbTriangleTests_ / nbRays << "\n";
	std::cout << "#BoundingBox tests / #rays = " << s.nbBoundingBoxTests_ / nbRays << "\n";
	std::cout << "****************" << std::endl;
}

#endif
