#pragma once
#ifndef MENU_H
#define MENU_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>

#include <map>
#include <vector>

#include "Render.h"
#include "GBInput.h"
#include "GameConsts.h"



struct Menu
{

	glm::vec2 position;
	std::vector<MenuElems> menuOptions;
};

class MenuManager
{
public:
	MenuManager(Render& render);
	void Update(Btn& btn);
	void Draw(Render& render, glm::vec2 offset);
	unsigned int OpenMenu(glm::vec2 position, std::vector<MenuElems> options);
	void CloseMenu(unsigned int menuIndex);
	void reset();
	MenuElems getChoice();
	bool isActive();
	unsigned int activeMenu() { return activeMenuIndex; }
private:
	std::map<MenuElems, unsigned int> menuTex;
	unsigned int activeMenuIndex = 0;
	unsigned int selected = 0;
	std::vector<Menu> openMenus;
	MenuElems userChoice = MenuElems::None;
	unsigned int arrowTex = 0;
	unsigned int topTex = 0;
	unsigned int bottomTex = 0;
	const glm::vec2 MENU_ELEMS_TEX_DIM = glm::vec2(32, 8);
	const glm::vec4 ARROW_TEX_DIM = glm::vec4(24, 2, 5, 5);
};




#endif