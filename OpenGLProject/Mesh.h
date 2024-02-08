#pragma once
#include "glm/glm.hpp"
class Mesh
{



public:
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
	};
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	void InitialiseQuad();
	void Initialise(unsigned int vertexCount, const Vertex* vertices, 
		            unsigned int indexCount = 0, unsigned int* indices = nullptr);;
	void  InitialiseFromFile(const char* fileName);
	virtual void Draw();

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;


};

