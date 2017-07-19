#pragma once
#include "define.h"
#include <sstream>
#include <fstream>
#include <iomanip>

const int GEMMAX = 20;

enum ChipNo
{
	chip_way = 0,
	chip_GateO = 2,
	chip_Gem = 4,
	chip_GateC = 5,
	chip_Key = 10
};

struct Gem
{
	objectbase::ObjData obj_;
	RECT rt_;
	ChipNo no;
};

struct Key
{
	objectbase::ObjData obj_;
	RECT rt_;
	ChipNo no;
	bool open;
};

extern Key gate;
extern std::vector<std::vector<int>> map;

void InitMap();
void DispMap();
POINT CheckMap(int x, int y, int w, int h);
void InitKey();
void DispKey();
void StepKey();
void InitGem();
void DispGem();
void StepGem();
void InitGate();
void DispGate();
void CreateGate(int x, int y);
void CreateKey(int x, int y);
void CreateGem(int x, int y);
bool IsGetGem();
