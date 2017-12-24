/*
 * Scene.h
 *
 *  Created on: 26 нояб. 2017 г.
 *      Author: Алексей
 */

#ifndef SRC_SCENE_H_
#define SRC_SCENE_H_


#include <vector>
#include <iostream>
#include "assimp/scene.h"
#include "glm/glm.hpp"
#include "funcs.h"
#include "Mesh.h"

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::string;
using std::vector;


struct box
{
	vec3 min,max;
	bool has_mesh=false;
	bool has_sb=false;
	unsigned int meshid=0;
	vector<box> subboxes;
};

class Scene
{
private:
	int addNode(aiNode *node, const aiScene *scene);
	int addMeshes(aiMesh *mesh, const aiScene *scene);
	vec3 getmin(Mesh mesh);
	vec3 getmax(Mesh mesh);
public:
	vector<Mesh> meshes;
	box rootbox;

	Scene();
	void import(const aiScene * scene);
	void tree_it();
	void addrouter(vec3 wpos);


};

#endif /* SRC_SCENE_H_ */
