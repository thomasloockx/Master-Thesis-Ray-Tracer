#include <bboxbvh.h>
#include <boundingbox.h>
#include <image.h>
#include <fuzzybvh.h>
#include <simpletimer.h>
#include <stats.h>
#include <twolevelraytracer.h>

#include <algorithm>
#include <set>

rt::TwoLevelRayTracer::TwoLevelRayTracer(const Camera& camera, int packetSize)
: PacketRayTracer(camera, packetSize)
{
}

void rt::TwoLevelRayTracer::render(Image& img) const
{
	BBoxBVH topLevelTree(boundingBoxes(), nbClusters());

    RayPacket packet(packetSize());
	RayPacket transformed(packetSize());
    ClusterCmp cmp(camera().origin(), boundingBoxes_);

    FOREACH_PACKET(i,j) {
        camera().createPacket(i, j, packet);

		if (topLevelTree.traverse(packet)) {
            // hack get the set sorted
            std::vector<int> vhits;
            vhits.insert(vhits.end(), 
                         packet.clusterHitIndices.begin(), 
                         packet.clusterHitIndices.end());
            packet.clusterHitIndices.clear();
            std::sort(vhits.begin(), vhits.end(), cmp);
         
			bool hit = false;
			std::vector<int>::const_iterator idx;
			for (idx = vhits.begin(); idx != vhits.end(); ++idx) {
				if (trees()[*idx]->traverse(packet))
                    hit = true;
            }

			if (hit) {
				processHit(i, j, packet, img);
				hit = false;
			}
		}
    } 
}

void rt::TwoLevelRayTracer::render(Image& img, Stats& stats) const
{
	BBoxBVH topLevelTree(boundingBoxes(), nbClusters());

    RayPacket packet(packetSize());
	stats.nbRaysPacket_ = packetSize();
    ClusterCmp cmp(camera().origin(), boundingBoxes_);

    FOREACH_PACKET(i,j) {
        camera().createPacket(i, j, packet);

		SimpleTimer topLevelTimer;
		bool descent = topLevelTree.traverse(packet, stats);
		stats.topLevelTime_ += topLevelTimer.elapsed();

		SimpleTimer localLevelTimer;

		if (descent) {          
            // hack to get the set sorted
            std::vector<int> vhits;
            vhits.insert(vhits.end(), 
                         packet.clusterHitIndices.begin(), 
                         packet.clusterHitIndices.end());
            packet.clusterHitIndices.clear();
            std::sort(vhits.begin(), vhits.end(), cmp);
          
            bool hit = false;
			std::vector<int>::const_iterator idx;
			for (idx = vhits.begin(); idx != vhits.end(); ++idx) {
				if (trees()[*idx]->traverse(packet, stats))
                    hit = true;
            }

			if (hit) {
				processHit(i, j, packet, img, stats);
                hit = false;
			}
		}
		stats.localLevelTime_ += localLevelTimer.elapsed();
    } 
}

void rt::TwoLevelRayTracer::setNbClusters(int nbClusters)
{
	nbClusters_ = nbClusters;
}

void rt::TwoLevelRayTracer::setTrees(const FuzzyBVH** trees)
{
	trees_ = trees;
}

void rt::TwoLevelRayTracer::setBoundingBoxes(const BoundingBox* boundingBoxes)
{
	boundingBoxes_ = boundingBoxes;
}
