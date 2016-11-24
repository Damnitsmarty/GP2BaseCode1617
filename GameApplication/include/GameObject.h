#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Common.h";
#include "Texture.h";
#include "Shader.h";
class GameObject {

public:
	vec3 m_Position;
	vec3 m_Rotation;
	vec3 m_Scale;

	mat4 m_TranslationMatrix;
	mat4 m_RotationMatrix;
	mat4 m_ScaleMatrix;
	
	int m_nVerts;
	
	
	
	GameObject();
	~GameObject();
private:

	GLuint m_VBO;
	GLuint m_VAO;

	GLuint m_ShaderProgram;
	GLuint m_Texture;
	GLuint m_ClampSampler;


	//matrix
	mat4 m_ModelMatrix;


};

#endif // !GAMEOBJECT_H
