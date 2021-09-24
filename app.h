#pragma once

#ifndef APP_H
#define APP_H

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Render.h"
#include "input.h"
#include "Audio.h"
#include "Timer.h"
#include "Consts.h"
//gbjam
#include "GBInput.h"
#include "World.h"


class App
{
public:
	App();
	~App();
	void loadAssets();
	void run();
	void update();
	void MoveCamera();
	void PostUpdate();
	void PreUpdate();
	void draw();
	void resize(int windowWidth, int windowHeight);

#pragma region GLFW_CALLBACKS
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
#pragma endregion

	Input input;

private:
	glm::vec2 correctedPos(glm::vec2 pos);
	glm::vec2 correctedMouse();

	GLFWwindow* mWindow;
	Render* mRender;
	int mWindowWidth, mWindowHeight;
	Input previousInput;
	Timer timer;
	Btn btn;
	World* world;
	Audio mainMusic;
};

#endif