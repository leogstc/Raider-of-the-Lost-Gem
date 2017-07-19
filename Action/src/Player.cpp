#include "Player.h"
#include "Map.h"
#include "StageControl.h"
#include "config.h"
using namespace gplib;

Player player;

void InitPlayer()
{
	player.obj_.init(
		0.f, 0.f, 0.f, //pos
		0.f, 0.f, 0.f, //add
		"player", //resname
		1, 0, 0, //src
		64, 64); //src size
	player.obj_.setUseOn();
	player.dir_ = STAND;
	//start pos
	player.obj_.pos.x = 2.f * config.CHARSZX;
	player.obj_.pos.y = 15.f * config.CHARSZY + config.CHARSZY/2;
	//アニメーション
	player.anim_.no_ = 0;
	player.anim_.max_ = 4;
	player.anim_.speed_ = 5;
	player.anim_.start();

	player.onGround_ = true;
	config.GameTime = 200;
}

void DispPlayer()
{
	if (player.obj_.checkUse())
	{
		player.obj_.draw();
	}
}

//水平方向移動&当たり判定
void MovePlayerX() 
{
	float newX = player.obj_.pos.x + player.obj_.add.x;
	float newY = player.obj_.pos.y;
	POINT r = CheckMap((int)newX, (int)newY, player.obj_.width, player.obj_.height); //現在位置を送る
	if (r.x == -1) //移動可能なら
	{
		player.obj_.pos.x += player.obj_.add.x;
		if (player.obj_.add.y > 0) //空中ジャンプを避けるため
		{
			player.onGround_ = false;
		}
	}
	else
	{
		if (player.obj_.add.x < 0) //左移動に壁の判定
		{
			player.obj_.pos.x = (float)((r.x + 1)*config.MAPCELLSZ + player.obj_.width / 2);
		}
		else if (player.obj_.add.x > 0) //右移動に壁の判定
		{
			player.obj_.pos.x = (float)((r.x - 1)*config.MAPCELLSZ + player.obj_.width / 2);
		}
		player.obj_.add.x = 0.f;
	}
}

//垂直方向移動&当たり判定
void MovePlayerY() 
{
	player.obj_.add.y += config.GRAVITY;

	float newX = player.obj_.pos.x;
	float newY = player.obj_.pos.y + player.obj_.add.y;
	POINT r = CheckMap((int)newX, (int)newY, player.obj_.width, player.obj_.height);
	if (r.x == -1)
	{
		player.obj_.pos.y += player.obj_.add.y;
	}
	else
	{
		if (player.obj_.add.y < 0) //上移動に壁の判定
		{
			player.obj_.pos.y = (float)((r.y + 1)*config.MAPCELLSZ + player.obj_.height / 2);
		}
		else if (player.obj_.add.y > 0) //下移動に壁の判定
		{
			player.onGround_ = true;
			player.obj_.pos.y = (float)((r.y - 1)*config.MAPCELLSZ + player.obj_.height / 2);
		}
		player.obj_.add.y = 0.f;
	}
	if (gplib::input::CheckPress(gplib::input::KEY_BTN0)) //長押しジャンプ
	{
		if (player.obj_.add.y < 0)
		{
			player.obj_.add.y -= config.PLAYER_JUMP_INC;
		}
	}
}

//update
void StepPlayer()
{
	if (player.obj_.checkUse())
	{
		if (input::CheckPress(input::KEY_LEFT))
		{
			player.obj_.add.x = -config.PLAYER_MOVE_SPD;
			player.dir_ = LEFTMOVE;
		}
		else if (input::CheckPress(input::KEY_RIGHT))
		{
			player.obj_.add.x = config.PLAYER_MOVE_SPD;
			player.dir_ = RIGHTMOVE;
		}
		else
		{
			player.obj_.add.x = 0;
		}
		if (input::CheckPush(input::KEY_BTN0))
		{
			if (player.onGround_)
			{
					player.onGround_ = false;
					player.obj_.add.y = -config.PLAYER_JUMP_POW;
			}
		}
		if (player.obj_.pos.y > config.MAPCELLSZ*map.size()-(1.5f*config.MAPCELLSZ)) //落とし穴
		{
			effect::Create((int)player.obj_.pos.x, (int)player.obj_.pos.y, effect::EFFECTTYPE::DIFFUSE);
			player.obj_.setUseOff();
			se::Play("dead");
		}
		MovePlayerX();
		MovePlayerY();
		player.obj_.src.y = (float)player.dir_;
		//アニメーション
		if (player.obj_.add.x != 0 || player.obj_.add.y != 0) 
		{
			int t[] = { 0,1,2,1 };
			player.obj_.src.x = (float)t[(int)player.anim_.no_];
			player.anim_.step();
		}
	}
}
