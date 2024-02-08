#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"


class Application
{



public:
	bool Startup();
	bool Update();
	void Draw();
	void Shutdown();

protected:
	const int windowWidth = 1920;
	const int windowHeight = 1080;

	glm::mat4 view;
	glm::mat4 projection;

	GLFWwindow* window;
	aie::ShaderProgram shader;
	Mesh vertexOnlyMesh;
	Mesh indiciesMesh;

	Mesh bunnyMesh;

	glm::mat4 quadTransform;
	glm::mat4 indiciesQuadTransform;
	glm::mat4 bunnyTransform;

	Camera camera;
};

