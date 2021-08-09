#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include "../Maths/Matrix.hpp"
#include "../Maths/vec.hpp"

class Cube{
private:
    unsigned int VBO;
    void setup_cube();

public:
    Cube();
    void draw_cube();

    FMath::Vec3<float> position;
    FMath::Vec3<float> scale;
    FMath::Vec3<float> color;
    unsigned int VAO;
};

#endif
