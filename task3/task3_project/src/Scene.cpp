/*
 * Scene.cpp
 *
 *  Created on: 26 нояб. 2017 г.
 *      Author: Алексей
 */

#include "Scene.h"

extern int wt_mode;


vector<Texture> loadTexture(aiMaterial *material, aiTextureType type, string typeName)
{
	vector<Texture> ret;
	//ther nothing yet
//	for(int i=0; i< material->GetTextureCount(type); ++i)
//	{
//		aiString str;
//		material->GetTexture(type, i, &str);
//		Texture tex;
//		tex.id = TextureFromFile(str.C_Str(), directory);
//		tex.type = typeName;
//		tex.path = str;
//		ret.push_back(tex);
//	}
	return ret;
}


int Scene::addMeshes(aiMesh *mesh, const aiScene *scene)
{
	int num=0;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	//verticies
	for(unsigned int i=0; i<mesh->mNumVertices; ++i)
	{
		Vertex vert;

		vec3 v;
		v.x=mesh->mVertices[i].x;
		v.y=mesh->mVertices[i].y;
		v.z=mesh->mVertices[i].z;
		vert.pos=v;

		v.x=mesh->mNormals[i].x;
		v.y=mesh->mNormals[i].y;
		v.z=mesh->mNormals[i].z;
		vert.norm=v;


		vec2 v2=vec2(0,0);
		if(mesh->HasTextureCoords(0))
		{
			v2.x=mesh->mTextureCoords[0][i].x;
			v2.y=mesh->mTextureCoords[0][i].y;
		}
		vert.texcoord=v2;

//		v.x = mesh->mTangents[i].x;
//		v.y = mesh->mTangents[i].y;
//		v.z = mesh->mTangents[i].z;
//		vert.tangent = v;
//
//		v.x = mesh->mBitangents[i].x;
//		v.y = mesh->mBitangents[i].y;
//		v.z = mesh->mBitangents[i].z;
//		vert.bitangent = v;

		vertices.push_back(vert);
	}

	unsigned int num_faces=mesh->mNumFaces;
	for(unsigned int i=0; i< num_faces; ++i)
	{
		unsigned int num_indicies=mesh->mFaces[i].mNumIndices;
		//num_indicies vsegda = 3
		for(unsigned int j=0; j< num_indicies; ++j )
		{
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	num+=mesh->mNumFaces;

	//yet unused
//	if(mesh->mMaterialIndex >= 0)
//	{
//		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//		vector<Texture> diffMap = loadTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
//		textures.insert(textures.end(), diffMap.begin(), diffMap.end());
//		vector<Texture> specMap = loadTexture(material,aiTextureType_SPECULAR, "texture_specular");
//		textures.insert(textures.end(), specMap.begin(), specMap.end());
//	}


	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
	aiColor3D diffuse;
	vec4 color = vec4(1,1,1,1);
	if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
	{
		color = vec4(diffuse.r, diffuse.g, diffuse.b,1);
	}

	float opacity;
	if (AI_SUCCESS == mat->Get(AI_MATKEY_OPACITY, opacity))
	{
		color.a=opacity;
	}

	Mesh tmesh=Mesh(vertices, indices, textures, color);
	string name=mesh->mName.C_Str();
	int len=name.size();
	if(len>=4 && wt_mode==1)
	{	if(name[len-4]=='_' && name[len-3]=='w' && name[len-2]=='t' && name[len-1]=='w')
			{tmesh.transparent=true;}
	}
	else if(wt_mode==2)
	{	tmesh.transparent=true;}
	meshes.push_back(tmesh);

	return num;
}

int Scene::addNode(aiNode *node, const aiScene *scene)
{
	int num=0;
	for(unsigned int i=0; i< node->mNumChildren; ++i)
	{
		num +=addNode(node->mChildren[i], scene);
	}

	for(unsigned int i=0; i< node->mNumMeshes; ++i)
	{
		num+= addMeshes(scene->mMeshes[node->mMeshes[i]], scene);
	}

	return num;
}

void Scene::import(const aiScene * scene)
{
	addNode(scene->mRootNode, scene);
}

vec3 Scene::getmin(Mesh mesh)
{
	vec3 ret;
	int verts = mesh.vertices.size();
	for(int j=0; j< verts; ++j)
	{
		vec3 temp=mesh.vertices[j].pos;
		if( temp.x < ret.x )
			ret.x=temp.x;
		if( temp.y < ret.y )
			ret.y=temp.y;
		if( temp.z < ret.z )
			ret.z=temp.z;
	}
	return ret;
}

vec3 Scene::getmax(Mesh mesh)
{
	vec3 ret;
	int verts = mesh.vertices.size();
	for(int j=0; j< verts; ++j)
	{
		vec3 temp=mesh.vertices[j].pos;
		if( temp.x > ret.x )
			ret.x=temp.x;
		if( temp.y > ret.y )
			ret.y=temp.y;
		if( temp.z > ret.z )
			ret.z=temp.z;
	}
	return ret;
}

void Scene::tree_it()
{
	rootbox.min=min_vertex(scene->mRootNode);
	rootbox.max=max_vertex(scene->mRootNode);
	for(unsigned int i=0; i< meshes.size(); ++i)
	{
		box b;
		b.min=getmin(meshes[i]);
		b.max=getmax(meshes[i]);
		b.has_mesh=true;
		b.meshid=i;
		rootbox.subboxes.push_back(b);
	}
	if(rootbox.subboxes.size()>0)
		rootbox.has_sb=true;
	std::cout << "subboxsize " <<  rootbox.subboxes.size() << std::endl;
}

void Scene::addrouter(vec3 wpos)
{
	//creating mesh
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	vec3 v[4];
	float r=0.1;
	v[0]=vec3(r,r,r);
	v[1]=vec3(-r,-r,r);
	v[2]=vec3(-r,r,-r);
	v[3]=vec3(r,-r,-r);

	for(unsigned int i=0; i<4; ++i)
	{
		Vertex vert;
		vert.pos=v[i]+wpos;
		vert.norm=vec3(0,0,0);
		vert.texcoord=vec2(0,0);
		vertices.push_back(vert);
	}

	indices.push_back( 0 ); indices.push_back( 1 ); indices.push_back( 2 );
	indices.push_back( 0 ); indices.push_back( 1 ); indices.push_back( 3 );
	indices.push_back( 0 ); indices.push_back( 2 ); indices.push_back( 3 );
	indices.push_back( 1 ); indices.push_back( 2 ); indices.push_back( 3 );

	vec4 color = vec4(0.1,0.1,0.1,1);
	Mesh tmesh=Mesh(vertices, indices, textures, color);
	meshes.push_back(tmesh);
	int index=meshes.size()-1;

	box b;
	b.min=getmin(meshes[index]);
	b.max=getmax(meshes[index]);
	b.has_mesh=true;
	b.meshid=index;
	rootbox.subboxes.push_back(b);
}

Scene::Scene(){}


