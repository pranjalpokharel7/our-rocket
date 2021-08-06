#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include "../Maths/Matrix.hpp"
#include "../Maths/vec.hpp"

class Cube{
private:
    unsigned int VAO, VBO;
    void setup_cube();

public:
    Cube();
    void draw_cube(unsigned int program_ID);

    FMath::Vec3<float> position;
    FMath::Vec3<float> scale;
};

#endif
