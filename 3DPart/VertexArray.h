#pragma once

constexpr float vertices[] = {
	-.5f, .5f, 0.f, 0.f, 0.f, //top left
	.5f, .5f, 0.f, 1.f, 0.f, //top right
	.5f, -.5f, 0.f,  1.f, 1.f, //bottom right
	-.5f, -.5f, 0.f, 0.f, 1.f // bottom left
};

constexpr unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

class VertexArray
{
public:
	VertexArray(const float* verticesP, unsigned int nbVerticesP, const unsigned int* indicesP, unsigned int nbIndicesP);
	~VertexArray();

	void SetActive();

	unsigned int GetNbVertices() const { return nbVertices; }
	unsigned int GetNbIndices() const { return nbIndices; }

private:
	unsigned int nbVertices;
	unsigned int nbIndices;

	// OpenGL ID of the vertex array obj
	unsigned int vertexArray;
	// OpenGL ID of the vertex buffer
	unsigned int vertexBuffer;
	// OpenGL ID of the index buffer
	unsigned int indexBuffer;
};

