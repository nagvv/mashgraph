/*
 * Mesh.h
 *
 *  Created on: 26 нояб. 2017 г.
 *      Author: Алексей
 */

#ifndef SRC_MESH_H_
#define SRC_MESH_H_


#include <vector>
#include <string>
#include <glm/glm.hpp>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::string;
using std::vector;

struct Vertex
{
	vec3 pos;
	vec3 norm; //not used
	vec2 texcoord;//unused

//	vec3 tangent;//not used
//	vec3 bitangent;//not used
};

struct Texture//unused
{
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indicies;
	vector<Texture> textures;
	vec4 color = vec4(1,1,1,1);//[0,1]
	bool transparent=false;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vec4 color);
};

#endif /* SRC_MESH_H_ */
