#ifndef RT_PARTITION_ALGORITHM_H
#define RT_PARTITION_ALGORITHM_H

#include <triangle.h>
#include <boundingbox.h>

#include <vector>

namespace rt {

int partitionMedian(const BoundingBox* boundingBoxes,
					std::vector<int>& objectIDs,
					int begin,
					int end,
					int &outAxis);

class MedianCompare
{
public:
	MedianCompare(const BoundingBox* boundingBoxes, float median, int axis)
	: boxes_(boundingBoxes), median_(median), axis_(axis)
	{
	}

	bool operator()(int index) const
	{
		return boxes_[index].center()[axis_] < median_;
	}

private:
	const BoundingBox* boxes_;
	float median_;
	int axis_;
};

} // namespace rt
#endif
