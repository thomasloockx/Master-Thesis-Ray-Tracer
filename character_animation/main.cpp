#include <camera.h>
#include <cfgparser.h>
#include <fastrebuild.h>
#include <fuzzy.h>
#include <twolevelrefit.h>

#include <refit.h>
#include <cal3dadapter.h>

#include <cstdlib>
#include <string>

using namespace rt;

typedef enum 
{
    REBUILD,
    REFIT,
    FUZZY,
	TWOLEVELREFIT
} RENDER_MODE;

typedef enum
{
	CALLY,
	SKELETON,
	PALADIN
} MODEL;

/*
 * Framework for character animation.
 * 
 * command:
 * character_animation mode model loop nb_frames res
 * mode = rebuild/refit/fuzzy
 * model = cally/skeleton/paladin
 * loop = walking/jogging/freestyle
 * res = resolution in pixels (we render only squares)
 * stats = stats (optional)
 * 
 * 
 * @author  Thomas Loockx
 * @date    april 2009
 */
int main(int argc, char **argv) 
{
	if (argc < 6) {
		std::cerr << "wrong number of args" << std::endl;	
		std::cerr << "Usage: character_animation mode model loop #frames resolution (stats)" << std::endl;	
		exit(-1);
	}

    /* settings */
    RENDER_MODE mode = REBUILD;
	MODEL model = SKELETON;
    ANIMATION_LOOP animation_loop = WALKING;
    int nbFrames = 50;
	int res = 512;
	bool stats = false;

	/* command line parsing */
	std::string modeStr(argv[1]);
	std::string modelStr(argv[2]);
	std::string loopStr(argv[3]);
	std::string nbFrameStr(argv[4]);
	std::string resStr(argv[5]);

	std::string statStr;
	if (argc > 6) 
		statStr = std::string(argv[6]);

	if (modeStr == "rebuild") mode = REBUILD;
	else if (modeStr == "refit") mode = REFIT;
	else if (modeStr == "fuzzy") mode = FUZZY;
	else if (modeStr == "twolevelrefit") mode = TWOLEVELREFIT;
	else {
		std::cerr << modeStr << " should be rebuild/refit/fuzzy" << std::endl; 
		exit(-1);
	}

	if (modelStr == "cally") model = CALLY;
	else if (modelStr == "skeleton") model = SKELETON;
	else if (modelStr == "paladin") model = PALADIN;
	else {
		std::cerr << modelStr << " should be cally/skeleton/paladin" << std::endl;
		exit(-1);
	}

	if (loopStr == "walking") animation_loop = WALKING;
	else if (loopStr == "jogging") animation_loop = JOGGING;
	else if (loopStr == "freestyle") animation_loop = FREESTYLE;
	else {
		std::cerr << modeStr << " should be walking/jogging/freestyle" << std::endl;
		exit(-1);
	}

	nbFrames = atoi(nbFrameStr.c_str());
	if (nbFrames <= 0) {
		std::cerr << "#frames should be greater than zero" << std::endl;
		exit(-1);
	}

	res = atoi(resStr.c_str());
	if (res <= 0) {
		std::cerr << "resolution should be greater than zero" << std::endl;
		exit(-1);
	}

	if (statStr == "stats") stats = true;

	/* select model */	
	CFGParser* parser;
	if (model == CALLY) {
		parser = new CFGParser("cally", "characters/cally/", "cally.cfg", animation_loop);
	}	
	else if (model == SKELETON) {
		parser = new CFGParser("skeleton", "characters/skeleton/", "skeleton.cfg", animation_loop);
	} 
	else if (model == PALADIN) {
		parser = new CFGParser("paladin", "characters/paladin/", "paladin.cfg", animation_loop);
	} 
	else {
		std::cerr << "error: wrong model" << std::endl;
		exit(-1);
	}

	Camera camera(Point(-120,-120,220), Vector(1,1,-1), Vector(0,0,1), 30, 30, 1, res, res);
	// Special camera for rendering the residual pose
    //Camera camera(Point(0,-170,90), Vector(0,1,0), Vector(0,0,1), 30, 30, 1, res, res);
    // Camera for the pelvis of cally
    //Camera camera(Point(0,-30,100), Vector(0,1,0), Vector(0,0,1), 30, 30, 1, res, res);
	//other camera for the palading, since smaller model
	if (model == PALADIN)
		camera = Camera(Point(-60,-60,110), Vector(1,1,-1), Vector(0,0,1), 30, 30, 1, res, res);


	/* select test case */
    TestCase* testCase;
    if (mode == REBUILD) {
        testCase = new FastRebuild(parser->model(), camera, nbFrames, stats);
    } 
    else if (mode == REFIT) {
        testCase = new Refit(parser->model(), camera, nbFrames, stats);
    } 
    else if (mode == FUZZY) {
        testCase = new Fuzzy(parser->model(), camera, nbFrames, stats, NORMAL);
    //    static_cast<Fuzzy*>(testCase)->startResidual();
	}
    else if (mode == TWOLEVELREFIT) {
        testCase = new TwoLevelRefit(parser->model(), camera, nbFrames, stats);
    } else {
        std::cerr << "error: wrong mode" << std::endl;
        exit(-1);
    }

    testCase->run();

    delete testCase;
	delete parser;

    exit(0);
}
