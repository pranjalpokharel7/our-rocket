#include "../include/renderer.h"
#include "../Maths/vec.hpp"
#include "../Maths/Matrix.hpp"
#include <GLFW/glfw3.h>

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
    auto metalTexture = Texture2D("./include/metal.jpg");

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
	transform = transform.perspective(1.0f);

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

	
	glfwPollEvents();
	glfwSwapBuffers(render.window);
    }

    Render::DestroyRenderer(&render);
    return 0;
}

