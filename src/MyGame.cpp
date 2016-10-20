#include "MyGame.h"
const std::string ASSET_PATH = "assets";
const std::string SHADER_PATH = "/shaders";
const std::string TEXTURE_PATH = "/textures";


struct Vertex {
	float x, y, z;
	float tu, tv;
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
	//texture init
	GameApplication::initScene();
	Vertex verts[] = {
		{-0.5f,	-0.5f,	0.0f,	0.0f,	0.0f },
		{0.5f,	-0.5f,	0.0f,	0.0f,	0.0f },
		{-0.5f,	0.5f,	0.0f,	0.0f,	0.0f },
		{0.5f,	0.5f,	0.0f,	0.0f,	0.0f },
		{0.5f,	-0.5f,	0.0f,	0.0f,	0.0f },
		{-0.5f,	0.5f,	0.0f,	0.0f,	0.0f }
	};
	string texturePath = ASSET_PATH + TEXTURE_PATH + "/texture.png";
	m_Texture = loadTextureFromFile(texturePath);

	//GL Sampler
	glGenSamplers(1, &m_Sampler);
	glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S,GL_CLAMP);
	glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T,GL_CLAMP);


	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), verts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(3*sizeof(float)));

	//Define shader objects
	GLuint vertexShaderProgram = 0;
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);

	//Create program
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShaderProgram);
	glAttachShader(m_ShaderProgram, fragmentShaderProgram);
	glLinkProgram(m_ShaderProgram);
	checkForLinkErrors(m_ShaderProgram);
	logShaderInfo(m_ShaderProgram);

	//Delete shader objects
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);
}

void MyGame::destroyScene()
{
	glDeleteProgram(m_ShaderProgram);
}

void MyGame::render()
{
	glUseProgram(m_ShaderProgram);
	glBindVertexArray(m_VAO);

	GLint MVPLocation = glGetUniformLocation(m_ShaderProgram, "MVP");
	if (MVPLocation != -1)
	{
		mat4 MVP = m_ProjMatrix*m_ViewMatrix*m_ModelMatrix;
		glUniformMatrix4fv(MVPLocation, 1,GL_FALSE, glm::value_ptr(MVP));
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MyGame::update()
{
	m_ProjMatrix = perspective(radians(45.0f), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
	m_ViewMatrix = lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	m_ModelMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
}
