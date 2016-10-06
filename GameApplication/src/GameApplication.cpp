#include "GameApplication.h"

GameApplication::GameApplication()
{
 	m_pWindow=nullptr;
	m_WindowWidth=640;
	m_WindowHeight=480;
	m_WindowCreationFlags=SDL_WINDOW_OPENGL;
	CREATELOG("log.txt");
	m_bIsActive=false;
	m_bIsRunning=false;
	m_WindowTitle="Game";
}

GameApplication::~GameApplication()
{

}

void GameApplication::createWindow(const string& windowTitle,const unsigned int width, const unsigned int height, const unsigned int windowFlags)
{
  LOG(INFO,"Window Created %s Width - %d Height - %d",windowTitle.c_str(),width,height);
	//Create a window
	m_pWindow = SDL_CreateWindow(
		windowTitle.c_str(),             // window title
		SDL_WINDOWPOS_CENTERED,     // x position, centered
		SDL_WINDOWPOS_CENTERED,     // y position, centered
		width,                        // width, in pixels
		height,                        // height, in pixels
		windowFlags           // flags
		);

		m_WindowWidth=width;
		m_WindowHeight=height;
}

void GameApplication::OnBeginRender()
{
	//Set the clear colour(background)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
vec2 pos1 = { 0,0 };
vec2 pos2 = { -0.5,0 };
void GameApplication::render()
{
	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);

	//Reset using the Identity Matrix
	glLoadIdentity();
	
	//Translate to -5.0f on z-axis
	glTranslatef(-0.5f, 0.0f, -5.0f);
	//Begin drawing triangles
	glBegin(GL_TRIANGLES);
	glColor3f(0.4f, 0.0f, 0.0f); //Colour of the vertices

	glVertex3f(pos1.x - 1.0f, pos1.y + 1.0f, 0.0f); // Top
	glVertex3f(pos1.x - 1.0f, pos1.y - 1.0f, 0.0f); // Bottom Left
	glVertex3f(pos1.x + 1.0f, pos1.y - 1.0f, 0.0f); // Bottom Right

	glColor3f(0.0f, 0.3f, 0.5f);
	glVertex3f(pos2.x + 0.0f, pos2.y + 1.0f, 0.0f); // TopLeft
	glVertex3f(pos2.x + 2.0f, pos2.y + 1.0f, 0.0f); // TopRight
	glVertex3f(pos2.x + 2.0f, pos2.y - 1.0f, 0.0f); // BtmR
	glEnd();
}

void GameApplication::OnEndRender()
{
	SDL_GL_SwapWindow(m_pWindow);
}

void GameApplication::update()
{
}

void GameApplication::initGraphics()
{
	//OpenGl Context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	m_GLcontext = SDL_GL_CreateContext(m_pWindow);

	//GLEW
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		LOG(ERROR, "Error: %s", glewGetErrorString(err));
	}

	//OpenGL States
	//Smooth shading
	glShadeModel(GL_SMOOTH);

	//clear the background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Clear the depth buffer to 1.0
	glClearDepth(1.0f);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//The depth test to use
	glDepthFunc(GL_LEQUAL);

	//Turn on best perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Set our viewport
	setViewport((int)m_WindowWidth, (int)m_WindowHeight);
}

void GameApplication::setViewport(int width, int height)
{
	//screen ratio
	GLfloat ratio;

	//make sure height is always above 1
	if (height == 0) {
		height = 1;
	}
	//calculate screen ration
	ratio = (GLfloat)width / (GLfloat)height;

	//Setup viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//Change to projection matrix mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Calculate perspective matrix, using gLM
	mat4 projectionMatrix = perspective(radians(45.0f), ratio, 0.1f, 100.0f);
	glLoadMatrixf(&projectionMatrix[0][0]);

	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);

	//Reset using the Identity Matrix
	glLoadIdentity();
}

void GameApplication::handleMovement(SDL_Keycode key)
{
	float speed = 0.05;
	if (key == SDLK_w) {
		pos1.y += speed;
	}
	if (key == SDLK_s) {
		pos1.y -= speed;
	}
	if (key == SDLK_a) {
		pos1.x -= speed;
	}
	if (key == SDLK_d) {
		pos1.x += speed;
	}

	if (key == SDLK_UP) {
		pos2.y += speed;
	}
	if (key == SDLK_DOWN) {
		pos2.y -= speed;
	}
	if (key == SDLK_LEFT) {
		pos2.x -= speed;
	}
	if (key == SDLK_RIGHT) {
		pos2.x += speed;
	}
}

void GameApplication::parseConfig(int args,char * arg[])
{
  stringstream ss;
  //parse config file
  XMLOptionsParser xmlParser=XMLOptionsParser("settings.xml");
  xmlParser.parse(m_Options);
  ss<<m_Options;
  LOG(INFO,"Settings Parsed\n%s",ss.str().c_str());
  ss.str( std::string());
  ss.clear();

	//parse command line arguments into keyvalue pairs, this should
	//overide options in config files
  CommandLineParser commandLineParser=CommandLineParser(args,arg);
  commandLineParser.parse(m_Options);
  ss<<m_Options;
  LOG(INFO,"Command Line Parsed\n%s",ss.str().c_str());
  ss.str( std::string());
  ss.clear();
}


bool GameApplication::init(int args,char * arg[])
{
	parseConfig(args,arg);

	//Controls the game loop
	m_bIsRunning = true;

  LOG(INFO,"%s","Initialising SDL");
	// init everyting - SDL, if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		//std::cout << "ERROR SDL_Init " << SDL_GetError() << std::endl;
    LOG(ERROR,"SDL can't be initialised %s",SDL_GetError());
		return false;
	}

	m_WindowWidth=m_Options.getOptionAsInt("WindowWidth");
	m_WindowHeight=m_Options.getOptionAsInt("WindowHeight");
	m_WindowTitle=m_Options.getOption("WindowTitle");
	createWindow(m_WindowTitle,m_WindowWidth,m_WindowHeight,m_WindowCreationFlags);
	initGraphics();

	m_bIsActive=true;
	return true;
}

void GameApplication::OnQuit()
{
	//set our boolean which controls the loop to false
	m_bIsRunning = false;

	SDL_GL_DeleteContext(m_GLcontext);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
	CLOSELOG();
}

void GameApplication::OnMaximize()
{
  LOG(INFO,"%s","Maximize");
}

void GameApplication::OnMinimize()
{
  LOG(INFO,"%s","Minimize");
  m_bIsActive=false;
}

void GameApplication::OnRestored()
{
  LOG(INFO,"%s","Restored");
  m_bIsActive=true;
}

void GameApplication::run()
{
	SDL_Event event;
	//Game Loop
	while (m_bIsRunning)
	{
		//While we still have events in the queue
		while (SDL_PollEvent(&event)) {
			//Get event type
			if (event.type == SDL_QUIT) {

				OnQuit();
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					OnQuit();
				}
				if (event.key.keysym.sym == SDLK_f)
				{
					if (SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN) == -1)
					{
						LOG(ERROR, "Can't Maximize %s", SDL_GetError());
					}
				}
				handleMovement(event.key.keysym.sym);

			}
			if (event.type == SDL_WINDOWEVENT)
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
				{
					OnQuit();
					break;
				}

				case SDL_WINDOWEVENT_MINIMIZED:
				{
					OnMinimize();
					break;
				}

				case SDL_WINDOWEVENT_MAXIMIZED:
				{
					OnMaximize();
					break;
				}
				case SDL_WINDOWEVENT_RESTORED:
				{
					OnRestored();
					break;
				}
				}
			}
		}

		update();

		OnBeginRender();
		render();
		OnEndRender();

	}

}
