#ifndef RT_PACKETRAYTRACER_H
#define RT_PACKETRAYTRACER_H

#include <image.h>
#include <raytracerbase.h>

#define FOREACH_PACKET(i,j) \
		for(int i = 0; i < camera().nbHorizontalPixels(); i += sqrtPacketSize()) \
		for (int j = 0; j < camera().nbVerticalPixels(); j += sqrtPacketSize())

namespace rt {

class SimpleBVH;
class BVH;
struct Stats;
	
class PacketRayTracer : public RayTracerBase
{
public:
	PacketRayTracer(const Camera& camera, int packetSize);

	void render(const BVH* tree, Image& img) const;
	void render(const BVH* tree, Image& img, Stats& stats) const;

	int packetSize() const { return packetSize_; }

protected:
	void processHit(int i, int j, RayPacket& packet, Image& img) const;
	void processHit(int i, int j, 
					RayPacket& packet, 
					Image& img, 
					Stats& stats) const;
	int sqrtPacketSize() const { return sqrtPacketSize_; }

private:
	int packetSize_;
	int sqrtPacketSize_;

	void reset(int index, RayPacket& packet) const;	
};

} // namespace rt

inline
void rt::PacketRayTracer::processHit(int i, 
									 int j, 
									 RayPacket& packet, 
									 Image& img) const
{
	for (int y = 0; y < sqrtPacketSize(); ++y) {
		for (int x = 0; x < sqrtPacketSize(); ++x) {
			int idx = y * sqrtPacketSize() + x;
			if (packet.hit(idx)) {
				img(i + x,j + y) = shader().diffuse(packet.hitRecord(idx));
				reset(idx, packet);
			}
		}
	}
}

inline
void rt::PacketRayTracer::reset(int index, RayPacket& packet) const
{
	packet.hit(index) = false;
	packet.hitRecord(index).reset();
}

#endif
