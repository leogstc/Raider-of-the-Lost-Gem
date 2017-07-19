#pragma once

struct Config
{
	//player
	const float GRAVITY = 0.5f;
	//move
	const float PLAYER_MOVE_SPD = 8.0f;
	const float PLAYER_JUMP_POW = 11.0f;
	const float PLAYER_JUMP_INC = 0.24f;

	//map
	const int MAPCELLSZ = 64;
	const int CHARSZX = 64;
	const int CHARSZY = 64;

	//stagecontrol
	bool isopen = false;
	bool startuse = false;
	int gracnt;
	int GameTime;
};

extern Config config;