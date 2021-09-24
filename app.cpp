#include "app.h"


App::App()
{
	//set member variables
	mWindowWidth = 800;
	mWindowHeight = 720;
	//init glfw window
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //using vulkan not openGL
	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Vulkan App", nullptr, nullptr);
	glfwSetWindowUserPointer(mWindow, this);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	if(FIXED_RATIO)
		glfwSetWindowAspectRatio(mWindow, TARGET_WIDTH, TARGET_HEIGHT);
	mRender = new Render(mWindow, glm::vec2(TARGET_WIDTH, TARGET_HEIGHT));
	loadAssets();
}


App::~App()
{
	delete world;
	delete mRender;
	//cleanup glfw
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void App::loadAssets()
{
	//TODO load assets
	world = new World(glm::vec2(0, 0), 15, 20);
	world->LoadTextures(*mRender);
	mainMusic = Audio("audio/main.mp3");
	//mainMusic.loop();
	mainMusic.setVolume(0.5);

	mRender->endTextureLoad();
}

void App::run()
{
	while (!glfwWindowShouldClose(mWindow))
	{
		update();
		draw();
	}
}

void App::resize(int windowWidth, int windowHeight)
{
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;
	if(mRender != nullptr)
		mRender->framebufferResized = true;
}

void App::PreUpdate()
{
	glfwPollEvents();
	btn.press.Update(input.Keys);
	timer.Update();
}

void App::update()
{
	PreUpdate();

	world->Update(btn, timer);
	
	PostUpdate();
}

void App::PostUpdate()
{
	btn.prev = btn.press;
	MoveCamera();
}

void App::MoveCamera()
{
	mRender->setCameraOffset(world->getCameraOffset());
}


void App::draw()
{
	mRender->startDraw();

	world->Draw(*mRender);

	mRender->endDraw();
}

glm::vec2 App::correctedPos(glm::vec2 pos)
{
	return glm::vec2(pos.x * ((float)TARGET_WIDTH / (float)mWindowWidth), pos.y * ((float)TARGET_HEIGHT / (float)mWindowHeight));
}

glm::vec2 App::correctedMouse()
{
	return correctedPos(glm::vec2(input.X, input.Y));
}

	
#pragma region GLFW_CALLBACKS


void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->resize(width, height);
}

void App::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->input.X = xpos;
	app->input.Y = ypos;
}
void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->input.offset = yoffset;
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		if (glfwGetWindowMonitor(window) == nullptr)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(window, NULL, 100, 100, app->mWindowWidth, app->mWindowHeight, mode->refreshRate);
		}
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			app->input.Keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			app->input.Keys[key] = false;
		}
	}
}

void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

	if (button >= 0 && button < 8)
	{
		if (action == GLFW_PRESS)
		{
			app->input.Buttons[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			app->input.Buttons[button] = false;
		}
	}
}
#pragma endregion