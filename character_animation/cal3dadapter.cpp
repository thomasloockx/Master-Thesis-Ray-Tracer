#include <boundingbox.h>
#include <cal3dadapter.h>
#include <cal3dconversions.h>
#include <color.h>
#include <constants.h>
#include <matrix4.h>
#include <utils.h>
#include <triangle.h>
#include <color.h>
#include <cluster.h>
#include <cfgparser.h>

#include <cal3d.h>
#include <coretrack.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <list>

#include <boost/foreach.hpp>

rt::Cal3dAdapter::Cal3dAdapter(CalModel* model)
: model_(model),
  clusters_(new Cluster[nbBones()]),
  colors_(new Color[nbBones()]),
  boneTransforms_(new Matrix4[nbBones()]),
  invBoneTransforms_(new Matrix4[nbBones()]),
  restVertices_( *(new std::vector<Point>) ),
  currentVertices_( *(new std::vector<Point>) ),
  residualVertices_( *(new std::vector<Point> ) ),
  influences_( *(new std::vector< std::vector<Influence> >) ),
  faces_( *(new std::vector<Face>) )
{
	initColors();
	initialize();
	calcFuzzyBoxes();
	fillClusterWithRestPose();
	update(0);
}

rt::Cal3dAdapter::~Cal3dAdapter()
{
	delete[] clusters_;
	delete[] colors_;
	delete[] clusterOf_;
	delete[] boneTransforms_;
	delete[] invBoneTransforms_;
	delete &restVertices_;
	delete &currentVertices_;
	delete &influences_;
	delete &faces_;
}


void rt::Cal3dAdapter::initColors()
{
	std::cout << "# Initializing Colors" << std::endl;
	for (int i = 0; i < nbBones(); ++i) {
		int r = drand48() * 255;
		int g = drand48() * 255;
		int b = drand48() * 255;
		colors_[i] = Color(r,g,b);
	}
}

void rt::Cal3dAdapter::initialize()
{
	std::cout << "# Initializing Vertices/Influences" << std::endl;
	assert(restVertices_.empty());
	assert(influences_.empty());
	assert(faces_.empty());

	for (int b = 0; b < nbBones(); ++b) 
		clusters_[b].setBoneId(b); 

	for (int i = 0; i < nbMeshes(); ++i) {
		for (int j = 0; j < nbSubmeshes(i); ++j) {
			CalCoreSubmesh* submesh = coreSubmesh(i,j);

			std::vector<Vertex>& cal_vertices = submesh->getVectorVertex();
			std::vector<Face>& cal_faces = submesh->getVectorFace();
			
            std::vector<Vertex> vertices;
            std::vector<Face> faces;

            /* Subdivide the meshes. */
            int nbSubDivisions = 1;
            for (int d = 0; d < nbSubDivisions; ++d) {
            vertices.insert(vertices.end(), 
                            cal_vertices.begin(), 
                            cal_vertices.end());
            faces.insert(faces.end(), 
                         cal_faces.begin(), 
                         cal_faces.end());
            }

			BOOST_FOREACH(Face face, faces) {
				Face fcopy(face);
				// there's an offset
				int offset = restVertices_.size();
				fcopy.vertexId[0] = face.vertexId[0] + offset;
				fcopy.vertexId[1] = face.vertexId[1] + offset;
				fcopy.vertexId[2] = face.vertexId[2] + offset;
				faces_.push_back(fcopy);
			}

			BOOST_FOREACH(Vertex vertex, vertices) {
				restVertices_.push_back(toPoint(vertex.position));
				currentVertices_.push_back(toPoint(vertex.position));
				residualVertices_.push_back(toPoint(vertex.position));
				influences_.push_back(vertex.vectorInfluence);
			}
		}
	}
	clusterOf_ = new int[nbTriangles()];
}

/******************************************************************************
 * CLUSTER FILLING
 ******************************************************************************/

void rt::Cal3dAdapter::fillClusterWithRestPose()
{
	fillCluster(restVertices_);
}

void rt::Cal3dAdapter::fillClusterWithCurrentPose()
{
	fillCluster(currentVertices_);
}

void rt::Cal3dAdapter::fillClusterWithResidualPose()
{
	fillCluster(residualVertices_);
}

void rt::Cal3dAdapter::fillCluster(const std::vector<Point>& vertices)
{
	for (int b = 0; b < nbBones(); ++b) {
		Cluster& cluster = clusters_[b];
		cluster.clearTriangles();

		for (std::size_t i = 0; i < cluster.triangleIds().size(); ++i) {
			int id = cluster.triangleIds()[i];
			Point v0 = vertices[faces_[id].vertexId[0]];
			Point v1 = vertices[faces_[id].vertexId[1]];
			Point v2 = vertices[faces_[id].vertexId[2]];
			Triangle triangle(v0, v1, v2, colors_[b]);
			cluster.addTriangle(triangle);
		}
	}
}

/******************************************************************************
 * UPDATE POSE - update transformation & skin vertices
 ******************************************************************************/

void rt::Cal3dAdapter::update(float elapsedTime)
{
	model_->getMixer()->updateAnimation(elapsedTime);
	model_->getMixer()->updateSkeleton();

	updateTransformations();
	skinVertices();
	skinResidual();
}

void rt::Cal3dAdapter::skinVertices()
{
	assert(currentVertices_.size() == restVertices_.size());
    for (std::size_t v = 0; v < currentVertices_.size(); ++v) {

		currentVertices_[v] = Point();
		for (std::size_t i = 0; i < influences_[v].size(); ++i) {
			Matrix4 mat = boneTransforms_[influences_[v][i].boneId];
			Point newpos = influences_[v][i].weight * (mat * restVertices_[v]);
			// HACK to add a point to a point
			for (int c = 0; c < 3; ++c) 
				currentVertices_[v][c] += newpos[c];
			// END HACK
		}
	}
}

void rt::Cal3dAdapter::updateTransformations()
{
	for (int b = 0; b < nbBones(); ++b) {
		Matrix4 mat = createMatrix(bone(b)->getTransformMatrix(),
								   bone(b)->getTranslationBoneSpace());
		boneTransforms_[b] = mat;	
		invBoneTransforms_[b] = mat.inverse();
		clusters_[b].setLocal2World(boneTransforms_[b]);
		clusters_[b].setWorld2Local(invBoneTransforms_[b]);
	}
}

void rt::Cal3dAdapter::skinResidual()
{
	skinVertices();

	for (int i = 0; i < nbTriangles(); ++i) {
		int b = clusterOf_[i];
		Face face = faces_[i];
		for (int c = 0; c < 3; ++c) {
			int v = face.vertexId[c];
			residualVertices_[v] = invBoneTransforms_[b] * currentVertices_[v];
		}
	}
}

void rt::Cal3dAdapter::calcFuzzyBoxes()
{
	std::cout << "# Calculating Fuzzy Boxes / Assigning Bones" << std::endl;
	int vertices = residualVertices_.size();
	int bones = nbBones();
	int triangles = nbTriangles();

	BoundingBox* vtxFuzzyBox = new BoundingBox[vertices*bones];
	for (int b = 0; b < nbBones(); ++b) {
		for (int v = 0; v < vertices; ++v) {
			vtxFuzzyBox[b*vertices+v][0] = restVertices_[v];
			vtxFuzzyBox[b*vertices+v][1] = restVertices_[v];
		}
	}

#ifdef RT_ANIMATION_CYCLE_SAMPLING 
	/* animation cycle sampling. */	
	std::cout << "# Animation Cycle Sampling" << std::endl;
	for (int a = 0; a < coreModel()->getCoreAnimationCount(); ++a) {
		model_->getMixer()->clearCycle(a, 0.0f);
	}

	int maxi = 0;
	for (int a = 0; a < coreModel()->getCoreAnimationCount(); ++a) {
		model_->getMixer()->blendCycle(a, 1.0f, 0.0f);
		std::list<CalCoreTrack*> &cts = coreModel()->getCoreAnimation(a)->getListCoreTrack();
		int maxkf = 0; 
		std::list<CalCoreTrack*>::iterator ct;
		for (ct = cts.begin(); ct != cts.end(); ++ct) {
			maxkf = std::max(maxkf, (*ct)->getCoreKeyframeCount());
		}
		maxi += maxkf;

		float deltaTime = coreModel()->getCoreAnimation(a)->getDuration() / maxkf;	

		for (int kf = 0; kf < maxkf; ++kf) {
			model_->getMixer()->updateAnimation(deltaTime);
			model_->getMixer()->updateSkeleton();
			updateTransformations();
			skinVertices();

			for (int b = 0; b < bones; ++b) {
				for (int v = 0; v < vertices; ++v) {
					Point vtx = invBoneTransforms_[b] * currentVertices_[v];
					vtxFuzzyBox[b*vertices+v] += BoundingBox(vtx, vtx);
				}
			}
		}
		model_->getMixer()->clearCycle(a, 0.0f);
	}
    CFGParser::reAttachAnimation(model_);
#else
	std::cout << "#Random Sampling" << std::endl;
	for (int s = 0; s < NB_SAMPLES; ++s) {

		for (int b = 0; b < nbBones(); ++b) {
			bone(b)->clearState();
			CalVector f(drand48()-0.5f, drand48()-0.5f, drand48()-0.5f);
			CalVector t(drand48()-0.5f, drand48()-0.5f, drand48()-0.5f);
			f.normalize();
			t.normalize();
			bone(b)->setRotation(shortestArc(f, t));
			bone(b)->calculateState();
		}
		updateTransformations();
		skinVertices();
		
		for (int b = 0; b < bones; ++b) {
			for (int v = 0; v < vertices; ++v) {
				Point vtx = invBoneTransforms_[b] * currentVertices_[v];
				vtxFuzzyBox[b*vertices+v] += BoundingBox(vtx, vtx); 
			}
		}
	}
#endif
	// assign bones
	for (int d = 0; d < triangles; ++d) {
		BoundingBox minfuzzybox;
		float minarea = INF;
		int minbone = -1;

		for (int b = 0; b < bones; ++b) {
			BoundingBox fuzzybox;
			fuzzybox += vtxFuzzyBox[faces_[d].vertexId[0] + b*vertices];
			fuzzybox += vtxFuzzyBox[faces_[d].vertexId[1] + b*vertices];
			fuzzybox += vtxFuzzyBox[faces_[d].vertexId[2] + b*vertices];

			if (fuzzybox.area() < minarea) {
				minarea = fuzzybox.area();
				minfuzzybox = fuzzybox;
				minbone = b;
			}
		}
		assert(minbone != -1);
		clusterOf_[d] = minbone;
		clusters_[minbone].addFuzzyBox(minfuzzybox);
	}

	delete[] vtxFuzzyBox;

	// add triangles to cluster
	for (int d = 0; d < triangles; ++d) {
		int id = clusterOf_[d];
		clusters_[id].addTriangleId(d);
	}
}

/******************************************************************************
 * GETTERS 
 ******************************************************************************/

int rt::Cal3dAdapter::nbBones() const
{
	return static_cast<int>(skeleton()->getVectorBone().size());
}

int rt::Cal3dAdapter::nbMeshes() const
{
	return coreModel()->getCoreMeshCount();
}

int rt::Cal3dAdapter::nbSubmeshes(int meshId) const
{
	assert(0 <= meshId && meshId < nbMeshes());	
	return coreModel()->getCoreMesh(meshId)->getCoreSubmeshCount(); 
}

int rt::Cal3dAdapter::totalNbSubmeshes() const
{
	int count = 0;
	for (int i = 0; i < nbMeshes(); ++i)
		count += nbSubmeshes(i);	
	return count;
}

int rt::Cal3dAdapter::nbTriangles() const
{
	return faces_.size();
}

const rt::Cluster* rt::Cal3dAdapter::clusters() const
{
	return clusters_;
}

const int* rt::Cal3dAdapter::clusterOf() const
{
	return clusterOf_;	
}

int rt::Cal3dAdapter::filledClusterCount() const
{
	int count = 0;
	for (int c = 0; c < nbBones(); ++c) 
		if (!clusters_[c].empty())
			++count;	
	return count;
}

void rt::Cal3dAdapter::current(std::vector<Triangle>& triangles) const
{
	fillVector(currentVertices_, triangles);
}

void rt::Cal3dAdapter::rest(std::vector<Triangle>& triangles) const
{
	fillVector(restVertices_, triangles);
}

void rt::Cal3dAdapter::residual(std::vector<Triangle>& triangles) const
{
	fillVector(residualVertices_, triangles);
}

void rt::Cal3dAdapter::fillVector(const std::vector<Point>& vertices,
								  std::vector<Triangle>& triangles) const
{
	triangles.clear();

	for (int b = 0; b < nbBones(); ++b) {
		Cluster& cluster = clusters_[b];
		for (std::size_t i = 0; i < cluster.triangleIds().size(); ++i) {
			int id = cluster.triangleIds()[i]; 
			Point v0 = (vertices[faces_[id].vertexId[0]]);
			Point v1 = (vertices[faces_[id].vertexId[1]]);
			Point v2 = (vertices[faces_[id].vertexId[2]]);
			Triangle triangle(v0, v1, v2, colors_[b]);
			triangles.push_back(triangle);
		}
	}
}

CalSkeleton* rt::Cal3dAdapter::skeleton() const
{
	return model_->getSkeleton();
}

CalSkeleton* rt::Cal3dAdapter::skeleton() 
{
	return const_cast<const Cal3dAdapter*>(this)->skeleton();
}

CalCoreModel* rt::Cal3dAdapter::coreModel() const
{
	return model_->getCoreModel();
}

CalCoreModel* rt::Cal3dAdapter::coreModel()
{
	return const_cast<const Cal3dAdapter*>(this)->coreModel();
}

CalBone* rt::Cal3dAdapter::bone(int boneId) const
{
	assert(0 <= boneId && boneId < nbBones());
	return skeleton()->getBone(boneId);
}

CalBone* rt::Cal3dAdapter::bone(int boneId)
{
	return const_cast<const Cal3dAdapter*>(this)->bone(boneId);
}

CalCoreSubmesh* rt::Cal3dAdapter::coreSubmesh(int meshId, int submeshId) const
{
	assert(meshId >= 0 && meshId < nbMeshes());
	assert(submeshId >= 0 && submeshId < nbSubmeshes(meshId));
	return coreModel()->getCoreMesh(meshId)->getCoreSubmesh(submeshId);
}

CalCoreSubmesh* rt::Cal3dAdapter::coreSubmesh(int meshId, int submeshId)
{
	return const_cast<const Cal3dAdapter*>(this)->coreSubmesh(meshId, submeshId);
}

