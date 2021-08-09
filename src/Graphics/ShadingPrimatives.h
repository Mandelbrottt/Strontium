#pragma once

namespace Strontium
{
  struct DirectionalLight
  {
    glm::vec3 direction;
    glm::vec3 colour;
    GLfloat intensity;
    bool castShadows;
    bool primaryLight;

    DirectionalLight()
      : direction(glm::vec3(0.0f, 1.0f, 0.0f))
      , colour(glm::vec3(1.0f))
      , intensity(0.0f)
      , castShadows(false)
      , primaryLight(false)
    { }
  };

  struct PointLight
  {
    glm::vec3 position;
    glm::vec3 colour;
    GLfloat intensity;
    GLfloat radius;
    GLfloat falloff;
    bool castShadows;

    PointLight()
      : position(glm::vec3(0.0f))
      , colour(glm::vec3(1.0f))
      , intensity(0.0f)
      , radius(0.0f)
      , falloff(0.0f)
      , castShadows(false)
    { }
  };

  struct SpotLight
  {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 colour;
    GLfloat intensity;
    GLfloat innerCutoff;
    GLfloat outerCutoff;
    GLfloat radius;
    bool castShadows;

    SpotLight()
      : position(glm::vec3(0.0f))
      , direction(glm::vec3(0.0f, 1.0f, 0.0f))
      , colour(glm::vec3(1.0f))
      , intensity(0.0f)
      , innerCutoff(std::cos(glm::radians(45.0f)))
      , outerCutoff(std::cos(glm::radians(90.0f)))
      , radius(0.0f)
      , castShadows(false)
    { }
  };
}
