#include "Application.h"
#include <iostream>
#include "Gizmos.h"

bool Application::Startup()
{
    //Initialise glfw
    if (glfwInit() == false)
    {
        return false;
    }

    //Create window
    window = glfwCreateWindow(windowWidth, windowHeight, "Application Title", nullptr, nullptr);

    //Ensure window was created
    if (window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    //Make window our current context
    glfwMakeContextCurrent(window);


    if (!gladLoadGL())
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);


    aie::Gizmos::create(10000, 10000, 0, 0);
    view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
    projection = glm::perspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.0f);


    shader.loadShader(aie::eShaderStage::VERTEX, "./Shader/Vertex/simple.vert");
    shader.loadShader(aie::eShaderStage::FRAGMENT, "./Shader/Fragment/simple.frag");

    if (shader.link() == false)
    {
        printf("Shader Error: %s\n", shader.getLastError());
        return false;
    }

    vertexOnlyMesh.InitialiseQuad();

    Mesh::Vertex vertices[4];
    vertices[0].position = { -0.5f, 0, 0.5f, 1 };
    vertices[1].position = { 0.5f, 0, 0.5f, 1 };
    vertices[2].position = { -0.5f, 0, -0.5f, 1 };
    vertices[3].position = { 0.5f, 0, -0.5f, 1 };

    unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

    // quad transform - make it 10 units wide and high
    quadTransform = { 10, 0, 0, 0,
                      0, 10, 0, 0,
                      0, 0, 10, 0,
                      0, 0, 0, 1 };
    
    bunnyMesh.InitialiseFromFile("./Models/Bunny.obj");

    bunnyTransform= 
    { .5,0,0,0,
       0,.5,0,0,
       0,0,.5,0,
       0,0,0,1 };

    return true;
}



bool Application::Update()
{
    camera.Update(0.01f, window);

    if (glfwWindowShouldClose(window) == true || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        return false;
    }
    return true;
}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 pv = camera.GetProjectionMatrix(windowWidth, windowHeight)
        * camera.GetViewMatrix();




    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1));

    glm::vec4 white(1);
    glm::vec4 black(0, 0, 0, 1);

    for (int i = 0; i < 21; i++)
    {
        aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
        aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);


    }


    aie::Gizmos::draw(pv);

    //bind shader
    shader.bind();

    //bind transform
    auto pvm = pv * quadTransform;
    shader.bindUniform("ProjectionViewModel", pvm);

    //draw quad
    vertexOnlyMesh.Draw();

    pvm = pv * bunnyTransform;
    shader.bindUniform("ProjectionViewModel", pvm);
    bunnyMesh.Draw();

    glfwSwapBuffers(window);
    glfwPollEvents();


}

void Application::Shutdown()
{
    aie::Gizmos::destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
}
