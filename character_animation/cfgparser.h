#ifndef RT_CFGPARSER_H
#define RT_CFGPARSER_H

#include <cal3d.h>
#include <triangle.h>

#include <vector>

namespace rt
{

typedef enum 
{
    IDLE,
    WALK,
    JOG,
    STRUT,
    WAVE,
    SHOOT_ARROW,
    FUNKY
} ANIMATION_TYPE;

typedef enum
{
    WALKING,
    JOGGING,
    FREESTYLE
} ANIMATION_LOOP;
static ANIMATION_LOOP currentAnimation;

/*
 * Parser for .cfg files from the CAL3D lib.
 *
 * @author	Thomas Loockx
 * @date	november 2008
 */
class CFGParser
{
public:
	CFGParser(
		const std::string& name,	
		const std::string& path,
		const std::string& filename,
        ANIMATION_LOOP loop = WALKING);

	static void attachAnimation(ANIMATION_LOOP loop, CalModel* model);
    static void reAttachAnimation(CalModel* model);
    CalModel* model() { return model_; }

private:
	bool loadCFGFile();
	bool loadCoreSkeleton(const std::string& filename);
	bool loadCoreAnimation(const std::string& filename);
	bool loadCoreMesh(const std::string& filename);
	bool loadCoreMaterial(const std::string& filename);
	void handleError() const;
	
	std::string filename_;
	std::string path_;
	CalCoreModel coreModel_;
	CalModel* model_;

	std::vector<int> coreAnimationIds_;
	std::vector<int> coreMeshIds_;
	std::vector<Triangle> triangles_;

	// duplicate code 
	void tokenize(
		const std::string& str, 
		std::vector<std::string>& tokens, 
		const std::string& delimiters = " ") const;
};

} // namespace rt

#endif
