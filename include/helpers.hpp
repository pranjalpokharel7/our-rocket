#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "../Maths/vec.hpp"
#include <glad/glad.h>

void update_uniform_3f(const char *uniform_name, unsigned int program_ID,
                       FMath::Vec3<float> in_vec3);
void update_uniform_1f(const char *uniform_name, unsigned int program_ID,
                       float uniform_value);
void update_uniform_matrix_4f(const char *uniform_name, unsigned int program_ID,
                       float* first_element_address);

// positions all containers
//FMath::Vec3<float> cube_positions[] = {
//    FMath::Vec3<float>(0.0f, 0.0f, 0.0f),    FMath::Vec3<float>(2.0f, 5.0f, -15.0f),
//    FMath::Vec3<float>(-1.5f, -2.2f, -2.5f), FMath::Vec3<float>(-3.8f, -2.0f, -12.3f),
//    FMath::Vec3<float>(2.4f, -0.4f, -3.5f),  FMath::Vec3<float>(-1.7f, 3.0f, -7.5f),
//    FMath::Vec3<float>(1.3f, -2.0f, -2.5f),  FMath::Vec3<float>(1.5f, 2.0f, -2.5f),
//    FMath::Vec3<float>(1.5f, 0.2f, -1.5f),   FMath::Vec3<float>(-1.3f, 1.0f, -1.5f)
//};

#endif
