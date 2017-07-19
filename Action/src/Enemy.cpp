#include "Enemy.h"
#include "Player.h"
#include "Map.h"
#include "config.h"
using namespace gplib;


Enemy enemys[ENEMYMAX];

void InitEnemy()
{
	for (auto&enemy : enemys)
	{
		enemy.obj_.setUseOff();
	}
}

void DispEnemy()
{
	for (auto&enemy : enemys)
	{
		enemy.obj_.draw();
	}
}

void CreateEnemy(int x, int y)
{
	for (auto&enemy : enemys)
	{
		objectbase::ObjData& obj = enemy.obj_;
		if (!obj.checkUse())
		{
			obj.init(
				x*config.MAPCELLSZ + (config.MAPCELLSZ / 2), y*config.MAPCELLSZ + (config.MAPCELLSZ / 2), 0.f,
				0.f, 0.f, 0.f,
				"enemy",
				1, 0, 0,
				config.MAPCELLSZ, config.MAPCELLSZ);

			obj.setUseOn();
			enemy.speed = -2.f;
			enemy.dir_ = LEFTMOVE;
			enemy.anim_.no_ = 0;
			enemy.anim_.max_ = 4;
			enemy.anim_.speed_ = 5;
			enemy.anim_.start();
			break;
		}
	}
}

//êÖïΩï˚å¸à⁄ìÆ&ìñÇΩÇËîªíË
void MoveEnemyX()
{
	for (auto&enemy : enemys)
	{
		enemy.obj_.add.x = enemy.speed;
		float newX = enemy.obj_.pos.x + enemy.obj_.add.x;
		float newY = enemy.obj_.pos.y;
		POINT r = CheckMap((int)newX, (int)newY, enemy.obj_.width, enemy.obj_.height);
		if (r.x == -1)
		{
			enemy.obj_.pos.x += enemy.obj_.add.x;
		}
		else
		{
			if (enemy.obj_.add.x < 0)
			{
				enemy.obj_.pos.x = (float)((r.x + 1)*config.MAPCELLSZ + enemy.obj_.width / 2);
				enemy.speed *= -1;
				enemy.dir_ = RIGHTMOVE;
			}
			else if (enemy.obj_.add.x > 0)
			{
				enemy.obj_.pos.x = (float)((r.x - 1)*config.MAPCELLSZ + enemy.obj_.width / 2);
				enemy.speed *= -1;
				enemy.dir_ = LEFTMOVE;
			}

			enemy.obj_.add.x = 0.f;
		}
	}
}

//êÇíºï˚å¸à⁄ìÆ&ìñÇΩÇËîªíË
void MoveEnemyY()
{
	for (auto&enemy : enemys)
	{
		float newX = enemy.obj_.pos.x;
		float newY = enemy.obj_.pos.y + enemy.obj_.add.y;
		POINT r = CheckMap((int)newX, (int)newY, enemy.obj_.width, enemy.obj_.height);
		if (r.x == -1)
		{
			enemy.obj_.pos.y += enemy.obj_.add.y;
		}
		else
		{
			if (enemy.obj_.add.y < 0)
			{
				enemy.obj_.pos.y = (float)((r.y + 1)*config.MAPCELLSZ + enemy.obj_.height / 2);
			}
			else if (enemy.obj_.add.y > 0)
			{
				enemy.obj_.pos.y = (float)((r.y - 1)*config.MAPCELLSZ + enemy.obj_.height / 2);
			}
			enemy.obj_.add.y = 0.f;
		}
	}
}

//update
void StepEnemy()
{
	for (auto&enemy : enemys)
	{
		if (enemy.obj_.checkUse())
		{
			enemy.obj_.move();
		}
		if (player.obj_.checkUse())
		{
			if (enemy.obj_.checkHit(player.obj_))
			{
				se::Play("dead");
				player.dir_ = 0;
				effect::Create((int)player.obj_.pos.x, (int)player.obj_.pos.y, effect::EFFECTTYPE::DIFFUSE);
				player.obj_.setUseOff();
			}
		}
		enemy.obj_.src.y = (float)enemy.dir_;
		if (enemy.obj_.add.x != 0 || enemy.obj_.add.y != 0)
		{
			int t[] = { 0,1,2,1 };
			enemy.obj_.src.x = (float)t[(int)enemy.anim_.no_];
			enemy.anim_.step();
		}
	}
	MoveEnemyX();
	MoveEnemyY();

}