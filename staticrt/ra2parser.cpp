#include <ra2parser.h>
#include <utils.h>

#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>


rt::Ra2Parser::Ra2Parser(const std::string& filename, const std::string& camfile)
: trianglesInitialized_(false)
{
	loadRa2File(filename);
	loadCamFile(camfile);
}

rt::Ra2Parser::~Ra2Parser()
{
	if (trianglesInitialized_)
		delete triangles_;
}

void rt::Ra2Parser::write(
		const std::string& filename,
		const std::vector<Triangle>& triangles)
{
	
	std::ofstream file(filename.c_str(), std::ios::binary);	

	if (!file.is_open()) 
		std::cout << "error opening file " << filename << std::endl;

	for (std::size_t i = 0; i < triangles.size(); ++i) {
		/* write the vertices */
		float f1 = triangles[i].getA().x();	
		file.write((const char*) &f1, sizeof(float));

		float f2 = triangles[i].getA().y();	
		file.write((const char*) &f2, sizeof(float));

		float f3 = triangles[i].getA().z();	
		file.write((const char*) &f3, sizeof(float));

		float f4 = triangles[i].getB().x();	
		file.write((const char*) &f4, sizeof(float));

		float f5 = triangles[i].getB().y();	
		file.write((const char*) &f5, sizeof(float));

		float f6 = triangles[i].getB().z();	
		file.write((const char*) &f6, sizeof(float));

		float f7 = triangles[i].getC().x();	
		file.write((const char*) &f7, sizeof(float));

		float f8 = triangles[i].getC().y();	
		file.write((const char*) &f8, sizeof(float));

		float f9 = triangles[i].getC().z();	
		file.write((const char*) &f9, sizeof(float));
	}

	file.close();
}

void rt::Ra2Parser::loadRa2File(const std::string& filename)
{
	FILE* fp;
	if (!(fp = fopen(filename.c_str(), "r"))){
		std::cerr << "Scene: Error loading ra2 triangles from " << filename << std::endl;
 		exit(1);
	}
	
	long filesize;	
	fseek(fp, 0, SEEK_END); 
	filesize = ftell(fp); 
	fseek(fp, 0, SEEK_SET);
	
	std::size_t nbTriangles = filesize / ( 9 * sizeof(float));
	std::size_t nbVertices = nbTriangles * 3;
	std::size_t nbFloats = nbVertices * 3;

	//float* floats= new float[nbFloats];
	
	//fread(floats , sizeof(float), nbFloats, fp);		
	fclose(fp);	

	std::ifstream infile(filename.c_str(), std::fstream::binary);

	if (!infile.is_open()) {
		std::cerr << "Scene: Error loading ra2 triangles from " << filename << std::endl;
		exit(1);
	}

	union 
	{
		char bytes[4];	
		float f;
	} f1, f2, f3, f4, f5, f6, f7, f8, f9;

	triangles_ = new std::vector<Triangle>(nbTriangles);
	trianglesInitialized_ = true;
	
	std::size_t i = 0;
	while (infile.good() && i < triangles_->size()) {

		// boilerplate but we want in efficient and easy
		infile.get(f1.bytes[0]);	
		infile.get(f1.bytes[1]);	
		infile.get(f1.bytes[2]);	
		infile.get(f1.bytes[3]);	

		infile.get(f2.bytes[0]);	
		infile.get(f2.bytes[1]);	
		infile.get(f2.bytes[2]);	
		infile.get(f2.bytes[3]);	

		infile.get(f3.bytes[0]);	
		infile.get(f3.bytes[1]);	
		infile.get(f3.bytes[2]);	
		infile.get(f3.bytes[3]);	

		infile.get(f4.bytes[0]);	
		infile.get(f4.bytes[1]);	
		infile.get(f4.bytes[2]);	
		infile.get(f4.bytes[3]);	

		infile.get(f5.bytes[0]);	
		infile.get(f5.bytes[1]);	
		infile.get(f5.bytes[2]);	
		infile.get(f5.bytes[3]);	

		infile.get(f6.bytes[0]);	
		infile.get(f6.bytes[1]);	
		infile.get(f6.bytes[2]);	
		infile.get(f6.bytes[3]);	

		infile.get(f7.bytes[0]);	
		infile.get(f7.bytes[1]);	
		infile.get(f7.bytes[2]);	
		infile.get(f7.bytes[3]);	

		infile.get(f8.bytes[0]);	
		infile.get(f8.bytes[1]);	
		infile.get(f8.bytes[2]);	
		infile.get(f8.bytes[3]);	

		infile.get(f9.bytes[0]);	
		infile.get(f9.bytes[1]);	
		infile.get(f9.bytes[2]);	
		infile.get(f9.bytes[3]);	

		(*triangles_)[i++] = Triangle (Point(f1.f, f2.f, f3.f), 
									   Point(f4.f, f5.f, f6.f), 
									   Point(f7.f, f8.f, f9.f));
	}
	infile.close();
}

void rt::Ra2Parser::loadCamFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	std::string line;
	std::vector<std::string> tokens;

	Point position;
	Vector direction;
	Vector up;
	float depth;
	float filmSizeY;

	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, line);
			tokenize(line, tokens);	

			if (tokens[0] == "position") {
				float x = atof(tokens[1].c_str());									
				float y = atof(tokens[2].c_str());									
				float z = atof(tokens[3].c_str());									
				position = Point(x,y,z);
			}
			else if (tokens[0] == "direction") {
				float x = atof(tokens[1].c_str());									
				float y = atof(tokens[2].c_str());									
				float z = atof(tokens[3].c_str());									
				direction = Vector(x,y,z);
			}
			else if (tokens[0] == "up") {
				float x = atof(tokens[1].c_str());									
				float y = atof(tokens[2].c_str());									
				float z = atof(tokens[3].c_str());									
				up = Vector(x,y,z);
			}
			else if (tokens[0] == "depth") {
				depth = atof(tokens[1].c_str());
			}
			else if (tokens[0] == "filmSizeY") {
				filmSizeY = atof(tokens[1].c_str());
			}
		}	

		float fovy = toDegrees(atan((filmSizeY / 2) / depth));
		camera_ = Camera(position, direction, up, fovy, fovy, depth, 256, 256);
	}
	else 
		std::cerr << "Unabe to open file " << filename << std::endl;

	file.close();
}

void rt::Ra2Parser::tokenize(
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
