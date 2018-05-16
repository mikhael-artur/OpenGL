#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

class Shader {
public:
  // Constructor, read and build the shader
  Shader(const char vertex_shader_path[], const char fragment_shader_path[]);

  // Destructor
  ~Shader();

  // Use the shader
  void use();

  // Utility uniform functions
  void setBool(const char name[], bool value) const;
  void setInt(const char name[], int value) const;
  void setFloat(const char name[], float value) const;
  void setVec2(const char name[], const glm::vec2& value) const;
  void setVec2(const char name[], float x, float y) const;
  void setVec3(const char name[], const glm::vec3& value) const;
  void setVec3(const char name[], float x, float y, float z) const;
  void setVec4(const char name[], const glm::vec4& value) const;
  void setVec4(const char name[], float x, float y, float z, float w) const;
  void setMat2(const char name[], const glm::mat2& mat) const;
  void setMat3(const char name[], const glm::mat3& mat) const;
  void setMat4(const char name[], const glm::mat4& mat) const;

  // Getter
  unsigned int getID() const;
  int getAttribLocation(const char name[]) const;

private:
  // The program ID
  unsigned int id_;
};

#endif
