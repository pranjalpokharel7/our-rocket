#include "../include/renderer.h"
#include "../include/model.hpp"
#include "../include/stb_image.h"
#include "../include/helpers.hpp"
#include "../include/cube.hpp"
#include "../Maths/Matrix.hpp"
#include "../Maths/vec.hpp"
#include <GLFW/glfw3.h>

static Render::State state;

static void handleEvents(GLFWwindow *window);
// Lets try without header and matching function defintion
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int main() {
  auto render = Render::CreateRenderer();
  render.shader_program =
      ShaderProgram("./shader/model-vertex.glsl", "./shader/model-fragment.glsl");
  render.light_cube_shader_program = ShaderProgram("./shader/light-cube-vertex.glsl","./shader/light-cube-fragment.glsl");

  stbi_set_flip_vertically_on_load(true);

  //Model mainModel("./models/cyborg/cyborg.obj");
  //Model mainModel("./models/rocket/rocket.obj");
  Model mainModel("./models/rocket2/rocks.obj");
  //Model mainModel("./models/backpack/backpack.obj");

  Cube cube;
  cube.position = FMath::Vec3<float>(0.0f, 1.0f, -9.0f);
  cube.scale = FMath::Vec3<float>(0.1f, 0.1f, 0.1f);

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(render.window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(render.shader_program.shader_program);
    FMath::Mat4 projection(1.0f), view(1.0f), model(1.0f), cube_model(1.0f);

    projection = projection.perspective(1.0f, 30.0f); // 30 degrees so 30.0f
    view = state.camera.ViewMatrix();

    // rotating light
    cube.position.x = sin(glfwGetTime());
    cube.position.z = -3.0f * cos(glfwGetTime());

    update_uniform_3f("viewPos", render.shader_program.shader_program, state.camera.camera_pos);
    update_uniform_3f("light.position", render.shader_program.shader_program, cube.position);

    update_uniform_3f("light.ambient", render.shader_program.shader_program, FMath::Vec3<float>(0.1f, 0.0f, 0.0f));
    update_uniform_3f("light.diffuse", render.shader_program.shader_program, FMath::Vec3<float>(0.7f, 0.1f, 0.1f));
    //update_uniform_3f("light.specular", render.shader_program.shader_program, FMath::Vec3<float>(1.0f, 1.0f, 1.0f));

    model = model.translate({0.0f, 0.0f, 0.0f});
    model = model.scale({0.1f, 0.1f, 0.1f});

    update_uniform_matrix_4f("model", render.shader_program.shader_program, &model[0][0]);
    update_uniform_matrix_4f("projection", render.shader_program.shader_program, &projection[0][0]);
    update_uniform_matrix_4f("view", render.shader_program.shader_program, &view[0][0]);

    mainModel.draw_model(render.shader_program.shader_program);

    glUseProgram(render.light_cube_shader_program.shader_program);

    cube_model = cube_model.translate(cube.position);
    cube_model = cube_model.scale(cube.scale);

    update_uniform_matrix_4f("model", render.shader_program.shader_program, &cube_model[0][0]);
    update_uniform_matrix_4f("projection", render.shader_program.shader_program, &projection[0][0]);
    update_uniform_matrix_4f("view", render.shader_program.shader_program, &view[0][0]);

    cube.draw_cube(render.light_cube_shader_program.shader_program);

    handleEvents(render.window);
    glfwPollEvents();
    glfwSwapBuffers(render.window);
  }

  Render::DestroyRenderer(&render);
  return 0;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  // Doubt xoffset will be produced
  state.fovy -= yoffset;
  if (state.fovy > 90)
    state.fovy = 89.0f;
  else
    state.fovy = 0.0f;
}

void handleEvents(GLFWwindow *window) {
  // This particular part could be made event but it doesn't matter much here
  double xPos, yPos;
  glfwGetCursorPos(window, &xPos, &yPos);
  auto delta_xPos = -xPos + state.last_xpos;
  auto delta_yPos = yPos - state.last_ypos;

  if (state.first_mouse) {
    delta_yPos = delta_xPos = 0;
    state.first_mouse = false;
  }

  state.last_xpos = xPos;
  state.last_ypos = yPos;
  state.pitch += -delta_yPos * state.angle_constant;
  state.yaw -= delta_xPos * state.angle_constant;

  if (state.pitch > 1.5)
    state.pitch = 1.5;
  else if (state.pitch < -1.5)
    state.pitch = -1.5;

  state.camera.camera_front.x = std::sin(state.yaw) * std::cos(state.pitch);
  state.camera.camera_front.y = std::sin(state.pitch);
  state.camera.camera_front.z = -std::cos(state.pitch) * std::cos(state.yaw);

  state.camera.camera_front = state.camera.camera_front.unitVec();

  // Now handle the movement
  // Have to handle the first frame here
  constexpr float move_speed = 0.1f;
  auto current_frame = glfwGetTime();
  state.delta_time = current_frame - state.last_frame;
  state.last_frame = current_frame;

  if (state.first_frame) {
    state.delta_time = 0.0f;
    state.first_frame = false;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    state.camera.camera_pos =
        state.camera.camera_pos + state.camera.camera_front * state.delta_time;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    state.camera.camera_pos =
        state.camera.camera_pos - state.camera.camera_front * state.delta_time;
}
