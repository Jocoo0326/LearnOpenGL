#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

// render meshes
// -------------
void Mesh::Draw(Shader shader)
{
  // set up textures
  GLuint diffuseNr = 1;
  GLuint specularNr = 1;
  GLuint normalNr = 1;
  GLuint heightNr = 1;
  for (size_t i = 0; i < textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse")
    {
      number = std::to_string(diffuseNr++);
    }
    else if (name == "texture_specular") {
      number = std::to_string(specularNr++);
    }
    else if (name == "texture_normal") {
      number = std::to_string(normalNr++);
    }
    else if (name == "texture_height") {
      number = std::to_string(heightNr++);
    }
    shader.setInt((name + number), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  // draw elements
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}

Mesh::~Mesh()
{
}

// prepare vertex data and bind vertex attribute pointer
// -----------------------------------------------------
void Mesh::setupMesh()
{
  // create arrays/buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind vao
  glBindVertexArray(VAO);
  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices, GL_STATIC_DRAW);
  // bind ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices, GL_STATIC_DRAW);
  // set up vertex attribute pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bittangent));
  glEnableVertexAttribArray(4);
  // unbind vao
  glBindVertexArray(0);
}
