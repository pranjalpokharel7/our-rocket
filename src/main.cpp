#include "../include/renderer.h"
#include <GLFW/glfw3.h>

int main()
{
    auto render = Render::CreateRenderer();
    render.shader_program = ShaderProgram("./shader/vertex.vs","./shader/fragment.fs");

    float vertices[] = {0.5f,0.5f,-0.5f,0.5f,0.0f,-0.5f};

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,render.VBO);
    glBufferSubData(GL_ARRAY_BUFFER,render.vbo_offset,sizeof(vertices),vertices);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,nullptr);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(render.window))
    {
	glClearColor(0.4f,0.4f,0.4,1.0f);
	glBindVertexArray(vao);
	glUseProgram(render.shader_program.shader_program);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES,0,3);
	glfwPollEvents();
	glfwSwapBuffers(render.window);
    }

    Render::DestroyRenderer(&render);
    return 0;
}

