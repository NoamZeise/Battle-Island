#include "Camera.h"


Camera::Camera(glm::vec4 worldSize, Render& render)
{
	this->worldSize = worldSize;
	this->cursorTex = render.LoadTexture("textures/cursor.png");
	width = worldSize.z / TILE_WIDTH;
	height = worldSize.w / TILE_HEIGHT;
}

void Camera::Update(Btn& btn, Timer& timer)
{
	if (btn.press.Select() && !btn.prev.Select()) //swap control modes
		cameraMode = !cameraMode;
	if (cameraMode)
		cameraControls(btn, timer);
	else if (cursor == nullptr)
		centre();
	else
		cursorControls(btn);
}

void Camera::Draw(Render& render)
{
	if (cursor != nullptr)
		render.DrawSquare
		(glm::vec4(cursor->x * TILE_WIDTH, cursor->y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT), 0, cursorTex);
}

void Camera::cameraControls(Btn& btn, Timer& timer)
{
	if (cursor != nullptr) //remove cursor if in camera mode
	{
		delete cursor;
		cursor = nullptr;
	}

	if (btn.press.Right())
		offset.x -= 0.1 * timer.FrameElapsed();
	if (btn.press.Left())
		offset.x += 0.1 * timer.FrameElapsed();
	if (btn.press.Up())
		offset.y += 0.1 * timer.FrameElapsed();
	if (btn.press.Down())
		offset.y -= 0.1 * timer.FrameElapsed();

	correct();
}

void Camera::correct()
{
	if (offset.x > -worldSize.x) offset.x = -worldSize.x;
	if (offset.x < -worldSize.x - worldSize.z + TARGET_WIDTH) offset.x = -worldSize.x - worldSize.z + TARGET_WIDTH;
	if (offset.y > -worldSize.y) offset.y = -worldSize.y;
	if (offset.y < -worldSize.y - worldSize.w + TARGET_HEIGHT) offset.y = worldSize.y - worldSize.w + TARGET_HEIGHT;

	if (worldSize.z < TARGET_WIDTH) offset.x = (TARGET_WIDTH - worldSize.z) / 2;
	if (worldSize.w < TARGET_HEIGHT) offset.y = (TARGET_HEIGHT - worldSize.w) / 2;
}

void Camera::cursorControls(Btn& btn)
{
	int dir[2]{ 0, 0 };
	if (btn.press.Right() && !btn.prev.Right())
		dir[0] += 1;
	if (btn.press.Left() && !btn.prev.Left())
		dir[0] -= 1;

	if (btn.press.Up() && !btn.prev.Up())
		dir[1] -= 1;
	if (btn.press.Down() && !btn.prev.Down())
		dir[1] += 1;

	if (dir[0] == 0 && dir[1] == 0)
		return;

	if (cursor->x + dir[0] >= 0 && cursor->x + dir[0] < width)
		cursor->x += dir[0];

	if (cursor->y + dir[1] >= 0 && cursor->y + dir[1] < height)
	{
		cursor->y += dir[1];
	}
	target(getTileRect(cursor->x, cursor->y));
}

glm::vec4 Camera::getTileRect(int x, int y)
{
	return glm::vec4(worldSize.x + (x * TILE_WIDTH), worldSize.y + (y * TILE_HEIGHT),
		TILE_WIDTH, TILE_HEIGHT);
}

void Camera::target(glm::vec4 target)
{
	offset.x = -(target.x + (target.z / 2)) + (TARGET_WIDTH / 2);
	offset.y = -(target.y + (target.w / 2)) + (TARGET_HEIGHT / 2);

	correct();
}

void Camera::centre()
{
	glm::vec2 midPoint(-offset.x + ((TARGET_WIDTH) / 2), -offset.y + (TARGET_HEIGHT / 2));
	midPoint.x += TILE_WIDTH / 2;
	midPoint.y -= TILE_HEIGHT / 2;
	bool targetFound = false;
	for (unsigned int y = 0; y < height; y++)
		for (unsigned int x = 0; x < width; x++)
		{
			auto tileRect = glm::vec4(worldSize.x + (x * TILE_WIDTH), worldSize.y + (y * TILE_HEIGHT),
				TILE_WIDTH, TILE_HEIGHT);
			if (helper::contains(midPoint, tileRect))
			{
				cursor = new Location{ x, y };
				target(tileRect);
				targetFound = true;
				break;
			}
		}

	if (!targetFound)
	{
		cursor = new Location{ width / 2, height / 2 };
		auto tileRect = glm::vec4(worldSize.x + (cursor->x * TILE_WIDTH), worldSize.y + (cursor->y * TILE_HEIGHT),
			TILE_WIDTH, TILE_HEIGHT);
		target(tileRect);
	}
}

