#include "Renderer.h"

Renderer* Renderer::instance = nullptr;

// Get the renderer instance.
Renderer*
Renderer::getInstance()
{
  if (instance == nullptr)
  {
    instance = new Renderer();
    return instance;
  }
  else
    return instance;
}

// Initialize the renderer.
void
Renderer::init(GLenum mode)
{
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, mode);
}

// Draw the data to the screen.
void
Renderer::draw(GLFWwindow* window, VertexArray* data, Shader* program)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  data->bind();
  program->bind();

  glDrawElements(GL_TRIANGLES, data->numToRender(), GL_UNSIGNED_INT, nullptr);

  data->unbind();
  program->unbind();
  glfwSwapBuffers(window);
}
