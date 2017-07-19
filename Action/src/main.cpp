//**************************************************************************************//
//		main.cpp
//**************************************************************************************//
#include "define.h"

using namespace gplib;

namespace gplib{
	namespace system{
		const char USERNAME[256] = "製作者:黄駿庭　Raiders of the Lost Gem";

		const int	WINW = 1280;										//720Pサイズ
		const int	WINH = 720;										//
		bool	WindowMode = true;								//フルスクリーン対応
		int		KeyboardMode = 1;									//pad disable:1 pad enable:0
	}
}

//---------------------------------------------------------------------------------------
//ゲームループ
//---------------------------------------------------------------------------------------
void system::GameLoop()
{
	switch (stage::CheckStage()){
	case TITLESCENE:	Title();	break;
	case MEUNSCENE:		Meun();		break;
	case STAGE1SCENE:	Stage1();	break;
	case ENDSCENE:		End();		break;
	}
	effect::Step();
	easing::Step();
	effect::Draw();

}

//---------------------------------------------------------------------------------------
//ゲーム開始時のロード処理
void LoadGame()
{

	AddFontResource("res/data/Pieces of Eight.ttf");

	font::Create(0, 30, "メイリオ");
	font::Create(1, 50, "Buxton Sketch");
	font::Create(2, 50, "Pieces of Eight");
	font::Create(3, 30, "Pieces of Eight");
	font::Create(4, 30, "Buxton Sketch");
	

	draw::LoadObject("mapchip", "res/gra/2DBlock.png");
	draw::LoadObject("player", "res/gra/chara2.png");
	draw::LoadObject("jewel", "res/gra/jewel.png");
	draw::LoadObject("enemy", "res/gra/enemy1.png");
	draw::LoadObject("gameover", "res/gra/game_over.jpg");
	
	bgm::LoadFile("title_bgm", "res/snd/Far_The_Days_Come.mp3");
	bgm::LoadFile("stage_bgm", "res/snd/Back_of_the_Room_Hang.mp3");
	bgm::LoadFile("last", "res/snd/With_S2.wav");
	se::LoadFile("dead", "res/snd/scream-woman1.wav");
	se::LoadFile("gameover", "res/snd/fear1.wav");
	se::LoadFile("open", "res/snd/open.wav");
	se::LoadFile("yaa", "res/snd/yaa.wav");
	se::LoadFile("get", "res/snd/error.wav");
	se::LoadFile("start", "res/snd/start.wav");
}

//---------------------------------------------------------------------------------------
//プログラム全体の初期化
//フォントの作成、ローディング画面で使用する画像の読み込みなど
//起動時に一回だけ行う処理はここで行う
//以下は基本的に変更しない。
//---------------------------------------------------------------------------------------
void InitGame()
{
	//2Dカメラの注視点
	camera::InitCamera(system::WINW / 2, system::WINH / 2, 0.0f, 0.0f);

	effect::LoadReses();
}

void system::OnCreate()
{
	LoadGame();
	InitGame();
}
//---------------------------------------------------------------------------------------
//プログラム全体の後始末
//終了時に一回だけ行う処理はここで行う
//---------------------------------------------------------------------------------------
void system::OnDestroy()
{
	RemoveFontResource("res/data/Pieces of Eight.ttf");
}
//======================================================================================//
//WINDOW　メイン
//======================================================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return system::DoWindow(hInstance, hPreInst, lpszCmdLine, nCmdShow);
}


