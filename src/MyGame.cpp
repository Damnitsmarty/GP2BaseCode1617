#include "MyGame.h"

struct Vertex {
	float x, y, z;
};

MyGame::MyGame()
{
	m_VBO = 0;
	m_VAO = 0;

}

MyGame::~MyGame()
{

}

void MyGame::initScene()
{
	GameApplication::initScene();
	Vertex verts[] = {
		{-0.5f,-0.5f,0.0f},
		{0.5f,-0.5f,0.0f },
		{0.0f,0.5f, 0.0f}
	};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), verts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

}

void MyGame::destroyScene()
{
}

void MyGame::render()
{
}

void MyGame::update()
{
}
