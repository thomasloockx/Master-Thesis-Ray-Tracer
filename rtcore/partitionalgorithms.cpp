#include <partitionalgorithms.h>
#include <utils.h>
#include <constants.h>

#include <algorithm>
#include <iostream>
#include <assert.h>

int rt::partitionMedian(const BoundingBox* boundingBoxes,
						std::vector<int>& objectIDs,
						int begin,
						int end,
						int &outAxis)
{
	assert(end - begin > 1);
	
	// calculate the globalbox
	BoundingBox box;
	for (int i = begin; i < end; ++i) 
		box += boundingBoxes[objectIDs[i]];
	Point midPoint = box.center();	
	outAxis = box.longestAxis();

	MedianCompare comp(boundingBoxes, midPoint(outAxis), outAxis);
	std::vector<int>::iterator bound = stable_partition(
										objectIDs.begin() + begin,
										objectIDs.begin() + end,
										comp);

	int median = bound - objectIDs.begin();

	// rebalance
	if (median == begin)
		++median;
	else if (median == end)
		--median;

	return median;
}
