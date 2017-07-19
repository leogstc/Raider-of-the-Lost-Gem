#include "StageControl.h"
#include "Player.h"
#include "Map.h"
#include "config.h"

using namespace gplib;

//クリア判定
bool IsStageClear()
{
	if (gate.open)
	{
		if (player.obj_.pos.x>config.MAPCELLSZ * map[0].size() - config.MAPCELLSZ /1.6f)
		{
			if (!config.isopen)
			{
				se::Play("yaa");
				config.isopen = true;
			}
			bgm::Stop("last");
			return true;
		}
	}
	return false;
}

//ゲームオーバー判定
bool IsGameOver()
{
	if (!player.obj_.checkUse())
	{
		bgm::Stop("stage_bgm");
		bgm::Stop("last");
		return true;
	}
	else if (config.GameTime == 0)
	{
		return true;
	}
	return false;
}

//時間の起動
bool Timer()
{
	if (IsStageClear())
	{
		return false;
	}
	if (IsGameOver())
	{
		return false;
	}
	else
	{
		return true;
	}
}

//カメラ
void SetCamera()
{
	POINT lookPt = { player.obj_.pos.x, player.obj_.pos.y };
	char msg3[256];
	wsprintf(msg3, "Time: %d", config.GameTime);

	if (player.obj_.pos.x <= config.MAPCELLSZ * 10)
	{
		lookPt.x = config.MAPCELLSZ * 10;
	}
	if (player.obj_.pos.x >= config.MAPCELLSZ*30)
	{
		lookPt.x = config.MAPCELLSZ * 30;
	}
	if (player.obj_.pos.y <= config.MAPCELLSZ*5+ config.MAPCELLSZ/1.6f)
	{
		lookPt.y = config.MAPCELLSZ * 5 + config.MAPCELLSZ/1.6f;
	}
	if (player.obj_.pos.y >= system::WINH)
	{
		lookPt.y = system::WINH;
	}
	camera::SetLookAt(lookPt.x, lookPt.y);

	gplib::font::Text(lookPt.x + 500, lookPt.y - 350, 0.f, msg3, ARGB(255, 255, 0, 0), 3);
	if (IsStageClear())
	{
		font::Text(lookPt.x - 50, lookPt.y - 75, 0.1f, "Stage Clear", ARGB(255, 255, 255, 255), 2);
		if (input::CheckPush(input::KEY_BTN0))
		{
			stage::ChangeStage(ENDSCENE);
		}
	}
	if (IsGameOver())
	{
		font::Text(lookPt.x-100 , lookPt.y , 0.1f, "You Are Dead", ARGB(255, 255, 0, 0), 2);
		if (input::CheckPush(input::KEY_BTN0))
		{
			stage::ChangeStage(ENDSCENE);
		}
	}
}

void SetUI()
{
	if (config.gracnt == 30)
	{
		config.startuse = true;
	}
	config.gracnt++;
	if (config.gracnt == 60)
	{
		config.startuse = false;
		config.gracnt = 0;
	}
	if (config.gracnt % 59 == 0 && config.gracnt != 0)
	{
		if (Timer())
		{
			config.GameTime--;
		}
	}
	if (config.GameTime <= 0)
	{
		config.GameTime = 0;
	}
}