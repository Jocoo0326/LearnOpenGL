#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include <string>
#include <vector>

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bittangent;
};

struct Texture
{
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh
{
public:
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned int> indices;
  unsigned int VAO;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
  void Draw(Shader shader);
  ~Mesh();

private:
  unsigned int VBO, EBO;
  void setupMesh();
};

#endif // !MESH_H