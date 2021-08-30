#ifndef ZRENDER_HPP
#define ZRENDER_HPP

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace Render
{    
  class ZRender
  {

  public: 
    GLuint screenVAO;
    int width,height;
    GLuint sbuffer;
    GLuint shaderProgram;

    // Four internal buffers required here
    GLuint depthBuffer;
    GLuint colorBuffer;

    // FBOs.. two required
    GLuint depthFBO, colorFBO;
    GLuint activeDepth;


    // Catch the depth program .. HEHEHEHE
  public : 
    ZRender(int w, int h) : width{w},height{h}
    {
      float screen[] = { 1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,1.0f,-1.0, 1.0f,1.0f };

      glGenVertexArrays(1,&screenVAO);
      glBindVertexArray(screenVAO);

      GLuint screenVBO;
      glGenBuffers(1,&screenVBO);
      glBindBuffer(GL_ARRAY_BUFFER,screenVBO);
      glBufferData(GL_ARRAY_BUFFER,sizeof(screen),screen,GL_STATIC_DRAW);

      glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),nullptr);
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);

      // Initalize the program here
      const char *const vertexShaderSource[] = {
	"#version 330 core\n",
	"layout (location = 0) in vec2 aPos;\n",
	"out vec2 pos;\n",
	"void main()\n",
	"{\n",
	"   gl_Position = vec4(aPos,0.0f,1.0f);\n",
	"   pos = aPos.xy/2.0 + vec2(0.5f,0.5f);\n",
	"}"
      };

      GLuint vertexShader;
      vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, sizeof(vertexShaderSource)/sizeof(*vertexShaderSource), vertexShaderSource, nullptr);
      glCompileShader(vertexShader);
      int success;
      char infoLog[512];
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

      if (!success)
      {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        fputs(infoLog, stdout);
        exit(-1);
      }

      // Now we need to write Fragment Shader
      // Fragment Shader puts color to each pixel and is the phase where advance effects occur
      const char *fragmentShaderSource[] =
        {
	  "#version 330 core\n",
	  "out vec4 color;\n",
	  "uniform sampler2D sbuffer;\n", 
	  "in vec2 pos;\n",
	  "void main()\n",
	  "{\n",
	        "color = texture(sbuffer,pos);\n",
		//	  "color = vec4(1.0f,0.0f,1.0f,1.0f);",
	  "} "
	};
    
      GLuint fragmentShader;
      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, sizeof(fragmentShaderSource)/sizeof(*fragmentShaderSource), fragmentShaderSource, nullptr);
      glCompileShader(fragmentShader);

      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fputs(infoLog, stdout);
        exit(-1);
      }

      // Finally we write Shader Program to link vertex and fragment Shader
      shaderProgram = glCreateProgram();
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
      glLinkProgram(shaderProgram);

      // Check for the completion of linking of shader program
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fputs(infoLog, stdout);
        exit(EXIT_FAILURE);
      }

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      // Lets initialize the frame buffer here too
      
      glGenFramebuffers(1,&depthFBO);
      glBindFramebuffer(GL_FRAMEBUFFER,depthFBO);
      
      glGenTextures(1,&depthBuffer);
      glBindTexture(GL_TEXTURE_2D,depthBuffer);
      
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
      glBindTexture(GL_TEXTURE_2D,0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthBuffer, 0);

      // Same for color
      glGenFramebuffers(1,&colorFBO);
      glBindFramebuffer(GL_FRAMEBUFFER,colorFBO);
      
      glGenTextures(1,&colorBuffer);
      glBindTexture(GL_TEXTURE_2D,colorBuffer);
      
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
      glBindTexture(GL_TEXTURE_2D,0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

     }

    // This is the function to be called at the final 
    void draw()
    {
      glBindFramebuffer(GL_FRAMEBUFFER,0);
      glUseProgram(shaderProgram);
      glBindVertexArray(screenVAO);

      glUniform1i(glGetUniformLocation(shaderProgram,"sbuffer"),0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,colorBuffer);
      //      glBindTexture(GL_TEXTURE_2D,activeDepth);
      glDrawArrays(GL_TRIANGLES,0,6);
      glBindVertexArray(0);
      glUseProgram(0);
    }
   
    void clearBG(float r, float g, float b, float a)
    {
      glBindFramebuffer(GL_FRAMEBUFFER,colorFBO);
      glClearColor(r,g,b,a);
      glClear(GL_COLOR_BUFFER_BIT);
      glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    void clearDepth()
    {
      glBindFramebuffer(GL_FRAMEBUFFER,depthFBO);
     

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, activeDepth, 0);
       activeDepth = depthBuffer;
      glClearColor(1.0f,1.0f,1.0f,1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
          
      glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    // only call this function with depthVertex attached
    // It will attach the fragment shader and will link the program
    void attachDepthFragment(GLuint depthProgram)
    {
      std::string fragmentShaderSource =
	"#version 330 core\n out vec4 color;\n uniform sampler2D zbuffer;\n";
      fragmentShaderSource += "float width = " + std::to_string(width) + ";\nfloat height = " + std::to_string(height) + ";\n";
      fragmentShaderSource += "vec2 dimension = vec2(1000.0f,1000.0f);";
      fragmentShaderSource +=
	"void main() \n {color = texture(zbuffer, vec2(gl_FragCoord.xy/dimension));\n float old = color.x;\n float new = pow(gl_FragCoord.z,100);\n if (new <= old)\n color = vec4(vec3(new),1.0f);\n else \n color = vec4(vec3(old),1.0f); \n}";
											
									  
      // Don't know if it works .. let's see 
      GLuint fragmentShader;
      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      auto z = fragmentShaderSource.c_str();
      glShaderSource(fragmentShader, 1, &z, nullptr);
      glCompileShader(fragmentShader);
      int success;
      char infoLog[512];
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	fputs("From attachDepthFragment() ERROR ::",stdout);
        fputs(infoLog, stdout);
        exit(-1);
      }

      glAttachShader(depthProgram, fragmentShader);
      glLinkProgram(depthProgram);

      // Check for the completion of linking of shader program
      glGetProgramiv(depthProgram, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(depthProgram, 512, NULL, infoLog);
        fputs(infoLog, stdout);
        exit(EXIT_FAILURE);
      }

      glDeleteShader(fragmentShader);
      

    }


    void bindBuffers(GLuint colorProgram)
    {
      glUseProgram(colorProgram);
      glActiveTexture(GL_TEXTURE2);
      glUniform1i(glGetUniformLocation(colorProgram,"depthBuffer"),2);
      glBindTexture(GL_TEXTURE_2D,activeDepth);


      glUniform1i(glGetUniformLocation(colorProgram,"prevBuffer"),3);
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D,colorBuffer);
    }
  };
}

#endif
