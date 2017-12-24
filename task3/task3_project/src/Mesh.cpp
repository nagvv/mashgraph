/*
 * Mesh.cpp
 *
 *  Created on: 26 нояб. 2017 г.
 *      Author: Алексей
 */
#include <glm/glm.hpp>
#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	Mesh::vertices=vertices;
	Mesh::indicies=indices;
	Mesh::textures=textures;
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vec4 color)
{
	Mesh::vertices=vertices;
	Mesh::indicies=indices;
	Mesh::textures=textures;
	Mesh::color=color;
}
