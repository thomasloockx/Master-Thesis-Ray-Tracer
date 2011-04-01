#include <image.h>
#include <packetraytracer.h>
#include <raypacket.h>
#include <simplebvh.h>
#include <stats.h>

#include <cassert>


rt::PacketRayTracer::PacketRayTracer(const Camera& camera, int packetSize)
: RayTracerBase(camera),
  packetSize_(packetSize), 
  sqrtPacketSize_(sqrt(packetSize))
{
	assert(packetSize % 4 == 0);
}

void rt::PacketRayTracer::render(const BVH* tree, Image& img) const
{
	RayPacket packet(packetSize());

	FOREACH_PACKET(i,j) {
		camera().createPacket(i, j, packet);
		if (tree->traverse(packet))
			processHit(i, j, packet, img);	
	}
}

void rt::PacketRayTracer::render(const BVH* tree, 
								 Image& img, 
								 Stats& stats) const
{
	stats.nbNodes_ = tree->nbNodes();
	stats.nbTriangles_ = tree->nbObjects();

	RayPacket packet(packetSize());
	
	FOREACH_PACKET(i,j) {
		camera().createPacket(i, j, packet);
		if (tree->traverse(packet, stats))
			processHit(i, j, packet, img, stats);
	}
}

void rt::PacketRayTracer::processHit(int i, int j, 
									 RayPacket& packet, 
									 Image& img, 
									 Stats& stats) const
{
	for (int y = 0; y < sqrtPacketSize(); ++y) {
		for (int x = 0; x < sqrtPacketSize(); ++x) {
			int idx = y * sqrtPacketSize() + x;
			if (packet.hit(idx)) {
				stats.nbHits_++;
				img(i + x,j + y) = shader().diffuse(packet.hitRecord(idx));
				reset(idx, packet);
			}
		}
	}
}
