#ifndef RT_TWOLEVELREFIT_H
#define RT_TWOLEVELREFIT_H

#include <fuzzy.h>

namespace rt {

class Matrix4;
class SimpleBVH;
class Vector;

class TwoLevelRefit : public Fuzzy
{
public:
    TwoLevelRefit(CalModel* model, 
				  const Camera& camera, 
				  int nbFrames, 
				  bool stats);

protected:
	void initRendering();
	void updateState();
};

} // namespace rt

#endif
