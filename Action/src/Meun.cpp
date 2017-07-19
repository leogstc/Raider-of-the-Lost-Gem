
#include "define.h"

using namespace gplib;
int meuncnt = 0;
bool meungrause = false;
//---------------------------------
// Meun
//---------------------------------
//����������		
void MeunInit()
{

}

//�t���[������
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

//�`�揈��
void MeunDisp()
{
	gplib::font::Text(500, 25, 0.f, "How To Play", ARGB(255, 255, 0, 0), 2);
	gplib::font::Text(50, 100, 0.f, "��{����F   ���ړ��F���|�@�E�ړ��F�[��  �@�`�L�[�F�W�����v", ARGB(255, 255, 0, 0), 4);
	gplib::font::Text(50, 200, 0.f, "���[���F���[���͊ȒP�A�S���̕�΂����Ό��������A", ARGB(255, 255, 0, 0), 4);
	gplib::font::Text(125, 250, 0.f, "�������΃h�A�̃��b�N�͉�������A�h�A�ɓ��B����ƃQ�[���N���A�B", ARGB(255, 255, 0, 0), 4); 
	gplib::font::Text(50, 400, 0.f, "�S���G���Ƒ����A���ɗ����������", ARGB(255, 255, 0, 0), 4);
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