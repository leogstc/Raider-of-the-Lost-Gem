#pragma once
#include "define.h"

enum { STAND, LEFTMOVE, RIGHTMOVE };

struct Player
{
	objectbase::ObjData obj_;
	objectbase::Anim anim_;
	int dir_;
	int life;
	int moveUD_;
	int moveLR_;
	RECT rt_;
	bool onGround_;
};

extern Player player;

void InitPlayer();
void DispPlayer();
void MovePlayerX();
void MovePlayerY();
void StepPlayer();

