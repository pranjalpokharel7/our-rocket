#include "../include/renderer.h"
#include "../Maths/vec.hpp"
#include "../Maths/Matrix.hpp"
#include <GLFW/glfw3.h>

static Render::State state;

static void handleEvents(GLFWwindow *window);
// Lets try without header and matching function defintion
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
    auto render = Render::CreateRenderer();
    render.shader_program = ShaderProgram("./shader/vertex.vs","./shader/fragment.fs");

       float vertices[] = {
        // positions          // texture Coords
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

       -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
       -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

       -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
       -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
       -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
       -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

       -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
       -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
       -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,render.VBO);
    glBufferSubData(GL_ARRAY_BUFFER,render.vbo_offset,sizeof(vertices),vertices);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    render.vbo_offset += sizeof(vertices)/sizeof(vertices[0]);
    
    float planeVertices[] = {
	 // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
	 -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
	 -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

	 5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
	 -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f, 2.0f, 2.0f};
			  
     
    GLuint plane_vbo_offset, plane_vao;
    glGenVertexArrays(1,&plane_vao);
    glBindVertexArray(plane_vao);
    glBindBuffer(GL_ARRAY_BUFFER,render.VBO);
    plane_vbo_offset = render.vbo_offset;
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(float)*render.vbo_offset,sizeof(planeVertices),planeVertices);
    // glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(planeVertices),planeVertices);

    render.vbo_offset += sizeof(planeVertices)/sizeof(planeVertices[0]);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(1);

    FMath::Mat4 transform(1.0f), mode{1.0f};
    glEnable(GL_DEPTH_TEST);
    auto marbleTexture = Texture2D("./include/marble.jpg");
    auto metalTexture = Texture2D("./include/metal.png");

    std::cout << state.camera.ViewMatrix();
    while(!glfwWindowShouldClose(render.window))
    {
        glClearColor(0.4f,0.4f,0.4,1.0f);
	glBindVertexArray(vao);
	glUseProgram(render.shader_program.shader_program);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto texture_loc = glGetUniformLocation(render.shader_program.shader_program,"mtexture");
	
	auto mat_loc = glGetUniformLocation(render.shader_program.shader_program,"transform");
        auto timer = glfwGetTime();
	transform = 1.0f;
	// transform = transform.perspective(1.0f);
	//	transform = transform.perspective(1.0f) * state.camera.ViewMatrix();
	transform = state.camera.ViewMatrix() * transform.perspective(1.0f,30.0f) ; 

	// Set the texture value
	glUniform1i(texture_loc,0);
	// transform = transform.translate({0.0f,0.0f,-2.50f});
	// transform = transform.rotateY(timer*2.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,marbleTexture.texture_id);
	glUniformMatrix4fv(mat_loc,1,GL_FALSE,transform.translate(FMath::Vec3(0.0f,0.0f,-4.0f)).value_ptr());
	glDrawArrays(GL_TRIANGLES,0,36);

	glUniformMatrix4fv(mat_loc,1,GL_FALSE,transform.translate({-2.0f,0.0f,-8.0f}).value_ptr());
	glDrawArrays(GL_TRIANGLES,0,36);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,metalTexture.texture_id);
	glBindVertexArray(plane_vao);
	glUniformMatrix4fv(mat_loc,1,GL_FALSE,transform.translate({-1.0f,0.0f,-8.0f}).value_ptr());
	glDrawArrays(GL_TRIANGLES,plane_vbo_offset/5,6);		  

	handleEvents(render.window);
	glfwPollEvents();
	glfwSwapBuffers(render.window);
    }

    Render::DestroyRenderer(&render);
    return 0;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  // Doubt xoffset will be produced 
  state.fovy -= yoffset;
  if (state.fovy > 90)
    state.fovy = 89.0f;
  else
    state.fovy = 0.0f;
}

void handleEvents(GLFWwindow* window)
{
  // This particular part could be made event but it doesn't matter much here 
  double xPos, yPos;
  glfwGetCursorPos(window,&xPos,&yPos);
  auto delta_xPos = -xPos + state.last_xpos;
  auto delta_yPos =  yPos - state.last_ypos;

  if (state.first_mouse)
  {
    delta_yPos = delta_xPos = 0;
    state.first_mouse = false;
  }

  state.last_xpos = xPos;
  state.last_ypos = yPos;
  state.pitch += -delta_yPos * state.angle_constant;
  state.yaw += delta_xPos * state.angle_constant;

  if (state.pitch > 1.5)
    state.pitch = 1.5;
  else if (state.pitch < -1.5)
    state.pitch = -1.5;

  // This is the rotation around X followed by rotation about Y
  state.camera.camera_front.x = -std::sin(state.yaw);
  state.camera.camera_front.y = std::sin(state.pitch) * std::cos(state.yaw);
  state.camera.camera_front.z = -std::cos(state.pitch) * std::cos(state.yaw);

  state.camera.camera_front = state.camera.camera_front.unitVec();
  
  // Now handle the movement
  // Have to handle the first frame here 
  constexpr float move_speed = 0.1f;
  auto current_frame = glfwGetTime();
  state.delta_time = current_frame - state.last_frame;
  state.last_frame = current_frame;

  if (state.first_frame)
  {
    state.delta_time = 0.0f;
    state.first_frame = false;
  }
  
  if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
    state.camera.camera_pos = state.camera.camera_pos + state.camera.camera_front * state.delta_time;
  if (glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
    state.camera.camera_pos = state.camera.camera_pos -  state.camera.camera_front * state.delta_time;
}
