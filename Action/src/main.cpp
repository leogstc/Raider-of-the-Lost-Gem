//**************************************************************************************//
//		main.cpp
//**************************************************************************************//
#include "define.h"

using namespace gplib;

namespace gplib{
	namespace system{
		const char USERNAME[256] = "�����:���x��@Raiders of the Lost Gem";

		const int	WINW = 1280;										//720P�T�C�Y
		const int	WINH = 720;										//
		bool	WindowMode = true;								//�t���X�N���[���Ή�
		int		KeyboardMode = 1;									//pad disable:1 pad enable:0
	}
}

//---------------------------------------------------------------------------------------
//�Q�[�����[�v
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
//�Q�[���J�n���̃��[�h����
void LoadGame()
{

	AddFontResource("res/data/Pieces of Eight.ttf");

	font::Create(0, 30, "���C���I");
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
//�v���O�����S�̂̏�����
//�t�H���g�̍쐬�A���[�f�B���O��ʂŎg�p����摜�̓ǂݍ��݂Ȃ�
//�N�����Ɉ�񂾂��s�������͂����ōs��
//�ȉ��͊�{�I�ɕύX���Ȃ��B
//---------------------------------------------------------------------------------------
void InitGame()
{
	//2D�J�����̒����_
	camera::InitCamera(system::WINW / 2, system::WINH / 2, 0.0f, 0.0f);

	effect::LoadReses();
}

void system::OnCreate()
{
	LoadGame();
	InitGame();
}
//---------------------------------------------------------------------------------------
//�v���O�����S�̂̌�n��
//�I�����Ɉ�񂾂��s�������͂����ōs��
//---------------------------------------------------------------------------------------
void system::OnDestroy()
{
	RemoveFontResource("res/data/Pieces of Eight.ttf");
}
//======================================================================================//
//WINDOW�@���C��
//======================================================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return system::DoWindow(hInstance, hPreInst, lpszCmdLine, nCmdShow);
}


