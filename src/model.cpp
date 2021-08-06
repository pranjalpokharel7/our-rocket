#include "../include/model.hpp"
#include "../include/stb_image.h"

Model::Model(){};
Model::Model(const char* path){
    load_model(std::string(path));
}

void Model::draw_model(unsigned int program_ID) {
    for (unsigned int i = 0; i < meshes.size(); i++){
        meshes[i].draw_mesh(program_ID);
    }
}

void Model::load_model(std::string path) {
  // use trianles as primitives, flip texture coordinates to make it opengl
  // oriented
  Assimp::Importer importer;
  auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "ERROR - ASSIMP::" << importer.GetErrorString() << std::endl;
  }
  directory = path.substr(0, path.find_last_of('/'));
  process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene) {
  for (unsigned int mesh_index = 0; mesh_index < node->mNumMeshes; mesh_index++) {
    // only the root scene mMeshes array contains the actual mesh data
    // the other children nodes only contain the index for the mesh in the root
    // array
    aiMesh *mesh = scene->mMeshes[node->mMeshes[mesh_index]];
    meshes.push_back(process_mesh(mesh, scene, mesh_index));
  }
  // then continue the process for it's children once processing for that node
  // is complete
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene, unsigned int mesh_index) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    // store the vertex positions/coordinates in object space
    // we need to convert to our own data type since assimp has it's own data
    // structure that is not directly compatible with our maths library
    FMath::Vec3<float> vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    // store normals
    if (mesh->HasNormals()){
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
    }

    // process texture coordinates, first check if the mesh actually contains
    // textures
    //std::cout << "TexCoords: " << mesh->mTextureCoords[0][i].x << std::endl;
    if (mesh->mTextureCoords[0]) {
      FMath::Vec2<float> vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.tex_coords.x = vec.x;
      vertex.tex_coords.y = vec.y;
    } else {
      vertex.tex_coords = FMath::Vec2<float>(0.0f, 0.0f);
    }

    // after processing vertex positions, normals and texture coordinates
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
    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuse_maps = load_material_textures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    // 2. specular maps
    std::vector<Texture> specular_maps = load_material_textures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(aiMaterial *mat,
                                                   aiTextureType type,
                                                   std::string type_name) {
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString texture_file_name;
    mat->GetTexture(type, i, &texture_file_name);
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++){
        if (std::strcmp(textures_loaded[j].path.data(), texture_file_name.C_Str()) == 0){
           textures.push_back(textures_loaded[j]);
           skip = true;
           break;
        }
    }

    if (skip) {
        // skip this already loaded textures and load next one
        continue;
    }

    Texture texture;
    texture.id = texture_from_file(texture_file_name.C_Str());
    texture.type = type_name;
    texture.path = std::string(texture_file_name.C_Str());
    textures.push_back(texture);
    textures_loaded.push_back(texture);
  }
  return textures;
}

unsigned int Model::texture_from_file(const char *file_path) {
  std::string file_name = std::string(file_path);
  file_name = directory + '/' + file_name;
  std::cout << file_name << std::endl;

  unsigned int texture;
  glGenTextures(1, &texture);

  // rgb image has 3 channels : r,g,b
  int img_width, img_height, nr_channels;
  unsigned char *img_data = stbi_load(file_name.c_str(), &img_width,
                                      &img_height, &nr_channels, 0);

  if (!img_data) {
    std::cerr << "Failed to load image data!\n";
    stbi_image_free(img_data);
    return 1;
  }

  GLenum format = GL_RGB;
  if (nr_channels == 1)
    format = GL_RED;
  else if (nr_channels == 4)
    format = GL_RGBA;

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, img_width, img_height, 0, format,
               GL_UNSIGNED_BYTE, img_data);
  glGenerateMipmap(GL_TEXTURE_2D); // only base mipmap generated since

  // set repeeat parameters for the texture on S and T axes
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // set filtering methods to mip maps (maginification/minification)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR); // come back later, parameter might be mistake

  stbi_image_free(img_data);
  return texture;
}
