#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>

class Cube{
private:
    unsigned int VAO, VBO;
    void setup_cube();

public:
    Cube();
    void draw_cube(unsigned int program_ID);
};

#endif
