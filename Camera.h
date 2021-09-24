#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>

#include <map>

#include "Consts.h"
#include "Render.h"
#include "Timer.h"
#include "GBInput.h"
#include "GameHelper.h"
#include "GameConsts.h"


class Camera
{
public:
	Location* cursor = nullptr;
	Camera() {}
	Camera(glm::vec4 worldSize, Render& render);
	~Camera() { delete cursor; cursor = nullptr; }
	void Update(Btn& btn, Timer& timer);
	void Draw(Render& render);
	glm::vec2 getCameraOffset() { return offset; }
	bool inCameraMode() { return cameraMode; }
	Location* getCursor() { return cursor; }
private:
	unsigned int cursorTex = 0;
	unsigned int width, height;
	glm::vec4 worldSize;
	glm::vec2 offset = glm::vec2(0, 0);
	bool cameraMode = false;

	glm::vec4 getTileRect(int x, int y);
	void cursorControls(Btn& btn);
	void cameraControls(Btn& btn, Timer& timer);
	void correct();
	void target(glm::vec4 target);
	void centre();
};



#endif