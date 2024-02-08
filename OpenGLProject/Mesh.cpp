#include "Mesh.h"
#include "glad.h"
#include "assert.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <vector>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::InitialiseQuad()
{

	//Check that mesh is not initialised already
	assert(vao == 0);

	//Generate Buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//Bind vertex array -> mesh wrapper
	glBindVertexArray(vao);

	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	//Fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//Enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Quad has 2 triangles
	triCount = 2;

}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	//Check that mesh is not initialised already
	assert(vao == 0);

	//Generate Buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//Bind vertex array -> mesh wrapper
	glBindVertexArray(vao);

	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Fills our vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);




	//Enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);


	//Bind indicies if there are any
	if (indexCount != 0)
	{
		glGenBuffers(1, &ibo);

		//bind indice buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		//fill it
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int),
			indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else
	{
		triCount = vertexCount / 3;
	}


	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseFromFile(const char* fileName)
{
	//Read the verticies from the model
	const aiScene* scene = aiImportFile(fileName, 0);

	//Just use the first mesh we find
	aiMesh* mesh = scene->mMeshes[0];

	//Extract the indices from that first mesh
	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indicies;

	for (int i = 0; i < numFaces; ++i)
	{
		indicies.push_back(mesh->mFaces[i].mIndices[0]);
		indicies.push_back(mesh->mFaces[i].mIndices[2]);
		indicies.push_back(mesh->mFaces[i].mIndices[1]);

		//Generate a second triangle for our quads
		if (mesh->mFaces[i].mNumIndices == 4)
		{
			indicies.push_back(mesh->mFaces[i].mIndices[0]);
			indicies.push_back(mesh->mFaces[i].mIndices[3]);
			indicies.push_back(mesh->mFaces[i].mIndices[2]);
		}

	}

	//extract vertex data
	int numV = mesh->mNumVertices;
	Vertex* vertices = new Vertex[numV];

	for (int i = 0; i < numV; i++)
	{
		vertices[i].position = glm::vec4(mesh->mVertices[i].x,
			mesh->mVertices[i].y, mesh->mVertices[i].z, 1);

		//TODO Normals and UVS

	}


	Initialise(numV, vertices, indicies.size(), indicies.data());
	delete[] vertices;

}

void Mesh::Draw()
{
	glBindVertexArray(vao);

	//using indices or just vertices

	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
}
