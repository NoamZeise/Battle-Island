#include "Menu.h"



MenuManager::MenuManager(Render& render)
{
	topTex = render.LoadTexture("textures/menu/top.png");
	bottomTex = render.LoadTexture("textures/menu/bottom.png");
	arrowTex = render.LoadTexture("textures/menu/arrow.png");
	
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Hit, render.LoadTexture("textures/menu/hit.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Act, render.LoadTexture("textures/menu/act.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Build, render.LoadTexture("textures/menu/build.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Exit, render.LoadTexture("textures/menu/exit.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Move, render.LoadTexture("textures/menu/move.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Take, render.LoadTexture("textures/menu/take.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Pass, render.LoadTexture("textures/menu/pass.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Save, render.LoadTexture("textures/menu/save.png")));
	menuTex.insert(std::pair<MenuElems, uint32_t>(MenuElems::Load, render.LoadTexture("textures/menu/load.png")));
}
void MenuManager::Update(Btn& btn)
{
	if (openMenus.size() > 0)
	{
		if (btn.press.Up() && !btn.prev.Up())
		{
			selected--;
			if (selected > openMenus[activeMenuIndex].menuOptions.size() - 1)  selected = openMenus[activeMenuIndex].menuOptions.size() - 1;
		}
		if (btn.press.Down() && !btn.prev.Down())
		{
			selected++;
			if (selected > openMenus[activeMenuIndex].menuOptions.size() - 1)  selected = 0;
		}
		if (btn.press.A() && !btn.prev.A())
		{
			userChoice = openMenus[activeMenuIndex].menuOptions[selected];
		}
	}
}
void MenuManager::Draw(Render& render, glm::vec2 offset)
{
	for (int mIndex = 0; mIndex < openMenus.size(); mIndex++)
	{
		for (int i = 0; i < openMenus[mIndex].menuOptions.size() + 2; i++)
		{
			unsigned int tex = 0;
			if (i == 0)
				tex = topTex;
			else if (i == openMenus[mIndex].menuOptions.size() + 1)
				tex = bottomTex;
			else
				tex = menuTex[openMenus[mIndex].menuOptions[i - 1]];

			render.DrawSquare(
				glm::vec4(
					openMenus[mIndex].position.x - offset.x,
					openMenus[mIndex].position.y + (i * MENU_ELEMS_TEX_DIM.y) - offset.y,
					MENU_ELEMS_TEX_DIM.x, MENU_ELEMS_TEX_DIM.y), 0, tex);
		}
		if (mIndex == activeMenuIndex)
		{
			//draw select arrow
			render.DrawSquare(
				glm::vec4(
					openMenus[mIndex].position.x + ARROW_TEX_DIM.x - offset.x,
					openMenus[mIndex].position.y + ((selected + 1) * MENU_ELEMS_TEX_DIM.y) + ARROW_TEX_DIM.y - offset.y,
					ARROW_TEX_DIM.z, ARROW_TEX_DIM.w), 0, arrowTex);
		}
	}
}
unsigned int MenuManager::OpenMenu(glm::vec2 position, std::vector<MenuElems> options)
{
	options.push_back(MenuElems::Exit);
	Menu menu{ position, options };
	openMenus.push_back(menu);
	reset();
	return openMenus.size() - 1;
}
void MenuManager::CloseMenu(unsigned int menuIndex)
{
	openMenus.erase(openMenus.begin() + menuIndex, openMenus.begin() + menuIndex + 1);
	reset();
}

void MenuManager::reset()
{
	activeMenuIndex = openMenus.size() - 1;
	selected = 0;
	userChoice = MenuElems::None;
}

MenuElems MenuManager::getChoice()
{
	return userChoice;
}
bool MenuManager::isActive() { return openMenus.size() > 0; }
