#pragma once

constexpr float vertices[] = {
	-0.5f, 0.5f, 0.f, 0.f, 0.f,
	 0.5f, 0.5f, 0.f, 1.f, 0.f,
	 0.5f,-0.5f, 0.f, 1.f, 1.f,
	-0.5f,-0.5f, 0.f, 0.f, 1.f
};

constexpr unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

class VertexArray {

public:
	VertexArray(const float* verticesP, unsigned int nbVerticesP, const unsigned int* indicesP, unsigned int nbIndicesP);
	~VertexArray();

	void setActive();

	unsigned int getNbVertices() const { return nbVertices; }
	unsigned int getNbIndices() const { return nbIndices; }

private:
	unsigned int nbVertices;
	unsigned int nbIndices;

	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
};
