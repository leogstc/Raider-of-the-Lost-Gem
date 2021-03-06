
#include "define.h"

using namespace gplib;
int cnt = 0;
bool grause = false;
//---------------------------------
// Title
//---------------------------------
//初期化処理		
void TitleInit()
{
	bgm::Play("title_bgm");
}

//フレーム処理
void TitleStep()
{
	cnt++;
	if (input::CheckPush(input::KEY_BTN0))
	{
		//bgm::Stop("title_bgm");
		se::Play("start");
		stage::ChangeStage(MEUNSCENE);

	}
	if (cnt == 10 && cnt != 0)
	{
		grause = true;
	}
	if (cnt == 50 && cnt != 0)
	{
		grause = false;
		cnt = 0;
	}
}

//描画処理
void TitleDisp()
{
	gplib::font::Text(100, 100, 0.f, "Raiders of the Lost Gem", ARGB(255, 255, 0, 0), 2);
	if (grause)
	{
		gplib::font::Text(500, gplib::system::WINH / 2, 0.f, "Press A to Start", ARGB(255, 255, 0, 0), 2);
	}
}

void Title()
{
	if (gplib::stage::InitStage()){ TitleInit(); }
	TitleStep();
	TitleDisp();
}



