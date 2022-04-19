#include "Mesh.h"
#include "VertexArray.h"

Mesh::Mesh():
	vertexArray{nullptr},
	shaderName{ "" },
	radius{ 0.0f }{}

Mesh::~Mesh(){}

void Mesh::Unload() {
	delete vertexArray;
	vertexArray = nullptr;
}

void Mesh::AddTexture(Texture* texture) {
	textures.emplace_back(texture);
}

Texture* Mesh::GetTexture(int index) {
	if (index < textures.size()) {
		return textures[index];
	}
	else {
		return nullptr;
	}
}

void Mesh::SetVertexArray(VertexArray* vertexArrayP) {
	vertexArray = vertexArrayP;
}

void Mesh::SetShaderName(const string& shaderNameP) {
	shaderName = shaderNameP;
}

void Mesh::SetRadius(float radiusP) {
	radius = radiusP;
}
