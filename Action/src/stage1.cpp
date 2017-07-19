#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "StageControl.h"
#include "config.h"

using namespace gplib;

//---------------------------------
// Stage1
//---------------------------------
//‰Šú‰»ˆ—		
void Stage1Init()
{
	gamestart::Init();
	gamestart::Start();

	InitGate();
	InitKey();
	InitGem();
	InitMap();
	InitPlayer();
	InitEnemy(); 

	bgm::Play("stage_bgm");
}

//ƒtƒŒ[ƒ€ˆ—
void Stage1Step()
{
		if (gamestart::isGameStart() == false)
		{
			if (!IsStageClear())
			{
				StepEnemy();
				StepPlayer();
			}
		}
		gamestart::Step();
		
		SetCamera();
		StepGem();
		StepKey();
		SetUI();
		//Debug
#ifdef _DEBUG
		debug::Draw_ShowFps();
#endif
}

//•`‰æˆ—
void Stage1Disp()
{
	DispMap();
	DispPlayer();
	DispGem();
	DispGate();
	DispEnemy();
	DispKey();
	if (gamestart::isGameStart())
	{
		if (config.startuse)
		{
			char msg[256];
			wsprintf(msg, "Ready", (int)gamestart::GetCount());
			gplib::font::Text(system::WINW/2, system::WINH, 0.f, msg, ARGB(255, 255, 0, 0), 1);
		}
	}
}

void Stage1()
{
	if (gplib::stage::InitStage()){ Stage1Init(); }
	Stage1Step();
	Stage1Disp();
}
