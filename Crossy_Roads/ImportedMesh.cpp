#include "ImportedMesh.h"
#include <assimp\Importer.hpp>
#include <iostream>
#include <assimp/postprocess.h>
#include <fstream>
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

	for (unsigned int j = 0; j < nVertices; j++) {
		bbox[0] = min(bbox[0], vertices[j]);
		bbox[1] = max(bbox[1], vertices[j]);
	}

	generateAllbbPoints();
	height = bbox[1].y - bbox[0].y;
	center = (bbox[0] + bbox[1]) / 2.f;
}

bool ImportedMesh::initMaterials(const aiScene *pScene, const string &filename, string& texturePath) {
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
				texturePath.assign(fullPath);
				if (!texture.loadFromFile(fullPath, TEXTURE_PIXEL_FORMAT_RGB, false)) {
					cerr << "Error loading texture '" << fullPath << "'" << endl;
					return false;
				}
				texture.magFilter = GL_NEAREST;
				texture.minFilter = GL_NEAREST;
				texture.applyParams();
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

	vertices =  (vec3*)malloc(paiMesh->mNumVertices*sizeof(vec3));
	normals  =   (vec3*)malloc(paiMesh->mNumVertices * sizeof(vec3));
	texCoords = (vec2*)malloc(paiMesh->mNumVertices * sizeof(vec2));
	triangles = (uint*)malloc(paiMesh->mNumFaces * 3 * sizeof(uint));
	nModelVertices = paiMesh->mNumFaces * 3;
	nVertices = paiMesh->mNumVertices;

	// Init vertices properties
	for (uint i = 0; i < paiMesh->mNumVertices; i++) {
		pPos = &(paiMesh->mVertices[i]);
		pNormal = &(paiMesh->mNormals[i]);
		pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		vertices[i] = vec3(pPos->x, pPos->y, pPos->z);
		normals[i] = vec3(pNormal->x, pNormal->y, pNormal->z);
		texCoords[i] = vec2(pTexCoord->x, pTexCoord->y);
	}

	// Init faces indices
	for (uint i = 0, i3 = 0; i < paiMesh->mNumFaces; i++, i3 += 3) {
		const aiFace& Face = paiMesh->mFaces[i];
		triangles[i3] = Face.mIndices[0];
		triangles[i3 + 1] = Face.mIndices[1];
		triangles[i3 + 2] = Face.mIndices[2];
	}

	totalTriangles = paiMesh->mNumFaces;
}

void ImportedMesh::prepareArrays(ifstream& input) {
	unsigned int index;
	vec3 vertex, normal;
	vec2 texCoord;

	vector<float> vboVertices;
	vector<float> vboNormals;
	vector<float> vboTex;

	uint size;
	input.read((char*)&size, sizeof(uint));
	vboVertices.resize(size);
	input.read((char*)&vboVertices[0], sizeof(float)*size);
	
	input.read((char*)&size, sizeof(uint));
	vboNormals.resize(size);
	input.read((char*)&vboNormals[0], sizeof(float)*size);

	input.read((char*)&size, sizeof(uint));
	vboTex.resize(size);
	input.read((char*)&vboTex[0], sizeof(float)*size);

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
}

void ImportedMesh::prepareArrays(ofstream& output) {
	unsigned int index;
	vec3 vertex, normal;
	vec2 texCoord;

	vector<float> vboVertices;
	vector<float> vboNormals;
	vector<float> vboTex;

	vboVertices.resize(nModelVertices * 3);
	vboNormals.resize(nModelVertices * 3);
	vboTex.resize(nModelVertices * 2);

	for (unsigned int j = 0; j < nModelVertices; j++) {
		index = triangles[j];
		vertex = vertices[index];
		normal = normals[index];
		texCoord = texCoords[index];
		vboVertices[j * 3] = vertex.x; vboVertices[j * 3 + 1] = vertex.y; vboVertices[j * 3 + 2] = vertex.z;
		vboNormals[j * 3] = normal.x; vboNormals[j * 3 + 1] = normal.y; vboNormals[j * 3 + 2] = normal.z;
		vboTex[j * 2] = texCoord.x; vboTex[j * 2 + 1] = texCoord.y;
	}

	uint size = vboVertices.size();
	output.write((const char*)&size, sizeof(uint));
	output.write((const char*)&vboVertices[0], sizeof(float)*size);
	size = vboNormals.size();
	output.write((const char*)&size, sizeof(uint));
	output.write((const char*)&vboNormals[0], sizeof(float)*size);
	size = vboTex.size();
	output.write((const char*)&size, sizeof(uint));
	output.write((const char*)&vboTex[0], sizeof(float)*size);
	
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
}

bool ImportedMesh::loadFromFile(const string& fileName) {
	ofstream output(fileName + ".objBin", ofstream::binary);
	renderMode = GL_TRIANGLES;
	bool retCode = false;
	Assimp::Importer Importer;
	const aiScene *pScene;

	string fullFileName = fileName + ".obj";
	pScene = Importer.ReadFile(fullFileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (!pScene) {
		cerr << "Error parsing '" << fullFileName << "': '" << Importer.GetErrorString() << endl;
		return false;
	}

	const aiMesh* paiMesh = pScene->mMeshes[0];
	initMesh(paiMesh);

	string texturePath;
	if (!initMaterials(pScene, fullFileName, texturePath))
		return false;

	output.write((const char*)&nVertices, sizeof(uint));
	output.write((const char*)&totalTriangles, sizeof(uint));

	uint length = texturePath.length();
	output.write((const char*)&length, sizeof(uint));
	output.write(texturePath.c_str(), length);

	computeBoundingBox();

	output.write((const char*)bbox, sizeof(bbox));
	output.write((const char*)&height, sizeof(height));
	output.write((const char*)&center, sizeof(center));

	prepareArrays(output);

	free(vertices);
	free(normals);
	free(texCoords);
	free(triangles);

	output.close();

	return true;
}

bool ImportedMesh::loadFromBinary(const string & filename) {
	if (filename == "models/cubierta_2_open") {
		int a = 3;
	}
	ifstream input(filename + ".objBin", ifstream::binary);

	input.read((char*)&nVertices, sizeof(uint));
	input.read((char*)&totalTriangles, sizeof(uint));

	uint length;
	input.read((char*)&length, sizeof(uint));
	char* c_std = new char[length];
	input.read(c_std, length);
	string fullPath(c_std, length);

	if (!texture.loadFromFile(fullPath, TEXTURE_PIXEL_FORMAT_RGB, false)) {
		cerr << "Error loading texture '" << fullPath << "'" << endl;
		return false;
	}
	texture.magFilter = GL_NEAREST;
	texture.minFilter = GL_NEAREST;
	texture.applyParams();

	input.read((char*)bbox, sizeof(bbox));
	input.read((char*)&height, sizeof(height));
	input.read((char*)&center, sizeof(center));
	
	prepareArrays(input);

	input.close();

	return false;
}

void ImportedMesh::useTexture() const {
	texture.use();
}