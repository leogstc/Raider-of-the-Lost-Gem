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

//ゲームオーバー時に切り替わる時間を指定する。
//デフォルトは3秒InitGameOverで設定するので
//任意変更の場合はStartGameOverで処理の開始を行うこと
void gameover::SetTime(float time)
{
  gameover_.changeTime = time;
}

//終了処理開始
//ゲームオーバー成立時に呼び出し
void gameover::Start()
{
  gameover_.isOver = true;
  gameover_.count = gameover_.changeTime;
}

//終了処理初期化
void gameover::Init(float time = 3.0f)
{
  gameover_.isOver = false;
  gameover_.count = 0.f;
  gameover_.changeTime = time;
}

//終了処理判別
//キャラクタの移動抑止などに
bool gameover::isGameOver()
{
  return gameover_.isOver;
}

//終了時ステージ移行処理
//StartGameOver後に一定時間で画面切り替えを実行する。
void gameover::Step(int next)
{
  if (gameover_.isOver == true){
    gameover_.count += system::ONEFRAME_TIME;//60分の１sec
    if (gameover_.count >= gameover_.changeTime){
      gplib::stage::ChangeStage(next);
      gplib::bgm::AllStop();
      gplib::se::AllStop();
    }
  }
}

