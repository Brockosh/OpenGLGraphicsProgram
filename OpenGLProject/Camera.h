#pragma once
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
class Camera
{
private:
	float theta;
	float phi;
	glm::vec3 position;



public:
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);


	void Update(float dt, GLFWwindow* window);
};

