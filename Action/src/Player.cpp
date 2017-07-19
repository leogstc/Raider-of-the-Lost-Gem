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
	//�A�j���[�V����
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

//���������ړ�&�����蔻��
void MovePlayerX() 
{
	float newX = player.obj_.pos.x + player.obj_.add.x;
	float newY = player.obj_.pos.y;
	POINT r = CheckMap((int)newX, (int)newY, player.obj_.width, player.obj_.height); //���݈ʒu�𑗂�
	if (r.x == -1) //�ړ��\�Ȃ�
	{
		player.obj_.pos.x += player.obj_.add.x;
		if (player.obj_.add.y > 0) //�󒆃W�����v������邽��
		{
			player.onGround_ = false;
		}
	}
	else
	{
		if (player.obj_.add.x < 0) //���ړ��ɕǂ̔���
		{
			player.obj_.pos.x = (float)((r.x + 1)*config.MAPCELLSZ + player.obj_.width / 2);
		}
		else if (player.obj_.add.x > 0) //�E�ړ��ɕǂ̔���
		{
			player.obj_.pos.x = (float)((r.x - 1)*config.MAPCELLSZ + player.obj_.width / 2);
		}
		player.obj_.add.x = 0.f;
	}
}

//���������ړ�&�����蔻��
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
		if (player.obj_.add.y < 0) //��ړ��ɕǂ̔���
		{
			player.obj_.pos.y = (float)((r.y + 1)*config.MAPCELLSZ + player.obj_.height / 2);
		}
		else if (player.obj_.add.y > 0) //���ړ��ɕǂ̔���
		{
			player.onGround_ = true;
			player.obj_.pos.y = (float)((r.y - 1)*config.MAPCELLSZ + player.obj_.height / 2);
		}
		player.obj_.add.y = 0.f;
	}
	if (gplib::input::CheckPress(gplib::input::KEY_BTN0)) //�������W�����v
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
		if (player.obj_.pos.y > config.MAPCELLSZ*map.size()-(1.5f*config.MAPCELLSZ)) //���Ƃ���
		{
			effect::Create((int)player.obj_.pos.x, (int)player.obj_.pos.y, effect::EFFECTTYPE::DIFFUSE);
			player.obj_.setUseOff();
			se::Play("dead");
		}
		MovePlayerX();
		MovePlayerY();
		player.obj_.src.y = (float)player.dir_;
		//�A�j���[�V����
		if (player.obj_.add.x != 0 || player.obj_.add.y != 0) 
		{
			int t[] = { 0,1,2,1 };
			player.obj_.src.x = (float)t[(int)player.anim_.no_];
			player.anim_.step();
		}
	}
}
