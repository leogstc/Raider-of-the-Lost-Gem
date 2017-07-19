#pragma once
#include "define.h"

const int ENEMYMAX = 10;

struct Enemy
{
	objectbase::ObjData obj_;
	objectbase::Anim anim_;
	int dir_;
	float speed;
	RECT rt_;
	bool onGround_;
};

void InitEnemy();
void DispEnemy();
void CreateEnemy(int x, int y);
void MoveEnemyX();
void MoveEnemyY();
void StepEnemy();
