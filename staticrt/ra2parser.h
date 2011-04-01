#ifndef RT_RA2PARSER_H
#define RT_RA2PARSER_H

#include <triangle.h>
#include <camera.h>

#include <string>
#include <vector>

namespace rt {

/*
 * Parser for the bwfirt ra2 file format.
 *
 * @author	Thomas Loockx
 * @date	october 2008
 */
class Ra2Parser
{
public:
	Ra2Parser(const std::string& filename, const std::string& camfile);
	~Ra2Parser();

	std::vector<Triangle> &triangles() const { return *triangles_; }
	Camera camera() const { return camera_; }

	static void write(
		const std::string& filename,
		const std::vector<Triangle>& triangles);

private:
	std::vector<Triangle>* triangles_;
	bool trianglesInitialized_;
	Camera camera_;
	
	void loadRa2File(const std::string& filename);
	void loadCamFile(const std::string& filename);


	void tokenize(
		const std::string& str, 
		std::vector<std::string>& tokens, 
		const std::string& delimiters = " ") const;
	
	// prevent evil
	Ra2Parser(const Ra2Parser&);
	Ra2Parser& operator=(const Ra2Parser&);
};

} // namespace rt
#endif
