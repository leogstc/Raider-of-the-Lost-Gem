#include "gplib.h"
#include "Gameover.h"

using namespace gplib;


struct GameOver
{
  float count;
  bool	isOver;
  float	changeTime;
};

static GameOver gameover_;

//�Q�[���I�[�o�[���ɐ؂�ւ�鎞�Ԃ��w�肷��B
//�f�t�H���g��3�bInitGameOver�Őݒ肷��̂�
//�C�ӕύX�̏ꍇ��StartGameOver�ŏ����̊J�n���s������
void gameover::SetTime(float time)
{
  gameover_.changeTime = time;
}

//�I�������J�n
//�Q�[���I�[�o�[�������ɌĂяo��
void gameover::Start()
{
  gameover_.isOver = true;
  gameover_.count = gameover_.changeTime;
}

//�I������������
void gameover::Init(float time = 3.0f)
{
  gameover_.isOver = false;
  gameover_.count = 0.f;
  gameover_.changeTime = time;
}

//�I����������
//�L�����N�^�̈ړ��}�~�Ȃǂ�
bool gameover::isGameOver()
{
  return gameover_.isOver;
}

//�I�����X�e�[�W�ڍs����
//StartGameOver��Ɉ�莞�Ԃŉ�ʐ؂�ւ������s����B
void gameover::Step(int next)
{
  if (gameover_.isOver == true){
    gameover_.count += system::ONEFRAME_TIME;//60���̂Psec
    if (gameover_.count >= gameover_.changeTime){
      gplib::stage::ChangeStage(next);
      gplib::bgm::AllStop();
      gplib::se::AllStop();
    }
  }
}

