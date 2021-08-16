#include "../include/mesh.hpp"

Mesh::Mesh() {}
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setup_mesh();
}

void Mesh::setup_mesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // vertex attributes, {coordinates, normal, tex_coords}
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tex_coords));

  // unbind vao so other objects can be bound later
  glBindVertexArray(0);
}

void Mesh::draw_mesh(unsigned int program_ID, unsigned int zprogram, Render::ZRender& zrender) {

   glBindVertexArray(VAO);
   for (int j = 0; j < indices.size()/3; j++)
   {
     glUseProgram(zprogram);
     zrender.depthIndexDrawArrays(GL_TRIANGLES,3,(j*3)*sizeof(unsigned int),zprogram);

    glUseProgram(program_ID);
    zrender.bindBuffers(program_ID);
    
    unsigned int diffuse_count = 1;
    unsigned int specular_count = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
      std::string number;
      std::string texture_type = textures[i].type;
      if (texture_type == "texture_diffuse") {
	number = std::to_string(diffuse_count++);
      } else if (texture_type == "texture_specular") {
	number = std::to_string(specular_count++);
      }

      // do I not need to use program here?
      glUniform1f(glGetUniformLocation(program_ID,
				       (texture_type + number).c_str()), i);
      glActiveTexture(GL_TEXTURE0 + i); // wait why this again?
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,reinterpret_cast<void*>(j*3*sizeof(unsigned int)));
    }
}
