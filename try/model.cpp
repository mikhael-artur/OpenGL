#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
using namespace std;

// Constructor, expects a filepath to a 3D model
Model::Model(const string& path) {
  loadModel(path);
}

// Render the model, and thus all its meshes
void Model::render(Shader& shader) const {
  for (unsigned int i = 0; i < meshes_.size(); i++) {
    meshes_[i].render(shader);
  }
}

// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
void Model::loadModel(const string& path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
    return;
  }

  directory_ = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

// Processes a node in a recursive fashion
// Processes each individual mesh located at the node and repeats this process on its children nodes (if any)
void Model::processNode(aiNode *node, const aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(processMesh(mesh, scene));
  }

  // do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

// Processes mesh
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;
  glm::vec3 diffuse_color;
  glm::vec3 ambient_strength;
  glm::vec3 specular_strength;

  // process vertex positions, normals and texture coordinates
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.normal = vector;
    }

    // does the mesh contain texture coordinates?
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.texture_coordinate = vec;
    } else {
      vertex.texture_coordinate = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // process material
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    diffuse_color = glm::vec3(color.r, color.g, color.b);

    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    ambient_strength = glm::vec3(color.r, color.g, color.b);

    material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    specular_strength = glm::vec3(color.r, color.g, color.b);

    vector<Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    vector<Texture> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  }
  return Mesh(vertices, indices, textures, diffuse_color, ambient_strength, specular_strength);
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet
vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, string type_name) {
  vector<Texture> textures;
  for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;
    material->GetTexture(type, i, &str);
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded_.size(); j++) {
      if (strcmp(textures_loaded_[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(textures_loaded_[j]);
        skip = true;
        break;
      }
    }

    // if texture hasn't been loaded already, load it
    if (!skip) {
      Texture texture;
      texture.id = textureFromFile(str.C_Str(), directory_);
      texture.type = type_name;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded_.push_back(texture);
    }
  }

  return textures;
}
