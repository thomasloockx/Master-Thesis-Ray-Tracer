#ifndef RT_BVH_H
#define RT_BVH_H

#include <boundingbox.h>
#include <raypacket.h>
#include <utils.h>

#include <stack>
#include <stdint.h>
#include <vector>

namespace rt {

struct Stats;

class BVH
{
public:
    BVH(int nbObjects);
    virtual ~BVH();

	bool traverse(RayPacket& packet) const;
    bool traverse(RayPacket& packet, Stats& stats) const;
    int nbObjects() const { return nbObjects_; }
    int nbNodes() const { return nbNodes_; }
    void print() const;	

protected:
    struct Node
	{
		BoundingBox box;
        uint32_t firstChild;
        uint8_t axis; // x = 0, y = 1, z = 2
        uint8_t axisSign; // 0 = left-to-right, 1 = right-to-left
        uint16_t nbObjects;
		
		bool isLeaf() const { return nbObjects != 0; }
		bool isInner() const { return nbObjects == 0; }

        inline
        void makeLeaf(int first, int nbChildren) {
            firstChild = first;
            nbObjects = nbChildren;
            axis = 0;
            axisSign = 0;
        }

        inline 
        void makeInner(int first, uint8_t axis) {
            firstChild = first;
            nbObjects = 0;
            axis = axis;
            axisSign = 0;
        }
	}; // 32 bytes

    std::vector<Node>& nodes_;
    int nextFreeNode_;
    int nbNodes_;

    int nbObjects_;
    std::vector<int>& objectIDs_;

	virtual void build(int nodeID, int beginID, int endID, int axis) = 0;

    void processInner(const Node& node, 
                      int firstActive, 
                      const RayPacket& packet) const;

	virtual bool checkLeaf(const Node& node, 
                           const Ray& ray, 
                           HitRecord& hitRecord) const = 0;

    virtual bool processLeaf(const Node& node, 
                             int firstActive,
                             RayPacket& packet) const; 
    virtual bool processLeaf(const Node& node, 
                             int firstActive,
                             RayPacket& packet,
                             Stats& stats) const;

    int findFirst(const RayPacket& packet,
                  int parentFirstActive, 
                  const BoundingBox& box) const;
    int findFirst(const RayPacket& packet,
                  int parentFirstActive, 
                  const BoundingBox& box,
                  Stats& stats) const;


    /* Helpers for the traversal */
    struct StackRec 
    {
        StackRec(int id, int fstActive)
        : nodeID(id), firstActive(fstActive)
        {
        }

        int nodeID;
        int firstActive;
    };
    mutable std::stack<StackRec> stack_;

private:
    void init();
    void print(int nodeID, int depth) const;

    DISALLOW_COPY_AND_ASSIGN(BVH);
};

} // namespace rt

inline
void rt::BVH::processInner(const Node& n, 
                           int firstActive, 
                           const RayPacket& packet) const
{
    int s = (packet.ray(firstActive).sign[n.axis] > 0) ? 1 : 0;
    stack_.push(StackRec(n.firstChild + s, firstActive));
    stack_.push(StackRec(n.firstChild + (1 - s), firstActive));
}

#endif
