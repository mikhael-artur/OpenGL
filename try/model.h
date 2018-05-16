#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include <assimp/scene.h>
#include <string>
#include <vector>
using namespace std;

class Model {
public:
  // Constructor, expects a filepath to a 3D model
  Model(const string& path);

  // Render the model, and thus all its meshes
  void render(Shader& shader) const;

private:
  // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
  void loadModel(const string& path);

  // Processes a node in a recursive fashion
  // Processes each individual mesh located at the node and repeats this process on its children nodes (if any)
  void processNode(aiNode *node, const aiScene *scene);

  // Processes mesh
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  // Checks all material textures of a given type and loads the textures if they're not loaded yet
  vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string type_name);

  // Model Data
  vector<Texture> textures_loaded_;
  vector<Mesh> meshes_;
  string directory_;
};

#endif
