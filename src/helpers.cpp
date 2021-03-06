#include "../include/helpers.hpp"

void update_uniform_3f(const char* uniform_name, unsigned int program_ID, FMath::Vec3<float> in_vec3){
    unsigned int uniform_loc = glGetUniformLocation(program_ID, uniform_name);
    glUniform3f(uniform_loc, in_vec3.x, in_vec3.y, in_vec3.z);
}

void update_uniform_1f(const char* uniform_name, unsigned int program_ID, float uniform_value){
    unsigned int uniform_loc = glGetUniformLocation(program_ID, uniform_name);
    glUniform1f(uniform_loc, uniform_value);
}

void update_uniform_matrix_4f(const char *uniform_name, unsigned int program_ID,
                              float* first_element_address){
    unsigned int uniform_loc = glGetUniformLocation(program_ID, uniform_name);
    glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, first_element_address);
}

void update_uniform_1i(const char *uniform_name, unsigned int program_ID,
                       int uniform_value){
    unsigned int uniform_loc = glGetUniformLocation(program_ID, uniform_name);
    glUniform1i(uniform_loc, uniform_value);
}

FMath::Vec3<float> ambient(FMath::Vec3<float> color){
   return FMath::Vec3<float>(color.r * 0.3, color.g * 0.3, color.b * 0.3);
}

FMath::Vec3<float> diffuse(FMath::Vec3<float> color){
   return FMath::Vec3<float>(color.r * 0.7, color.g * 0.7, color.b * 0.7);
}

FMath::Vec3<float> specular(FMath::Vec3<float> color){
   return FMath::Vec3<float>(color.r * 0.9, color.g * 0.9, color.b * 0.9);
}
