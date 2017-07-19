
#include "define.h"

using namespace gplib;
int meuncnt = 0;
bool meungrause = false;
//---------------------------------
// Meun
//---------------------------------
//初期化処理		
void MeunInit()
{

}

//フレーム処理
void MeunStep()
{
	meuncnt++;
	if (input::CheckPush(input::KEY_BTN0))
	{
		bgm::Stop("title_bgm");
		se::Play("start");
		stage::ChangeStage(STAGE1SCENE);

	}
	if (meuncnt == 10 && meuncnt != 0)
	{
		meungrause = true;
	}
	if (meuncnt == 50 && meuncnt != 0)
	{
		meungrause = false;
		meuncnt = 0;
	}
}

//描画処理
void MeunDisp()
{
	gplib::font::Text(500, 25, 0.f, "How To Play", ARGB(255, 255, 0, 0), 2);
	gplib::font::Text(50, 100, 0.f, "基本操作：   左移動：＜－　右移動：ー＞  　Ａキー：ジャンプ", ARGB(255, 255, 0, 0), 4);
	gplib::font::Text(50, 200, 0.f, "ルール：ルールは簡単、全部の宝石を取れば鍵が現れる、", ARGB(255, 255, 0, 0), 4);
	gplib::font::Text(125, 250, 0.f, "鍵を取ればドアのロックは解錠する、ドアに到達するとゲームクリア。", ARGB(255, 255, 0, 0), 4); 
	gplib::font::Text(50, 400, 0.f, "亡霊を触れると即死、穴に落ちるも即死", ARGB(255, 255, 0, 0), 4);
	if (meungrause)
	{
		gplib::font::Text(500, gplib::system::WINH / 2 + 200, 0.f, "Press A to Start", ARGB(255, 255, 0, 0), 2);
	}
}

void Meun()
{
	if (gplib::stage::InitStage()) { MeunInit(); }
	MeunStep();
	MeunDisp();
}