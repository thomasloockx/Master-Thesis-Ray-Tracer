#include <boundingbox.h>
#include <cfgparser.h>
#include <color.h>

#include <cmath>
#include <iostream>

rt::CFGParser::CFGParser(
		const std::string& name,
		const std::string& path,
		const std::string& filename,
        ANIMATION_LOOP loop)
:	filename_(filename),	
	path_(path),
	coreModel_(CalCoreModel(name))
{
	loadCFGFile();

	model_ = new CalModel(&coreModel_);
	model_->setLodLevel(0.5f);

	for (int meshId = 0; meshId < coreModel_.getCoreMeshCount(); ++meshId) {
		if (!model_->attachMesh(meshId))
			handleError();
	}
	attachAnimation(loop, model_);
   currentAnimation = loop;
}

void rt::CFGParser::attachAnimation(ANIMATION_LOOP loop, CalModel* model)
{
	float weight  = 1.0f;
    float blend   = 0.0f;
    
    if (loop == WALKING) {
        model->getMixer()->blendCycle(WALK, weight, blend);
        model->getMixer()->executeAction(WAVE, 0.0f, 0.0f);
    } 
    else if (loop == JOGGING) {
        model->getMixer()->blendCycle(JOG, weight, blend);
        model->getMixer()->executeAction(SHOOT_ARROW, 0.0f, 0.0f);
    } 
    else if (loop == FREESTYLE) {
        model->getMixer()->executeAction(FUNKY, 0.0f, 1.0f);
    } 
    else {
        model->getMixer()->blendCycle(IDLE, weight, blend);
    }
}

void rt::CFGParser::reAttachAnimation(CalModel* model)
{
    attachAnimation(currentAnimation, model);
}

bool rt::CFGParser::loadCFGFile()
{
	std::ifstream file((path_ + filename_).c_str());
	std::string line;
	std::vector<std::string> tokens;

	if (file.is_open()) {
		while (!file.eof()) {

			getline(file, line);
			tokenize(line, tokens, "=");	

			if (tokens[0] == "skeleton") {
				if (!loadCoreSkeleton(path_ + tokens[1])) {
					handleError();
					return false;
				}
			}
			else if (tokens[0] == "animation") {
				if (!loadCoreAnimation(path_ + tokens[1])) {
					handleError();
					return false;
				}	
			}
			else if (tokens[0] == "mesh") {
				if (!loadCoreMesh(path_ + tokens[1])) {
					handleError();
					return false;
				}
			}
			else if (tokens[0] == "material") {
				if (!loadCoreMaterial(path_ + tokens[1])) {
					handleError();
					return false;
				}
			}
		}	
	}
	else 
		std::cerr << "Unabe to open file " << filename_ << std::endl;

	file.close();

	//coreModel_.getCoreSkeleton()->calculateBoundingBoxes(&coreModel_);
    return true;
}

bool rt::CFGParser::loadCoreSkeleton(const std::string& filename)
{
	return coreModel_.loadCoreSkeleton(filename);	
}

bool rt::CFGParser::loadCoreAnimation(const std::string& filename)
{
	int animationId = coreModel_.loadCoreAnimation(filename);
	coreAnimationIds_.push_back(animationId);
	return animationId != -1;
}

bool rt::CFGParser::loadCoreMesh(const std::string& filename)
{
	int meshId = coreModel_.loadCoreMesh(filename);
	coreMeshIds_.push_back(meshId);
	return meshId != -1;
}

bool rt::CFGParser::loadCoreMaterial(const std::string& filename)
{
	int materialId = coreModel_.loadCoreMaterial(filename);
	return materialId != -1;
}

void rt::CFGParser::tokenize(
	const std::string& str, 
	std::vector<std::string>& tokens, 
	const std::string& delimiters) const
{
	tokens.clear();

	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
    }
}

void rt::CFGParser::handleError() const
{
	CalError::printLastError();
}
