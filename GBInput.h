#pragma once
#ifndef GB_INPUT_H
#define GB_INPUT_H

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include <map>
#include <array>

enum GBButton
{
	Up,
	Down,
	Left,
	Right,
	A,
	B,
	Select,
	Start
};

class GBInput
{
public:
	GBInput()
	{
		for (uint32_t i = GBButton::Up; i < GBButton::Start; i++)
			pressed.insert(std::pair<GBButton, bool>(static_cast<GBButton>(i), false));
	}
	void Update(bool* keys)
	{
		for (uint32_t i = GBButton::Up; i < GBButton::Start; i++)
			pressed[static_cast<GBButton>(i)] = false;

		test(keys[GLFW_KEY_W],		GBButton::Up);
		test(keys[GLFW_KEY_A],		GBButton::Left);
		test(keys[GLFW_KEY_S],		GBButton::Down);
		test(keys[GLFW_KEY_D],		GBButton::Right);
		test(keys[GLFW_KEY_COMMA],	GBButton::A);
		test(keys[GLFW_KEY_PERIOD], GBButton::B);
		test(keys[GLFW_KEY_ENTER],  GBButton::Select);
		test(keys[GLFW_KEY_ESCAPE], GBButton::Start);
	}

	bool Up() { return pressed[GBButton::Up]; }
	bool Left() { return pressed[GBButton::Left]; }
	bool Right() { return pressed[GBButton::Right]; }
	bool Down() { return pressed[GBButton::Down]; }
	bool B() { return pressed[GBButton::B]; }
	bool A() { return pressed[GBButton::A]; }
	bool Select() { return pressed[GBButton::Select]; }
	bool Start() { return pressed[GBButton::Start]; }
private:
	std::map<GBButton, bool> pressed;

	void test(bool state, GBButton btn)
	{
		if (state)
			pressed[btn] = true;
	}
};










#endif