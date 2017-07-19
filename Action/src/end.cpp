
#include "define.h"

using namespace gplib;

//---------------------------------
// End
//---------------------------------
//‰Šú‰»ˆ—		
void EndInit()
{
	camera::SetLookAt(system::WINW/2.f, system::WINH/2.f);
	se::Play("gameover");
}

//ƒtƒŒ[ƒ€ˆ—
void EndStep()
{
		if (input::CheckPush(input::KEY_BTN0))
		{
			stage::ChangeStage(TITLESCENE);
			se::AllStop();
		}
}

//•`‰æˆ—
void EndDisp()
{
	//gplib::font::Text(100, 100, 0.f, "Game Over", ARGB(255, 255, 0, 0), 2);
	gplib::draw::GraphLeftTop(0, 0, 0.5f, "gameover", 1, 1, 1280, 720);
	gplib::font::Text(500, gplib::system::WINH / 2 + 100, 0.f, "Press A to Continue", ARGB(255, 255, 0, 0), 2);
}

void End()
{
	if (gplib::stage::InitStage()){ EndInit(); }
	EndStep();
	EndDisp();
}
