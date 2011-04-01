#include <raypacket.h>
#include <boundingbox.h>

#include <algorithm>
#include <cmath>
#include <map>

rt::RayPacket::RayPacket(int nbRays)
: clusterHitIndices(std::set<int>()),
  begin(0),
  nbRays_(nbRays),
  sqrtNbRays_(sqrt(nbRays)),
  rays_(new Ray[nbRays]),
  hitRecords_(new HitRecord[nbRays]),
  hits_(new bool[nbRays])
{
	for (int i = 0; i < nbRays; ++i)
		hits_[i] = false;	
}

rt::RayPacket::~RayPacket()
{
	delete[] rays_;
	delete[] hitRecords_;
	delete[] hits_;
}

void rt::RayPacket::calcIAData()
{
    // optimize for common origin
    // reset the data
    for (int axis = 0; axis < 3; ++axis) {
        iaData.origin[axis]    = Interval();
        iaData.direction[axis] = Interval();
    }

    for (int r = begin; r < nbRays(); ++r) {
        for (int axis = 0; axis < 3; ++axis) {
            float org = ray(r).origin[axis];
            float dir = ray(r).direction[axis];

            iaData.origin[axis].setLeft(std::min(org, iaData.origin[axis].left()));
            iaData.origin[axis].setRight(std::max(org, iaData.origin[axis].right()));

            iaData.direction[axis].setLeft(std::min(dir, iaData.direction[axis].left()));
            iaData.direction[axis].setRight(std::max(dir, iaData.direction[axis].right()));
        }
    }
}
