#ifndef MESH_HPP
#define MESH_HPP

#include "../Maths/vec.hpp"
#include <vector>
#include "../include/renderer.h"

struct Vertex {
  FMath::Vec3<float> position;
  FMath::Vec3<float> normal;
  FMath::Vec2<float> tex_coords;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
private:
  unsigned int VBO, EBO;
  void setup_mesh();

public:
  Mesh();
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  void draw_mesh(unsigned int program_ID);

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int VAO;
};

#endif
