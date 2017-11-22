#include "ImportedMesh.h"
#include <assimp\Importer.hpp>
#include <iostream>
#include <assimp/postprocess.h>
using namespace glm;

ImportedMesh::ImportedMesh()
{
}


ImportedMesh::~ImportedMesh()
{
}

void ImportedMesh::computeBoundingBox() {
	bbox[0] = vec3(1e10f, 1e10f, 1e10f);
	bbox[1] = vec3(-1e10f, -1e10f, -1e10f);

	for (unsigned int j = 0; j < vertices.size(); j++) {
		bbox[0] = min(bbox[0], vertices[j]);
		bbox[1] = max(bbox[1], vertices[j]);
	}
	height = bbox[1].y - bbox[0].y;
	center = (bbox[0] + bbox[1]) / 2.f;
}

bool ImportedMesh::initMaterials(const aiScene *pScene, const string &filename) {
	bool retCode = true;
	const aiMaterial* pMaterial;
	string::size_type SlashIndex = filename.find_last_of("/");
	string dir;

	// Extract the directory part from the file name
	if (SlashIndex == string::npos)
		dir = ".";
	else if (SlashIndex == 0)
		dir = "/";
	else
		dir = filename.substr(0, SlashIndex);
	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		// Initialize the material
		pMaterial = pScene->mMaterials[i];

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				string sFullPath = dir + "/" + Path.data;
				char fullPath[200];
				strcpy_s(fullPath, sFullPath.c_str());

				if (!texture.loadFromFile(fullPath, TEXTURE_PIXEL_FORMAT_RGB)) {
					cerr << "Error loading texture '" << fullPath << "'" << endl;
					return false;
				}
				break;
			}
		}
	}

	return true;
}

void ImportedMesh::initMesh(const aiMesh *paiMesh) {
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiVector3D* pPos;
	const aiVector3D* pNormal;
	const aiVector3D* pTexCoord;

	vertices.reserve(paiMesh->mNumVertices * 3);
	normals.reserve(paiMesh->mNumVertices * 3);
	texCoords.reserve(paiMesh->mNumVertices * 2);
	triangles.reserve(paiMesh->mNumFaces * 3);

	// Init vertices properties
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		pPos = &(paiMesh->mVertices[i]);
		pNormal = &(paiMesh->mNormals[i]);
		pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		vertices.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	// Init faces indices
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		triangles.push_back(Face.mIndices[0]);
		triangles.push_back(Face.mIndices[1]);
		triangles.push_back(Face.mIndices[2]);
	}
}

void ImportedMesh::prepareArrays() {
	unsigned int index;
	vec3 vertex, normal;
	vec2 texCoord;

	vector<float> vboVertices(triangles.size() * 3);
	vector<float> vboNormals(triangles.size() * 3);
	vector<float> vboTex(triangles.size() * 2);

	for (unsigned int j = 0; j < triangles.size(); j++) {
		index = triangles[j];
		vertex = vertices[index];
		normal = normals[index];
		texCoord = texCoords[index];
		vboVertices[j * 3] = vertex.x; vboVertices[j * 3 + 1] = vertex.y; vboVertices[j * 3 + 2] = vertex.z;
		vboNormals[j * 3] = normal.x; vboNormals[j * 3 + 1] = normal.y; vboNormals[j * 3 + 2] = normal.z;
		vboTex[j * 2] = texCoord.x; vboTex[j * 2 + 1] = texCoord.y;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBOvert);
	glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
	glBufferData(GL_ARRAY_BUFFER, vboVertices.size() * sizeof(float), &vboVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBOnorm);
	glBindBuffer(GL_ARRAY_BUFFER, VBOnorm);
	glBufferData(GL_ARRAY_BUFFER, vboNormals.size() * sizeof(float), &vboNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBOtex);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtex);
	glBufferData(GL_ARRAY_BUFFER, vboTex.size() * sizeof(float), &vboTex[0], GL_STATIC_DRAW);

	nVertices = triangles.size();
}

bool ImportedMesh::loadFromFile(const string &filename) {
	renderMode = GL_TRIANGLES;
	bool retCode = false;
	Assimp::Importer Importer;
	const aiScene *pScene;

	pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (!pScene) {
		cerr << "Error parsing '" << filename << "': '" << Importer.GetErrorString() << endl;
		return false;
	}

	const aiMesh* paiMesh = pScene->mMeshes[0];
	initMesh(paiMesh);

	if (!initMaterials(pScene, filename))
		return false;

	computeBoundingBox();
	prepareArrays();

	vertices.clear();
	normals.clear();
	triangles.clear();

	return true;
}

void ImportedMesh::render(ShaderProgram & shaderProgram) const {
	texture.use();
	Mesh::render(shaderProgram);
}
