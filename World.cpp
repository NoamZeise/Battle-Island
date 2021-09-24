#include "World.h"


World::World(glm::vec2 position, unsigned int width, unsigned int height)
{
	if (width < 2 || height < 2)
		throw std::runtime_error("dimentions too small");
	this->width = width;
	this->height = height;
	this->position = position;
	worldSize = getBounds();
}

void World::LoadTextures(Render& render)
{
	optionsTex = render.LoadTexture("textures/options.png");
	materialViewTex = render.LoadTexture("textures/materialView.png");
	for (int i = 0; i < 10; i++)
	{
		std::string n = std::to_string(i);
		numTex.insert(std::pair<std::string, uint32_t>(n, render.LoadTexture("textures/numbers/" + n + ".png")));
	}
	texture.insert(std::pair<Tile, uint32_t>(Tile::Empty, 0));
	texture.insert(std::pair<Tile, uint32_t>(Tile::Water, render.LoadTexture("textures/tiles/water.png")));
	texture.insert(std::pair<Tile, uint32_t>(Tile::Material, render.LoadTexture("textures/tiles/forest.png")));
	texture.insert(std::pair<Tile, uint32_t>(Tile::Land, render.LoadTexture("textures/tiles/land.png")));
	texture.insert(std::pair<Tile, uint32_t>(Tile::PlayerCity, render.LoadTexture("textures/tiles/city1.png")));
	texture.insert(std::pair<Tile, uint32_t>(Tile::EnemyCity, render.LoadTexture("textures/tiles/city2.png")));
	texture.insert(std::pair<Tile, uint32_t>(Tile::PlayerUnit, render.LoadTexture("textures/unit.png")));
	texture.insert(std::pair<Tile, uint32_t>(Tile::EnemyUnit, render.LoadTexture("textures/enemyUnit.png")));

	labels.insert(std::pair<Tile, uint32_t>(Tile::Empty, 0));
	labels.insert(std::pair<Tile, uint32_t>(Tile::Water, render.LoadTexture("textures/labels/sea.png")));
	labels.insert(std::pair<Tile, uint32_t>(Tile::Land, render.LoadTexture("textures/labels/land.png")));
	labels.insert(std::pair<Tile, uint32_t>(Tile::Material, render.LoadTexture("textures/labels/tree.png")));
	labels.insert(std::pair<Tile, uint32_t>(Tile::PlayerCity, render.LoadTexture("textures/labels/city.png")));
	labels.insert(std::pair<Tile, uint32_t>(Tile::EnemyCity, render.LoadTexture("textures/labels/city.png")));
	labels.insert(std::pair<Tile, uint32_t>(Tile::PlayerUnit, render.LoadTexture("textures/labels/unit.png")));
	labels.insert(std::pair<Tile, uint32_t>(Tile::EnemyUnit, render.LoadTexture("textures/labels/unit.png")));
	moveUsedTex = render.LoadTexture("textures/usedMove.png");
	moveAvailableTex = render.LoadTexture("textures/availableMove.png");

	map.push_back(std::vector<Tile>(width * height, Tile::Water));
	genWorld();
	camera = Camera(worldSize, render);
	menuManager = new MenuManager(render);
}

void World::Reset()
{
	cleanup();
	map.push_back(std::vector<Tile>(width * height, Tile::Water));
	genWorld();
}

void World::cleanup()
{
	map.clear();
	delete user;
	user = nullptr;
	delete enemy;
	enemy = nullptr;
}

void World::genWorld()
{
	genLand();
	map.push_back(std::vector<Tile>(width * height, Tile::Empty));
	unsigned int furthest = 0;
	bool playerPlaced = false;
	//place cities
	for (unsigned int i = 0; i < height; i++)
	{
		if (getTile(0, Location(width / 2, i)) == Tile::Land)
		{
			if (!playerPlaced)
			{
				playerPlaced = true;
				modTile(1, Location(width / 2, i), Tile::PlayerCity);
				user = new User(Location(width / 2, i), texture[Tile::PlayerCity], worldSize);
			}
			furthest = i;
		}
	}
	modTile(1, Location(width / 2, furthest), Tile::EnemyCity);
	enemy = new NPC(Location{ width / 2, furthest }, texture[Tile::EnemyCity], worldSize);

	//add unit around each city
	
	map.push_back(std::vector<Tile>(width * height, Tile::Empty));

	Location userLoc = user->getCityLocation();
	std::vector<Location> suitable;
	getAdjacentLand(userLoc, &suitable);
	int randomIndex = 0;
	bool suitableFound = false;
	int loops = 0;
	while (!suitableFound)
	{
		int randomIndex = (random.PositiveReal() * (suitable.size()));
		if (randomIndex >= suitable.size())
			randomIndex = suitable.size() - 1;
		std::vector<Location> testLand;
		getAdjacentLand(suitable[randomIndex], &testLand);
		if (testLand.size() != 0)
			suitableFound = true;
		loops++;
		if (loops > 10)
		{
			this->Reset();
			return;
		}
	}
	Location unitPlacement = suitable[randomIndex];
	modTile(2, unitPlacement, Tile::PlayerUnit);
	user->addUnit(Unit(worldSize, unitPlacement, texture[Tile::PlayerUnit], 4, random));
	Location enemyLoc = enemy->getCityLocation();

	suitable.clear();
	getAdjacentLand(enemyLoc, &suitable);
	suitableFound = false;
	loops = 0;
	while (!suitableFound)
	{
		int randomIndex = (random.PositiveReal() * (suitable.size()));
		if (randomIndex >= suitable.size())
			randomIndex = suitable.size() - 1;
		std::vector<Location> testLand;
		getAdjacentLand(suitable[randomIndex], &testLand);
		if (testLand.size() != 0)
			suitableFound = true;
		loops++;
		if (loops > 10)
		{
			this->Reset();
			return;
		}
	}
	unitPlacement = suitable[randomIndex];
	modTile(2, unitPlacement, Tile::EnemyUnit);
	enemy->addUnit(Unit(worldSize, unitPlacement, texture[Tile::EnemyUnit], 4, random));

	//add trees
	for (size_t i = 0; i < (width * height) / 5; i++)
	{
		Location pos(random.PositiveReal() * width, random.PositiveReal() * height);
		
		if (getTile(0, pos) == Tile::Land
			&& getTile(1, pos) != Tile::PlayerCity && getTile(1, pos) != Tile::EnemyCity
			&& getTile(2, pos) != Tile::PlayerUnit && getTile(2, pos) != Tile::EnemyUnit)
		{
			modTile(1, pos, Tile::Material);
		}
		else
			i--;
	}
}

void World::genLand()
{
	int lWid = width / 2;
	int xOffset = (random.PositiveReal() * width / 2);
	int initialYOffset(0), finalYOffset(0);
	if (random.PositiveReal() > 0.5)
	{
		if (random.PositiveReal() > 0.5)
			finalYOffset = 1;
		else
			initialYOffset = 1;
	}
	for (unsigned int y = initialYOffset; y < height - finalYOffset; y++)
	{
		for (unsigned int x = xOffset; x < lWid + xOffset; x++)
		{
			modTile(0, Location(x, y), Tile::Land);
		}
		if (random.PositiveReal() > 0.6)
			xOffset++;
		if (random.PositiveReal() < 0.4)
			xOffset--;

		if (xOffset < 0) xOffset = 1;
		if (xOffset + lWid + 1 > width) { xOffset = width - lWid - 1; }

		if (y < height / 2)
		{
			if (random.PositiveReal() > 0.1)
				lWid++;
		}
		else
		{
			if (random.PositiveReal() > 0.1)
				lWid--;
		}

		if (lWid < 1) { lWid = 1; }
						
		if (lWid >= width) lWid = width - 1;
	}
}

bool World::valid(unsigned int layer, Location location)
{
	bool isValid = true;
	if (map.size() < layer)
	{
#ifndef NDEBUG
		std::cout << ("layer does not exist") << std::endl;
#endif
		isValid = false;
	}
	if (location.x < 0 || location.x > width - 1 || location.y < 0 || location.y > height - 1)
	{
#ifndef NDEBUG
		std::cout << ("tile specified outsize of range x: " + std::to_string(location.x) + " y: " + std::to_string(location.y)) << std::endl;
#endif
		isValid = false;
	}
	return isValid;
}

Tile World::getTile(unsigned int layer, Location location)
{
	if (!valid(layer, location)) return Tile::Empty;
	return map[layer][(location.y * width) + location.x];
}

void World::modTile(unsigned int layer, Location location, Tile replace)
{
	if (!valid(layer, location)) return;

	map[layer][(location.y * width) + location.x] = replace;
}

void World::Update(Btn& btn, Timer& timer)
{

	if (menuManager->isActive())
	{
		menuManager->Update(btn);
		if (btn.press.Start() && !btn.prev.Start())
		{
			menuManager->CloseMenu(menuManager->activeMenu());
			return;
		}
		MenuElems choice = menuManager->getChoice();
		if (choice != MenuElems::None)
		{
			switch (choice)
			{
			case MenuElems::Exit:
				menuManager->CloseMenu(menuManager->activeMenu());
				break;
			case MenuElems::Move:
				CommandMode = true;
				command = choice;
				commandLoc = *camera.getCursor();
				menuManager->CloseMenu(menuManager->activeMenu());
				getAdjacentLand(lastCursor, &cmdOptions);
				break;
			case MenuElems::Hit:
				CommandMode = true;
				command = choice;
				commandLoc = *camera.getCursor();
				menuManager->CloseMenu(menuManager->activeMenu());
				getAdjacentTargets(lastCursor, &cmdOptions);
				break;
			case MenuElems::Take:
				modTile(1, lastCursor, Tile::Empty);
				user->addResource(1);
				user->UnitTake(lastCursor);
				menuManager->CloseMenu(menuManager->activeMenu());
				break;
			case MenuElems::Pass:
				menuManager->CloseMenu(menuManager->activeMenu());
				userTurn = false;
				user->ResetMoves();
				break;
			}
		}
	}
	else
	{
		camera.Update(btn, timer);
		if(userTurn)
			cursorUpdate(btn, timer);
		else
		{
			NPCommand enemyCommand = enemy->getNPCommand(&map);
			switch (enemyCommand.chosenCommand)
			{
			case MenuElems::Pass:
				userTurn = true;
				enemy->ResetMoves();
				break;
			}
		}
		user->Update(timer);
		enemy->Update(timer);
	}
}

void World::cursorUpdate(Btn& btn, Timer& timer)
{
	Location* cursor = camera.getCursor();
	if (cursor != nullptr && !camera.inCameraMode()) // get selected tile
	{
		if (!CommandMode && userTurn)
		{
			if (!(lastCursor == cursor) || tileHovering != Tile::Empty)
			{
				bool tileFound = false;
				for (int i = map.size() - 1; i >= 0; i--)
				{
					Tile tile = getTile(i, *cursor);
					if (tile != Tile::Empty)
					{
						tileHovering = tile;
						tileFound = true;
						break;
					}
				}
				if (!tileFound)
					tileHovering = Tile::Empty;
			}
			if (btn.press.A() && !btn.prev.A() && tileHovering != Tile::Empty)
			{
				switch (tileHovering)
				{
				case Tile::PlayerUnit:
					if (user->UnitMoves(*cursor) > 0)
					{
						std::vector<MenuElems> menuOptions{ MenuElems::Move };
						if (getTile(1, *cursor) == Tile::Material)
							menuOptions.push_back(MenuElems::Take);
						if(isAdjacentTargets(*cursor))
							menuOptions.push_back(MenuElems::Hit);
						menuManager->OpenMenu(MENU_POS, menuOptions);
					}
					break;

				}
			}
			else if(btn.press.Start() && !btn.prev.Start() || btn.press.B() && !btn.prev.B())
			{
				std::vector<MenuElems> menuOptions{ MenuElems::Pass, MenuElems::Save, MenuElems::Load };
				menuManager->OpenMenu(MENU_POS, menuOptions);
			}
		}
		if (CommandMode)
		{
			
			if (btn.press.A() && !btn.prev.A())
			{
				bool contains = false;
				for (size_t i = 0; i < cmdOptions.size(); i++)
				{
					if (cmdOptions[i] == cursor)
					{
						switch (command)
						{
						case MenuElems::Move:
							MoveUnit(commandLoc, *cursor, true);
							break;
						case MenuElems::Hit:
							//get both attackers, remove more health from one being attacked than one starting attack
							break;
						}
						resetCommandMode();
						break;
					}
				}
			}
			else if (btn.press.B() && !btn.prev.B())
				resetCommandMode();
		}
		lastCursor = *cursor;
	}
	else
	{
		tileHovering = Tile::Empty;
		tileSelected = Tile::Empty;
	}
}

void World::resetCommandMode()
{
	CommandMode = false;
	cmdOptions.clear();
	CommandMode = MenuElems::None;
}

void World::getAdjacentLand(Location loc, std::vector<Location>* vect)
{
	if (getTile(0, Location(loc.x + 1, loc.y)) == Tile::Land && getTile(1, Location(loc.x + 1, loc.y)) != Tile::EnemyCity && getTile(1, Location(loc.x + 1, loc.y)) != Tile::PlayerCity && getTile(2, Location(loc.x + 1, loc.y)) == Tile::Empty)
		vect->push_back(Location(loc.x + 1, loc.y));
	if (getTile(0, Location(loc.x - 1, loc.y)) == Tile::Land && getTile(1, Location(loc.x - 1, loc.y)) != Tile::EnemyCity && getTile(1, Location(loc.x - 1, loc.y)) != Tile::PlayerCity && getTile(2, Location(loc.x - 1, loc.y)) == Tile::Empty)
		vect->push_back(Location(loc.x - 1, loc.y));
	if (getTile(0, Location(loc.x, loc.y + 1)) == Tile::Land && getTile(1, Location(loc.x, loc.y + 1)) != Tile::EnemyCity && getTile(1, Location(loc.x, loc.y + 1)) != Tile::PlayerCity && getTile(2, Location(loc.x, loc.y + 1)) == Tile::Empty)
		vect->push_back(Location(loc.x, loc.y + 1));
	if (getTile(0, Location(loc.x, loc.y - 1)) == Tile::Land && getTile(1, Location(loc.x, loc.y - 1)) != Tile::EnemyCity && getTile(1, Location(loc.x, loc.y - 1)) != Tile::PlayerCity && getTile(2, Location(loc.x, loc.y - 1)) == Tile::Empty)
		vect->push_back(Location(loc.x, loc.y - 1));
}

void World::getAdjacentTargets(Location loc, std::vector<Location>* vect)
{
	if (getTile(1, Location(loc.x + 1, loc.y)) == Tile::EnemyCity || getTile(2, Location(loc.x + 1, loc.y)) == Tile::EnemyUnit)
		vect->push_back(Location(loc.x + 1, loc.y));
	if (getTile(1, Location(loc.x - 1, loc.y)) == Tile::EnemyCity || getTile(2, Location(loc.x - 1, loc.y)) == Tile::EnemyUnit)
		vect->push_back(Location(loc.x - 1, loc.y));
	if (getTile(1, Location(loc.x, loc.y + 1)) == Tile::EnemyCity || getTile(2, Location(loc.x, loc.y + 1)) == Tile::EnemyUnit)
		vect->push_back(Location(loc.x, loc.y + 1));
	if (getTile(1, Location(loc.x, loc.y - 1)) == Tile::EnemyCity || getTile(2, Location(loc.x, loc.y - 1)) == Tile::EnemyUnit)
		vect->push_back(Location(loc.x, loc.y - 1));
}
bool World::isAdjacentTargets(Location loc)
{
	if (getTile(1, Location(loc.x + 1, loc.y)) == Tile::EnemyCity || getTile(2, Location(loc.x + 1, loc.y)) == Tile::EnemyUnit)
		return true;
	if (getTile(1, Location(loc.x - 1, loc.y)) == Tile::EnemyCity || getTile(2, Location(loc.x - 1, loc.y)) == Tile::EnemyUnit)
		return true;
	if (getTile(1, Location(loc.x, loc.y + 1)) == Tile::EnemyCity || getTile(2, Location(loc.x, loc.y + 1)) == Tile::EnemyUnit)
		return true;
	if (getTile(1, Location(loc.x, loc.y - 1)) == Tile::EnemyCity || getTile(2, Location(loc.x, loc.y - 1)) == Tile::EnemyUnit)
		return true;
	return false;
}

void World::MoveUnit(Location src, Location dst, bool isPlayer)
{
	modTile(2, src, Tile::Empty);
	if (isPlayer)
	{
		user->moveUnit(src, dst);
		modTile(2, dst, Tile::PlayerUnit);
	}
	else
	{
		enemy->moveUnit(src, dst);
		modTile(2, dst, Tile::EnemyUnit);
	}
}

void World::Draw(Render& render)
{
	glm::vec2 offset = camera.getCameraOffset();
	for (const auto& layer : map)
	{
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				if (layer[(y * width) + x] == Tile::Empty ||
					layer[(y * width) + x] == Tile::PlayerCity ||
					layer[(y * width) + x] == Tile::EnemyCity ||
					layer[(y * width) + x] == Tile::PlayerUnit ||
					layer[(y * width) + x] == Tile::EnemyUnit )
					continue;
				render.DrawSquare(glm::vec4(
					position.x + (x * TILE_WIDTH), position.y + (y * TILE_HEIGHT),
					TILE_WIDTH, TILE_HEIGHT), 0, texture[layer[(y * width) + x]]);
			}
		}
	}
	if (tileHovering != Tile::Empty)
	{
		render.DrawSquare(glm::vec4(5 - offset.x, 130 - offset.y, 22, 8), 0, labels[tileHovering]);
		if (tileHovering == Tile::PlayerUnit)
		{
			unsigned int moves = user->UnitMoves(lastCursor);
			uint32_t tex = moveAvailableTex;
			for (size_t i = 1; i < 3; i++)
			{
				if (i > moves)
					tex = moveUsedTex;
				render.DrawSquare(glm::vec4((lastCursor.x * TILE_WIDTH) + ((i - 1) * (MOVE_TEX_DIM.x + 2) + 2), lastCursor.y * TILE_HEIGHT + 1,
					MOVE_TEX_DIM.x, MOVE_TEX_DIM.y), 0, tex);
			}
		}
	}
	user->Draw(render);
	enemy->Draw(render);
	camera.Draw(render);
	for (size_t i = 0; i < cmdOptions.size(); i++)
	{
		glm::vec4 optionsRect = helper::tileBounds(worldSize, cmdOptions[i]);
		render.DrawSquare(optionsRect, 0, optionsTex);
	}
	render.DrawSquare(glm::vec4(MATERIAL_VIEW_RECT.x - offset.x, MATERIAL_VIEW_RECT.y - offset.y, MATERIAL_VIEW_RECT.z, MATERIAL_VIEW_RECT.w), 0, materialViewTex);
	std::string matCount = std::to_string(user->getResourceCount());
	if (matCount.size() > 1)
	{
		render.DrawSquare(glm::vec4(MATERIAL_VIEW_RECT.x + NUM1_POS.x - offset.x, MATERIAL_VIEW_RECT.y + NUM1_POS.y - offset.y, NUM_DIM.x, NUM_DIM.y), 0,
			numTex[matCount.substr(0, 1)]);
		render.DrawSquare(glm::vec4((MATERIAL_VIEW_RECT.x + NUM2_POS.x) - offset.x,( MATERIAL_VIEW_RECT.y + NUM2_POS.y) - offset.y, NUM_DIM.x, NUM_DIM.y), 0,
			numTex[matCount.substr(1, 1)]);
	}
	else
		render.DrawSquare(glm::vec4(MATERIAL_VIEW_RECT.x + NUM2_POS.x - offset.x, MATERIAL_VIEW_RECT.y + NUM2_POS.y - offset.y, NUM_DIM.x, NUM_DIM.y), 0,
			numTex[matCount.substr(0, 1)]);
	menuManager->Draw(render, offset);
}

glm::vec2 World::getCameraOffset()
{
	return camera.getCameraOffset();
}

glm::vec4 World::getBounds()
{
	return glm::vec4(position.x, position.y, width * TILE_WIDTH, height * TILE_HEIGHT);
}