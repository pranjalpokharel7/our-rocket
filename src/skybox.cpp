#include "../include/skybox.hpp"

float skybox_vertices[] = {
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f
};

SkyBox::SkyBox(){
   setup_skybox();
}

void SkyBox::setup_skybox() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices,
               GL_STATIC_DRAW);

  // vertex attributes, {coordinates}
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  // unbind vao so other objects can be bound later
  glBindVertexArray(0);
}

void SkyBox::draw_skybox() {
  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void SkyBox::load_skybox_textures(const char* skybox_textures[]) {
  glGenTextures(1, &texture_ID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ID);
  stbi_set_flip_vertically_on_load(false);
  int img_width, img_height, nr_channels;
  for (int i = 0; i < number_of_textures; i++) {
    unsigned char *img_data = stbi_load(skybox_textures[i], &img_width,
                                        &img_height, &nr_channels, 0);
    if (!img_data) {
      std::cerr << "Cubemap texture failed to load at path : "
                << skybox_textures[i] << std::endl;
      stbi_image_free(img_data);
      continue;
    }

    // TODO: remove this if branch statement?

    GLenum img_color_format = GL_RGB;
    //if (nr_channels == 1) img_color_format = GL_RED;
    //else if (nr_channels == 4) img_color_format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, img_color_format, img_width,
                 img_height, 0, img_color_format, GL_UNSIGNED_BYTE, img_data);
    stbi_image_free(img_data);
  }

  // texture properties
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
