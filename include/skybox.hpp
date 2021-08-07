#include <glad/glad.h>
#include "../include/stb_image.h"
#include <string>
#include <iostream>
//#include <vector>

class SkyBox{
    private:
        unsigned int VAO, VBO;
        unsigned int texture_ID;
        void setup_skybox();
    public:
        SkyBox();
        void draw_skybox();
        void load_skybox_textures(std::string skybox_textures[]);

        int number_of_textures = 6; // we have 6 faces for each face of cube
};
