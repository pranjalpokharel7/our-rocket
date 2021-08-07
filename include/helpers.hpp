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
void update_uniform_1i(const char *uniform_name, unsigned int program_ID,
                       int uniform_value);

#endif
