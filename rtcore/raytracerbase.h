#ifndef RT_RAYTRACERBASE_H 
#define RT_RAYTRACERBASE_H

#include <camera.h>
#include <simpleshader.h>

namespace rt {

class RayTracerBase
{
public:
	RayTracerBase(const Camera& camera);

	const Camera& camera() const { return camera_; }

protected:
	const SimpleShader& shader() const { return shader_; }

private:
	Camera camera_;
	SimpleShader shader_;
};

} // namespace rt

#endif
