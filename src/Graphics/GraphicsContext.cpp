#include "Graphics/GraphicsContext.h"

namespace SciRenderer
{
  // Generic constructor / destructor pair.
  GraphicsContext::GraphicsContext(GLFWwindow* genWindow)
    : glfwWindowRef(genWindow)
  {
    if (this->glfwWindowRef == nullptr)
    {
      std::cout << "Window was nullptr, aborting." << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  GraphicsContext::~GraphicsContext()
  { }

  void
  GraphicsContext::init()
  {
    glfwMakeContextCurrent(this->glfwWindowRef);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
      std::cout << "Failed to load a graphics context!" << std::endl;
      exit(EXIT_FAILURE);
    }

    // Print info about the vendor to the console (for now).
    std::cout << "Graphics device vendor: " << glGetString(GL_VENDOR)
              << std::endl;
    std::cout << "Graphics device: " << glGetString(GL_RENDERER)
              << std::endl;
    std::cout << "Graphics context version: " << glGetString(GL_VERSION)
              << std::endl;
    this->contextInfo = std::string("Graphics device vendor: ") +
                        std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))) +
                        std::string("\nGraphics device: ") +
                        std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))) +
                        std::string("\nGraphics context version: ") +
                        std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  }

  void
  GraphicsContext::swapBuffers()
  {
    glfwSwapBuffers(this->glfwWindowRef);
  }
}