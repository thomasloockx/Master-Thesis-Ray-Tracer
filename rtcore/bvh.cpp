#include <bvh.h>
#include <raypacket.h>
#include <stats.h>

#include <limits>

/******************************************************************************
 * CONSTRUCTION/DESTRUCTION 
 ******************************************************************************/

rt::BVH::BVH(int nbObjects)
: nodes_(*( new std::vector<Node>(2 * nbObjects - 1) )),
  nextFreeNode_(0),
  nbNodes_(0),
  nbObjects_(nbObjects),
  objectIDs_(*( new std::vector<int>(nbObjects) )),
  stack_(std::stack<StackRec>()) 
{
    init();
}

rt::BVH::~BVH()
{
    delete &nodes_;
    delete &objectIDs_;
}

void rt::BVH::init()
{
    for (int id = 0; id < nbObjects(); ++id)
        objectIDs_[id] = id;
}

/******************************************************************************
 * TREE TRAVERSAL
 ******************************************************************************/

bool rt::BVH::traverse(RayPacket& packet) const
{
    assert(stack_.empty());

	bool hit = false;
    stack_.push(StackRec(0,0));	

	while (true) {

		const Node& n = nodes_[stack_.top().nodeID];
		int firstActive = findFirst(packet, stack_.top().firstActive, n.box);
	    stack_.pop();

		if (firstActive < packet.nbRays()) {
			if (n.isInner()) {
                processInner(n, firstActive, packet);
			} else {
                if(processLeaf(n, firstActive, packet)) {
                    hit = true;
                }
   			}
		} 
		if (stack_.empty()) 
			return hit;
	}
}

bool rt::BVH::processLeaf(const Node& n, int firstActive, RayPacket& p) const
{
    bool hit = false;
    for (int r = firstActive; r < p.nbRays(); ++r) {
        if (n.box.hit(p.ray(r), 0, p.hitRecord(r).t)) {
            if (checkLeaf(n, p.ray(r), p.hitRecord(r))) {
                hit = true;
                p.hit(r) = true;
            }
        }
    }
    return hit;
}
 
int rt::BVH::findFirst(const RayPacket& p, 
                       int parentFirstActive, 
                       const BoundingBox& box) const
{
	for (int i = parentFirstActive; i < p.nbRays(); ++i) {
		if (box.hit(p.ray(i), 0, p.hitRecord(i).t)) {
			return i;
		}
#if RT_CULLING 
        if (i == p.begin) {
            if (box.miss(p)) {
                return p.nbRays();    
            }
        }
#endif
	}
	return p.nbRays();
}


/******************************************************************************
 * STAT COLLECTING 
 ******************************************************************************/

bool rt::BVH::traverse(RayPacket& packet, Stats& stats) const
{

    assert(stack_.empty());

	bool hit = false;
    stack_.push(StackRec(0,0));	

	while (true) {

		const Node& n = nodes_[stack_.top().nodeID];
		int firstActive = findFirst(packet, 
                                    stack_.top().firstActive, 
                                    n.box, 
                                    stats);
	    stack_.pop();
        packet.begin = firstActive;

		if (firstActive < packet.nbRays()) {
			if (n.isInner()) {
                processInner(n, firstActive, packet);
            } else {
                if(processLeaf(n, firstActive, packet, stats))
                    hit = true;
            } 
        }
		if (stack_.empty()) 
			return hit;
	}
}

bool rt::BVH::processLeaf(const Node& n, 
                          int firstActive,
                          RayPacket& p,
                          Stats& stats) const
{
    bool hit = false;
    for (int r = firstActive; r < p.nbRays(); ++r) {
        stats.nbBoundingBoxTests_++;
        if (n.box.hit(p.ray(r), 0, p.hitRecord(r).t)) {
            stats.nbTriangleTests_ += n.nbObjects;
            if (checkLeaf(n, p.ray(r), p.hitRecord(r))) {
                hit = true;
                p.hit(r) = true;
            }
        }
    }
    return hit;
}
 
int rt::BVH::findFirst(const RayPacket& p,
                       int parentFirstActive, 
                       const BoundingBox& box,
                       Stats& stats) const 
{
	for (int i = parentFirstActive; i < p.nbRays(); ++i) {
        stats.nbBoundingBoxTests_++;
		if (box.hit(p.ray(i), 0, p.hitRecord(i).t)) {
			return i;
		}
#if RT_CULLING 
        if (i == p.begin) {
            const_cast<RayPacket&>(p).calcIAData();
            if (box.miss(p)) {
                return p.nbRays();    
            }
        }
#endif
	}
	return p.nbRays();
}

/******************************************************************************
 * PRINTING - Use with GraphViz Tool 
 ******************************************************************************/

void rt::BVH::print() const
{
    std::cout << "digraph BVH {" << std::endl;
    print(0, 0);
    std::cout << "}" << std::endl;
}

void rt::BVH::print(int nodeID, int depth) const
{
    const Node& node = nodes_[nodeID];
    if (node.isInner()) {
        std::cout << nodeID << "->" << node.firstChild<< "\n";
        std::cout << nodeID << "->" << node.firstChild+1 << "\n";
        print(node.firstChild, depth+1);
        print(node.firstChild+1, depth+1);
    }
}
