#pragma once
#ifndef WORLD_H
#define WORD_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>

#include <vector>
#include <map>


#include "Consts.h"
#include "Render.h"
#include "Timer.h"
#include "GBInput.h"
#include "GameHelper.h"
#include "GameConsts.h"
#include "Camera.h"
#include "MyRandom.h"
#include "Player.h"
#include "Menu.h"


class World
{
public:
	World(glm::vec2 position, unsigned int width, unsigned int height);
	~World() 
	{ 
		cleanup();	
		if (menuManager != nullptr)
			delete menuManager;
		menuManager = nullptr;
	}
	
	void LoadTextures(Render& render);

	void Update(Btn& btn, Timer& timer);

	void Draw(Render& render);
	void Reset(glm::vec2 position, unsigned int width, unsigned int height);
	bool getWorldComplete() { return worldComplete; }

	glm::vec2 getCameraOffset();
private:
	glm::vec2 position;
	unsigned int width, height;
	bool gameOver = false;
	bool playerLost = false;
	bool worldComplete = false;

	std::vector<std::vector<Tile>> map;
	Location lastCursor;
	Tile tileHovering = Tile::Empty;
	Tile tileSelected = Tile::Empty;

	bool CommandMode = false;
	Location commandLoc;
	MenuElems command = MenuElems::None;
	std::vector<Location> cmdOptions;
	unsigned int optionsTex = 0;

	bool userTurn = true;
	User* user;
	NPC* enemy;
	const float ENEMY_MOVE_DELAY = 1000.0f;
	float enemyMoveTimer = ENEMY_MOVE_DELAY;
	uint32_t pcTurnTex = 0;
	const glm::vec4 PC_TURN_RECT = glm::vec4(125, 5, 30, 9);
	uint32_t moveUsedTex = 0;
	uint32_t moveAvailableTex = 0;
	const glm::vec2 MOVE_TEX_DIM = glm::vec2(5, 3);
	std::map<Tile, uint32_t> texture;
	std::map<Tile, uint32_t> labels;
	glm::vec4 worldSize;
	Camera camera;
	Random random;
	MenuManager* menuManager;   
	uint32_t cityHpFullTex = 0;
	uint32_t cityHpEmptyTex = 0;
	const glm::vec2 CITY_HP_DIM = glm::vec2(4, 3);

	const glm::vec2 MENU_POS = glm::vec2(80, 40);

	std::map<std::string, uint32_t> numTex;
	uint32_t materialViewTex = 0;
	const glm::vec4 MATERIAL_VIEW_RECT = glm::vec4(2, 3, 17, 9);
	const glm::vec2 NUM_DIM = glm::vec2(3, 5);
	const glm::vec2 NUM1_POS = glm::vec2(8, 2);
	const glm::vec2 NUM2_POS = glm::vec2(12, 2);

	uint32_t unitCostTex = 0;
	const glm::vec4 UNIT_COST_RECT = glm::vec4(2, 17, 18, 18);

	uint32_t victoryTex = 0;
	uint32_t defeatTex = 0;

	uint32_t bgColourTex = 0;

	glm::vec4 getBounds();
	void genWorld();
	void genLand();
	bool valid(unsigned int layer, Location location);
	void cursorUpdate(Btn& btn, Timer& timer);
	void menuUpdate(Btn& btn);
	void enemyAiUpdate(Timer& timer);
	void resetCommandMode();
	void MoveUnit(Location src, Location dst, bool player);
	void HitUnitOrCity(Location attacker, Location defender, bool player);
	void buildUnit(Location dst, bool isPlayer);
	void modTile(unsigned int layer, Location location, Tile replace);
	Tile getTile(unsigned int layer, Location location);
	void getAdjacentLand(Location loc, std::vector<Location>* vect);
	void getAdjacentTargets(Location loc, std::vector<Location>* vect, bool player);
	bool isAdjacentTargets(Location loc, bool player);
	void cleanup();
};



#endif