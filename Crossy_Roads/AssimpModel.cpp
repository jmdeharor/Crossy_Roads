#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AssimpModel.h"


AssimpModel::AssimpModel()
{
}

AssimpModel::~AssimpModel()
{
	clear();
}


bool AssimpModel::loadFromFile(const string &filename, ShaderProgram &program)
{
	bool retCode = false;
	Assimp::Importer Importer;
	const aiScene *pScene;

	clear();
	pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if(pScene)
		retCode = initFromScene(pScene, filename);
	else
		cerr << "Error parsing '" << filename << "': '" << Importer.GetErrorString() << std::endl;
	computeBoundingBox();
	prepareArrays(program);

	return retCode;
}

float AssimpModel::getHeight() const
{
	return height;
}

glm::vec3 AssimpModel::getCenter() const
{
	return center;
}


void AssimpModel::render(ShaderProgram &program) const
{
	unsigned int index;

	program.use();
	for (index = 0; index < meshes.size(); index++)
	{
		if (textures[meshes[index]->textureIndex] != NULL)
		{
			glEnable(GL_TEXTURE_2D);
			textures[meshes[index]->textureIndex]->use();
		}
		else
			glDisable(GL_TEXTURE_2D);
		glBindVertexArray(VAOs[index]);
		glEnableVertexAttribArray(posLocations[index]);
		glEnableVertexAttribArray(normalLocations[index]);
		glEnableVertexAttribArray(texCoordLocations[index]);
		glDrawArrays(GL_TRIANGLES, 0, meshes[index]->triangles.size());
	}
	glDisable(GL_TEXTURE_2D);
}

void AssimpModel::clear()
{
	for(vector<Mesh *>::iterator itMesh = meshes.begin(); itMesh != meshes.end(); itMesh++)
		delete *itMesh;
	meshes.clear();
	for (vector<Texture *>::iterator itTexture = textures.begin(); itTexture != textures.end(); itTexture++)
		delete *itTexture;
	textures.clear();
}

bool AssimpModel::initFromScene(const aiScene *pScene, const string &filename)
{
	meshes.resize(pScene->mNumMeshes);
	textures.resize(pScene->mNumMaterials);

	for (unsigned int i = 0; i<meshes.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initMesh(i, paiMesh);
	}

	return initMaterials(pScene, filename);
}

void AssimpModel::initMesh(int index, const aiMesh *paiMesh)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiVector3D* pPos;
	const aiVector3D* pNormal;
	const aiVector3D* pTexCoord;

	meshes[index] = new Mesh();
	meshes[index]->textureIndex = paiMesh->mMaterialIndex;

	// Init vertices properties
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		pPos = &(paiMesh->mVertices[i]);
		pNormal = &(paiMesh->mNormals[i]);
		pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		meshes[index]->vertices.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		meshes[index]->normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		meshes[index]->texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	// Init faces indices
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		meshes[index]->triangles.push_back(Face.mIndices[0]);
		meshes[index]->triangles.push_back(Face.mIndices[1]);
		meshes[index]->triangles.push_back(Face.mIndices[2]);
	}
}

bool AssimpModel::initMaterials(const aiScene *pScene, const string &filename)
{
	bool retCode = true;
	const aiMaterial* pMaterial;
	std::string::size_type SlashIndex = filename.find_last_of("/");
	std::string dir;

	// Extract the directory part from the file name
	if (SlashIndex == std::string::npos)
		dir = ".";
	else if (SlashIndex == 0)
		dir = "/";
	else
		dir = filename.substr(0, SlashIndex);

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		textures[i] = NULL;
		pMaterial = pScene->mMaterials[i];

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string sFullPath = dir + "/" + Path.data;
				char fullPath[200];
				strcpy_s(fullPath, sFullPath.c_str());

				textures[i] = new Texture();
				if (!textures[i]->loadFromFile(fullPath, TEXTURE_PIXEL_FORMAT_RGB))
				{
					cerr << "Error loading texture '" << fullPath << "'" << endl;
					delete textures[i];
					textures[i] = NULL;
					retCode = false;
				}
			}
		}
	}

	return retCode;
}

void AssimpModel::computeBoundingBox()
{
	bbox[0] = glm::vec3(1e10f, 1e10f, 1e10f);
	bbox[1] = glm::vec3(-1e10f, -1e10f, -1e10f);
	for (unsigned int i = 0; i<meshes.size(); i++)
	{
		for (unsigned int j = 0; j<meshes[i]->vertices.size(); j++)
		{
			bbox[0] = glm::min(bbox[0], meshes[i]->vertices[j]);
			bbox[1] = glm::max(bbox[1], meshes[i]->vertices[j]);
		}
	}
	height = bbox[1].y - bbox[0].y;
	center = (bbox[0] + bbox[1]) / 2.f;
}

void AssimpModel::prepareArrays(ShaderProgram &program)
{
	unsigned int index;
	glm::vec3 vertex, normal;
	glm::vec2 texCoord;
	vector<float> vertices;
	GLuint vao, vbo;
	GLint posLocation, normalLocation, texCoordLocation;

	for (unsigned int i = 0; i<meshes.size(); i++)
	{
		// Array for submesh i
		for (unsigned int j = 0; j<meshes[i]->triangles.size(); j++)
		{
			index = meshes[i]->triangles[j];
			vertex = meshes[i]->vertices[index];
			normal = meshes[i]->normals[index];
			texCoord = meshes[i]->texCoords[index];
			vertices.push_back(vertex.x); vertices.push_back(vertex.y); vertices.push_back(vertex.z);
			vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
			vertices.push_back(texCoord.s); vertices.push_back(texCoord.t);
		}

		glGenVertexArrays(1, &vao);
		VAOs.push_back(vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		VBOs.push_back(vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * meshes[i]->triangles.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		posLocation = program.bindVertexAttribute("position", 3, 8 * sizeof(float), 0);
		posLocations.push_back(posLocation);
		normalLocation = program.bindVertexAttribute("normal", 3, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		normalLocations.push_back(normalLocation);
		texCoordLocation = program.bindVertexAttribute("texCoord", 2, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		texCoordLocations.push_back(texCoordLocation);
	}
}

