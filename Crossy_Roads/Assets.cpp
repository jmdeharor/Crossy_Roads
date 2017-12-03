#include "Assets.h"
using namespace std;
using namespace glm;
typedef map<string, int>::const_iterator Iterator;

const CubeMesh * Assets::getCubeMesh() const {
	return &cubeMesh;
}

int Assets::getMeshId(const string & name) const {
	Iterator it = meshIds.find(name);
	return it == meshIds.end() ? INVALID : it->second;
}

int Assets::getTextureId(const string & name) const {
	Iterator it = textureIds.find(name);
	return it == textureIds.end() ? INVALID : it->second;
}

const ImportedMesh * Assets::getMesh(IdMesh id) const {
	return &meshes[id];
}

const Texture* Assets::getTexture(IdTex id) const {
	return &textures[id];
}

void Assets::loadAssets(const string * meshNames, const string * textureNames,
						const TextureFilter* textureFilters, uint nMeshes, uint nTextures) {
	cubeMesh.init();
	nImportedMeshes = nMeshes;
	meshes = new ImportedMesh[nMeshes];
	for (uint i = 0; i < nMeshes; ++i) {
		meshes[i].loadFromFile("models/" + meshNames[i] + ".obj");
		meshIds[meshNames[i]] = i;
	}

	textures = new Texture[nTextures];
	for (uint i = 0; i < nTextures; ++i) {
		switch (textureFilters[i]) {
		case LINEAR:
			textures[i].loadFromFile("images/" + textureNames[i] + ".png", TEXTURE_PIXEL_FORMAT_RGBA, true);
			break;
		case NEAREST:
			textures[i].loadFromFile("images/" + textureNames[i] + ".png", TEXTURE_PIXEL_FORMAT_RGBA, false);
			textures[i].setMagFilter(GL_NEAREST);
			textures[i].setMinFilter(GL_NEAREST);
			break;
		}
		textures[i].applyParams();
		textureIds[textureNames[i]] = i;
	}
}

Assets::Assets()
{
}


Assets::~Assets()
{
}
