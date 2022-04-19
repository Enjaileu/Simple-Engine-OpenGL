#pragma once
#include <string>
#include <vector>
#include "Texture.h"

class Mesh
{
public :
	Mesh();
	~Mesh();

	void Unload();

	class VertexArray* GetVertexArray() { return vertexArray; }
	const string& GetShaderName() const { return shaderName; }
	float GetRadius() const { return radius; }

	void AddTexture(Texture* texture);
	Texture* GetTexture(int index);

	void SetVertexArray(VertexArray* vertexArrayP);
	void SetShaderName(const string& shaderNameP);
	void SetRadius(float radiusP);

private:
	std::vector<Texture*> textures;
	class VertexArray* vertexArray;
	string shaderName;
	float radius; // bounding sphere radius
};

