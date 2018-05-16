#include "mesh.h"
#include "stb_image.h"
#include <GL/glew.h>
#include <iostream>
// Constructor
Mesh::Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures, const glm::vec3& diffuse_color, const glm::vec3& ambient_strength, const glm::vec3& specular_strength) {
  vertices_ = vertices;
  indices_ = indices;
  textures_ = textures;
  diffuse_color_ = diffuse_color;
  ambient_strength_ = ambient_strength;
  specular_strength_ = specular_strength;

  // Set the vertex buffers and its attribute pointers
  setupMesh();
}

// Render the mesh
void Mesh::render(Shader& shader) const {
  // bind appropriate textures
  unsigned int diffuse_number  = 1;
  unsigned int specular_number = 1;

  for (unsigned int i = 0; i < textures_.size(); i++) {
    // active proper texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    // retrieve texture number (the N in diffuse_textureN)
    string number;
    string name = textures_[i].type;
    if (name == "texture_diffuse") {
      number = to_string(diffuse_number++);
    } else if (name == "texture_specular") {
      number = to_string(specular_number++);
    }

  	// set the sampler to the correct texture unit
    shader.setInt((name + number).c_str(), i);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  shader.setVec3("diffuse_color", diffuse_color_);
  shader.setVec3("ambient_strength", ambient_strength_);
  shader.setVec3("specular_strength", specular_strength_);
  // render mesh
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}

// Initializes all the buffer objects/arrays
void Mesh::setupMesh() {
  // create buffers/arrays
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  glBindVertexArray(VAO_);

  // load data into vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

  // set the vertex attribute pointers
  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  // vertex texture coordinate
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate));

  glBindVertexArray(0);
}

// Load a texture from file
unsigned int textureFromFile(const char *path, const string &directory) {
  string filename = string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, num_of_components;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &num_of_components, 0);
  if (data) {
    GLenum format;
    if (num_of_components == 1) {
      format = GL_RED;
    } else if (num_of_components == 2) {
      format = GL_RG;
    } else if (num_of_components == 3) {
      format = GL_RGB;
    } else if (num_of_components == 4) {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    cout << "Texture failed to load at path: " << path << endl;
    stbi_image_free(data);
  }

  return textureID;
}
