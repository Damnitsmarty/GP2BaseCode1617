//
//  Shader.cpp
//  GP2BaseCode
//
//  Created by Brian on 01/10/2014.
//  Copyright (c) 2014 Glasgow Caledonian University. All rights reserved.
//

#include "Shader.h"

//Load Shader from File
GLuint loadShaderFromFile(std::string& filename, SHADER_TYPE shaderType)
{
  LOG(INFO,"Loading File %s",filename.c_str() );
	std::string fileContents;
	std::ifstream file;
	file.open(filename.c_str(), std::ios::in);

	if (!file)
	{
    LOG(ERROR,"%s","Can't open file" );
		return 0;
	}
	//calculate file size
	if (file.good())
	{
		file.seekg(0, std::ios::end);
		unsigned long len = file.tellg();
		file.seekg(std::ios::beg);

		if (len == 0)
		{
			LOG(ERROR,"%s","File has no contents" );
			return 0;
		}

		fileContents.resize(len);
		file.read(&fileContents[0], len);
		file.close();
		GLuint program = loadShaderFromMemory(fileContents.c_str(), shaderType);
    LOG(INFO,"Shader Loaded sucessfully %s",filename.c_str() );
		return program;
	}

	return 0;
}


//Load it from a memory buffer
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType)
{
	GLuint program = glCreateShader(shaderType);
	glShaderSource(program, 1, &pMem, NULL);
	glCompileShader(program);
	if (checkForCompilerErrors(program))
	{
		return 0;
	}
	return program;
}


bool checkForLinkErrors(GLuint program)
{
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {

		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::string infoLog;
		glGetShaderInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		LOG(ERROR,"%s","Shader not Linked" );

		//We don't need the shader anymore.
		glDeleteProgram(program);
		return true;
	}

	return false;
}

bool checkForCompilerErrors(GLuint shaderProgram)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::string infoLog;
		infoLog.resize(maxLength);
		glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

		LOG(ERROR,"Shader not Compiled %s",infoLog.c_str());

		//We don't need the shader anymore.
		glDeleteShader(shaderProgram);
		return true;

	}
	return false;
}

void logAttributeInfo(GLuint program)
{
  int params=-1;
  glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
  LOG(INFO,"GL_ACTIVE_ATTRIBUTES = %i", params);
  char longName[64];
  char name[64];
  int maxLength=64;
  int actualLength=0;
  int size=0;
  GLenum type;
  for (int i=0;i<params;++i){
    glGetActiveAttrib (program,i,maxLength,&actualLength,&size,&type,name);
        //We have an array of stuff
        if (size>1)
        {
          for (int j=0;j<size;j++)
          {
            sprintf(longName, "%s[%i]", name, j);
            int location = glGetAttribLocation(program, longName);
            LOG(INFO,"Attribute %i type: %s name: %s location: %i",i,GLTypeToString(type),name,location);
          }
        }
        else
        {
          int location=glGetAttribLocation(program,name);
          LOG(INFO,"Attribute %i type: %s name: %s location: %i",i,GLTypeToString(type),name,location);
        }
  }
}

const char * GLTypeToString(GLenum type)
{
  switch(type) {
      case GL_BOOL: return "bool";
      case GL_INT: return "int";
      case GL_FLOAT: return "float";
      case GL_FLOAT_VEC2: return "vec2";
      case GL_FLOAT_VEC3: return "vec3";
      case GL_FLOAT_VEC4: return "vec4";
      case GL_FLOAT_MAT2: return "mat2";
      case GL_FLOAT_MAT3: return "mat3";
      case GL_FLOAT_MAT4: return "mat4";
      case GL_SAMPLER_2D: return "sampler2D";
      case GL_SAMPLER_3D: return "sampler3D";
      case GL_SAMPLER_CUBE: return "samplerCube";
      case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
      default: break;
    }
    return "other";
}

void logUniformInfo(GLuint program)
{
  int params=-1;
  glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
  LOG(INFO,"GL_ACTIVE_UNIFORMS = %i", params);
  char longName[64];
  char name[64];
  int maxLength=64;
  int actualLength=0;
  int size=0;
  GLenum type;
  for (int i=0;i<params;++i){
    glGetActiveUniform (program,i,maxLength,&actualLength,&size,&type,name);
        //We have an array of stuff
        if (size>1)
        {
          for (int j=0;j<size;j++)
          {
            sprintf(longName, "%s[%i]", name, j);
            int location = glGetUniformLocation(program, longName);
            LOG(INFO,"Uniform %i type: %s name: %s location: %i",i,GLTypeToString(type),name,location);
          }
        }
        else
        {
          int location=glGetUniformLocation(program,name);
          LOG(INFO,"Uniform %i type: %s name: %s location: %i",i,GLTypeToString(type),name,location);
        }
  }
}

void logShaderInfo(GLuint program)
{
  LOG(INFO,"Shader Programme %i info",&program);
  int params=-1;

  glGetProgramiv(program, GL_LINK_STATUS, &params);
  LOG(INFO,"GL_LINK_STATUS %i", params);

  glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
  LOG(INFO,"GL_ATTACHED_SHADERS = %i", params);

  logAttributeInfo(program);
  logUniformInfo(program);
}
