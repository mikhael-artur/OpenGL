#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>

// Constructor, read and build the shader
Shader::Shader(const char vertex_shader_path[], const char fragment_shader_path[]) {
  // Read the vertex shader code from the file
  std::string vertex_shader_code;
  std::ifstream vertex_shader_file;
  vertex_shader_file.open(vertex_shader_path);
  if (vertex_shader_file.is_open()) {
    std::stringstream vertex_shader_stream;
    vertex_shader_stream << vertex_shader_file.rdbuf();
    vertex_shader_code = vertex_shader_stream.str();
    vertex_shader_file.close();
  } else {
    std::cout << "Failed to open " << vertex_shader_path << std::endl;
  }

  // Read the fragment shader code from the file
  std::string fragment_shader_code;
  std::ifstream fragment_shader_file;
  fragment_shader_file.open(fragment_shader_path);
  if (fragment_shader_file.is_open()) {
    std::stringstream fragment_shader_stream;
    fragment_shader_stream << fragment_shader_file.rdbuf();
    fragment_shader_code = fragment_shader_stream.str();
    fragment_shader_file.close();
  } else {
    std::cout << "Failed to open " << fragment_shader_path << std::endl;
  }

  // Compile shaders
  int success = 0;
  char info_log[512];

  // Compile vertex shader
  const char* vertex_shader_source = vertex_shader_code.c_str();
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  std::cout << "Compiling vertex shader: " << vertex_shader_path << std::endl;
  glCompileShader(vertex_shader);

  // Check vertex shader compile result
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << info_log << std::endl;
  }

  // Compile fragment shader
  const char* fragment_shader_source = fragment_shader_code.c_str();
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  std::cout << "Compiling fragment shader: " << fragment_shader_path << std::endl;
  glCompileShader(fragment_shader);

  // Check fragment shader compile result
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl << info_log << std::endl;
  }

  // Link the program
  id_ = glCreateProgram();
  glAttachShader(id_, vertex_shader);
  glAttachShader(id_, fragment_shader);
  std::cout << "Linking program..." << std::endl;
  glLinkProgram(id_);

  // Check the program compile result
  glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTES, &success);
  if (!success) {
    glGetProgramInfoLog(id_, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::LINK_FAILED" << std::endl << info_log << std::endl;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

// Destructor
Shader::~Shader() {
  glDeleteProgram(id_);
}

// Use the shader
void Shader::use() {
  glUseProgram(id_);
}

// Utility uniform functions
void Shader::setBool(const char name[], bool value) const {
  glUniform1i(glGetUniformLocation(id_, name), (int) value);
}

void Shader::setInt(const char name[], int value) const {
  glUniform1i(glGetUniformLocation(id_, name), value);
}

void Shader::setFloat(const char name[], float value) const {
  glUniform1f(glGetUniformLocation(id_, name), value);
}

void Shader::setVec2(const char name[], const glm::vec2& value) const {
  glUniform2fv(glGetUniformLocation(id_, name), 1, &value[0]);
}

void Shader::setVec2(const char name[], float x, float y) const {
  glUniform2f(glGetUniformLocation(id_, name), x, y);
}

void Shader::setVec3(const char name[], const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(id_, name), 1, &value[0]);
}

void Shader::setVec3(const char name[], float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}

void Shader::setVec4(const char name[], const glm::vec4& value) const {
  glUniform4fv(glGetUniformLocation(id_, name), 1, &value[0]);
}

void Shader::setVec4(const char name[], float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}

void Shader::setMat2(const char name[], const glm::mat2& mat) const {
  glUniformMatrix2fv(glGetUniformLocation(id_, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const char name[], const glm::mat3& mat) const {
  glUniformMatrix3fv(glGetUniformLocation(id_, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const char name[], const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, &mat[0][0]);
}

// Getter
unsigned int Shader::getID() const {
  return id_;
}

int Shader::getAttribLocation(const char name[]) const {
  return glGetAttribLocation(id_, name);
}
