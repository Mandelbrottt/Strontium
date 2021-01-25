// Include guard.
#pragma once

// OpenGL includes.
#include <GL/glew.h>
#include <glm/glm.hpp>

// STL includes.
#include <vector>
#include <string>

// Vertex datatype to store vertex attributes.
struct Vertex
{
  glm::vec4 position;
  glm::vec3 normal;
};

class Mesh
{
public:
  // Constructors.
  Mesh();
  Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
  // Destructor.
  ~Mesh() = default;
  // Load data from a file.
  void loadOBJFile(const char* filepath);
  // Compute vertex and surface normals.
  void computeNormals();
  // Normalize the vertices to the screenspace (-1 -> 1).
  void normalizeVertices();
  // Normalize by a scaling float.
  void normalizeVertices(GLfloat scale);
  // Apply a matrix transform to the mesh.
  void applyTransform(glm::mat4 transform);
  // Debug function to dump to the console.
  void dumpMeshData();

  // Getters.
  std::vector<Vertex>     getData();
  std::vector<GLuint>     getIndices();
  std::vector<glm::vec3>  getTriNormals();
  std::string             getMaterial();

  bool                    isLoaded();
protected:
  // Mesh properties.
  bool loaded;
  std::vector<Vertex>     data;
  std::vector<GLuint>     indices;
  std::vector<glm::vec3>  triangleNormals;
  std::string             material;
};

// Load an obj file into an Mesh class.
Mesh* loadOBJFile(const char* filepath);

// Mesh loader.
std::vector<Mesh*> meshLoader(const char* filepath);

// Forward declaration of the max/min helper functions.
GLfloat vertexMin(std::vector<Vertex> vector, unsigned start,
                  unsigned end, unsigned axis);
GLfloat vertexMax(std::vector<Vertex> vector, unsigned start,
                  unsigned end, unsigned axis);