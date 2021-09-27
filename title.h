#pragma once
#ifndef TITLE_H
#define TITLE_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include "Render.h"
#include "Timer.h"
#include "GBInput.h"

#include <vector>
#include <map>


class Title
{
public:
	enum Menu
	{
		None,
		Small,
		Medium,
		Large,
		Controls,
		Exit,
	};
	Title(Render& render)
	{
		menuTex.insert(std::pair<Menu, uint32_t>(Menu::Small,  render.LoadTexture("textures/title/small.png")));
		menuTex.insert(std::pair<Menu, uint32_t>(Menu::Medium, render.LoadTexture("textures/title/medium.png")));
		menuTex.insert(std::pair<Menu, uint32_t>(Menu::Large, render.LoadTexture("textures/title/large.png")));
		menuTex.insert(std::pair<Menu, uint32_t>(Menu::Controls, render.LoadTexture("textures/title/controls.png")));
		menuTex.insert(std::pair<Menu, uint32_t>(Menu::Exit, render.LoadTexture("textures/title/exit.png")));
		selectedTex = render.LoadTexture("textures/title/titleselected.png");
		bgTex = render.LoadTexture("textures/title/emptyTitle.png");
		controlsTex = render.LoadTexture("textures/title/controlsScreen.png");
	}
	Menu getChoice()
	{
		return choice;
	}

	void Update(Btn& btn)
	{
		if (choice == Menu::Controls)
		{
			if ((btn.press.A() && !btn.prev.A()) || (btn.press.B() && !btn.prev.B()))
			{
				choice = Menu::None;
			}
		}
		else
		{
			if (btn.press.Up() && !btn.prev.Up())
			{
				selected--;
				if (selected < 0)  selected = max;
			}
			if (btn.press.Down() && !btn.prev.Down())
			{
				selected++;
				if (selected > max)  selected = 0;
			}
			if (btn.press.A() && !btn.prev.A())
			{
				choice = tm[selected];
			}
		}
	}

	void Draw(Render& render)
	{
		render.DrawSquare(glm::vec4(0, 0, 160, 144), 0, bgTex);
		for (int i = 0; i < tm.size(); i++)
		{
			render.DrawSquare(glm::vec4(TITLE_BEGIN.x, TITLE_BEGIN.y + i * (TITLE_DIM.y + 5), TITLE_DIM.x, TITLE_DIM.y), 0,
				menuTex[tm[i]]);
			if (i == selected)
			{
				render.DrawSquare(glm::vec4(TITLE_BEGIN.x - ((CHOOSE_DIM.x - TITLE_DIM.x) / 2)
					, TITLE_BEGIN.y + i * (TITLE_DIM.y + 5) - ((CHOOSE_DIM.y - TITLE_DIM.y)/ 2)
					, CHOOSE_DIM.x, CHOOSE_DIM.y), 0,
					selectedTex);
			}
		}
		if (choice == Menu::Controls)
		{
			render.DrawSquare(glm::vec4(0, 0, 160, 144), 0, controlsTex);
		}
	}

	void Reset()
	{
		selected = 0;
		choice = Menu::None;
		active = true;
	}
	bool isActive() { return active; }
	void disable()
	{
		active = false;
	}

private:

	bool active = true;
	const glm::vec2 TITLE_BEGIN = glm::vec2(5, 5);
	const glm::vec2 TITLE_DIM = glm::vec2(40, 15);
	std::vector<Menu> tm = { Menu::Small, Menu::Medium, Menu::Large, Menu::Controls, Menu::Exit};
	std::map<Menu, uint32_t> menuTex;
	uint32_t selectedTex = 0;
	const glm::vec2 CHOOSE_DIM = glm::vec2(44, 19);
	uint32_t bgTex = 0;
	uint32_t controlsTex = 0;

	int selected = 0;
	int max = 4;
	bool smallSelected = false;
	Menu choice = Menu::None;
};


#endif