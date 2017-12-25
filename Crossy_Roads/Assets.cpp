#include "Assets.h"
#include <document.h>
#include <error/en.h>
#include <fstream>
#include <iostream>
using namespace std;
using namespace glm;
using namespace rapidjson;
typedef map<string, int>::const_iterator Iterator;

const CubeMesh * Assets::getCubeMesh() const {
	return &cubeMesh;
}

int Assets::getMeshId(const string & name) const {
	Iterator it = meshIds.find(name);
	if (it == meshIds.end()) {
		int a = 3;
	}
	return it == meshIds.end() ? INVALID : it->second;
}

int Assets::getTextureId(const string & name) const {
	Iterator it = textureIds.find(name);
	if (it == textureIds.end()) {
		int a = 3;
	}
	return it == textureIds.end() ? INVALID : it->second;
}

const ImportedMesh * Assets::getMesh(IdMesh id) const {
	return &meshes[id];
}

const Texture* Assets::getTexture(IdTex id) const {
	return &textures[id];
}

const Texture* Assets::getTexture(const string& name) const {
	return &textures[textureIds.find(name)->second];
}

const std::vector<IdMesh>* Assets::getGroups() const {
	return groups;
}

const std::vector<MeshConfig>* Assets::getDecoration() const {
	return &decorationGroup;
}

uint Assets::getNumMeshes() const {
	return nImportedMeshes;
}

uint Assets::getNumTextures() const {
	return nTextures;
}

void Assets::loadAssets(const string& assetsFile) {
	ifstream jsonFile(assetsFile);
	string s((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());
	jsonFile.close();

	Document document;
	document.Parse(s.c_str());
	s.clear();
	if (document.HasParseError()) {
		fprintf(stderr, "\nError(offset %u): %s\n",
			(unsigned)document.GetErrorOffset(),
			GetParseError_En(document.GetParseError()));
	}

	const Value& models = document["models"];
	nImportedMeshes = models.Size();

	for (uint i = 0; i < nGroups-2; ++i) {
		groups[i].reserve(nImportedMeshes);
	}
	decorationGroup.reserve(nImportedMeshes);

	cubeMesh.init();
	meshes = new ImportedMesh[nImportedMeshes];

	string name, type;
	uint i = 0;
	for (const Value& meshProperties : models.GetArray()) {
		name.assign(meshProperties["names"][0].GetString());
		type.assign(meshProperties["type"].GetString());
		meshes[i].loadFromFile("models/" + name + ".obj");
		meshIds[name] = i;
		if (type == "decoration") {
			const Value& size = meshProperties["size"];
			MeshConfig aux;
			aux.rows = size[0].GetUint();
			aux.cols = size[1].GetUint();
			aux.height = meshProperties["height"].GetFloat();
			aux.mesh = i;
			decorationGroup.push_back(aux);
		}
		else if (type == "enemy") {
			groups[Enemy].push_back(i);
		}
		else if (type == "platform") {
			groups[Platform].push_back(i);
		}
		else if (type != "unique") {
			int a = 3;
		}
		++i;
	}

	for (uint i = 0; i < nGroups - 2; ++i) {
		groups[i].shrink_to_fit();
	}
	decorationGroup.shrink_to_fit();

	const Value& texturesJ = document["textures"];
	nTextures = texturesJ.Size();

	textures = new Texture[nTextures];
	i = 0;
	for (const Value& texture : texturesJ.GetArray()) {
		name.assign(texture["name"].GetString());
		type.assign(texture["type"].GetString());
		if (type == "linear") {
			textures[i].loadFromFile("images/" + name + ".png", TEXTURE_PIXEL_FORMAT_RGBA, true);
		}
		else {
			textures[i].loadFromFile("images/" + name + ".png", TEXTURE_PIXEL_FORMAT_RGBA, false);
			textures[i].setMagFilter(GL_NEAREST);
			textures[i].setMinFilter(GL_NEAREST);
		}
		textures[i].applyParams();
		textureIds[name] = i;
		++i;
	}
}

Assets::Assets() : meshes(NULL), textures(NULL) {
}


Assets::~Assets() {
	if (meshes != NULL) {
		delete meshes;
		delete textures;
	}
}
