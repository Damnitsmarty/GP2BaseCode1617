#ifndef _MyGame_H
#define _MyGame_H

#include "GameApplication.h"

class MyGame:public GameApplication
{
public:
	MyGame();
	~MyGame();
	void initScene();
	void destroyScene();
	void render();
	void update();
protected:
	GLuint m_VBO;
	GLuint m_VAO;

};
#endif
