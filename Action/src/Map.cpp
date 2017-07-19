#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "StageControl.h"
#include "config.h"
using namespace gplib;

std::vector<std::vector<int>> map;

Gem gems[GEMMAX];
Key key, gate;

//ファイルを 1 行ずつ読み込む
std::vector<int> getline(std::ifstream& f) 
{
	std::string line;
	std::vector<int> ret;
	std::getline(f, line);
	std::istringstream ss(line);
	while (true)
	{
		std::string field;
		std::getline(ss, field, ','); 
		if (field == "")
			break;
		ret.push_back(std::stoi(field));
	}
	return ret;
}

void InitMap()
{
	map.clear();
	std::ostringstream filename; 
	filename << "res/data/stage1.txt"; //ファイル読み込み
	std::ifstream f(filename.str());
	for (int y = 0; ; ++y)
	{
		auto chips = getline(f);
		if (chips.size() == 0)
			break;
		map.push_back(chips);

		for (int x = 0; x < chips.size(); ++x)  //stage作成
		{
			map[y][x] = chips[x] % 100;
			switch (chips[x] / 100)
			{
			case ChipNo::chip_Gem:
				CreateGem(x, y);
				break;
			case 2:
				CreateEnemy(x, y);
				break;
			case ChipNo::chip_Key:
				CreateKey(x, y);
				break;
			case ChipNo::chip_GateC:
				CreateGate(x, y);
				break;

			}
		}
	}
}

void DispMap()
{
	for (int y = 0; y < map.size(); ++y)
	{
		for (int x = 0; x < map[y].size(); ++x)
		{
			int chipNo = map[y][x] % 100;
			draw::GraphLeftTop(
				config.MAPCELLSZ * x, config.MAPCELLSZ * y, 0.5f,
				"mapchip",
				config.MAPCELLSZ * (chipNo % 5),
				config.MAPCELLSZ * (chipNo / 5), config.MAPCELLSZ, config.MAPCELLSZ);
		}
	}
}

POINT CheckMap(int x, int y, int w, int h) //mapの当たり判定
{
	int OffsetS = 1;
	int OffsetE = 1;

	POINT pt[] =										//キャラクター移動後次ぎの位置
	{
		{ x - w / 2 + OffsetS, y - h / 2 + OffsetS },
		{ x + w / 2 - OffsetE, y - h / 2 + OffsetS },
		{ x - w / 2 + OffsetE, y + h / 2 - OffsetS },
		{ x + w / 2 - OffsetE, y + h / 2 - OffsetS }
	};
	POINT res = { x, y };
	for (int i = 0; i < 4; ++i)
	{
		if (map[pt[i].y / config.MAPCELLSZ][pt[i].x / config.MAPCELLSZ] == 1) //壁、もしくは地面に当たったら
		{
			res.x = pt[i].x / config.MAPCELLSZ;
			res.y = pt[i].y / config.MAPCELLSZ;
			return res;
		}
	}
	res.x = -1;
	res.y = -1;
	return res;
}

void InitKey()
{
	key.obj_.setUseOff();
}

void DispKey()
{
	objectbase::ObjData obj = key.obj_;
	if (obj.checkUse())
	{
		draw::Graph(obj.pos.x, obj.pos.y, 0.5f,
			"mapchip",
			config.MAPCELLSZ*(key.no % 5),
			config.MAPCELLSZ*(key.no / 5),
			config.MAPCELLSZ, config.MAPCELLSZ);
	}
}

void StepKey()
{
	if (IsGetGem()) //宝石全部取ったら
	{
		if (gate.no == chip_GateC)
		{
			key.obj_.setUseOn(); //鍵を出見
		}
	}

	if (key.obj_.checkUse())
	{
		if (key.obj_.checkHit(player.obj_))
		{
			key.obj_.setUseOff();
			key.no = chip_way;
			gate.no = chip_GateO;
			gate.open = true;
			se::Play("open");
			bgm::Stop("stage_bgm");
			bgm::Play("last");
		}
	}
}

void InitGem()
{
	for (auto& gem : gems)
	{
		gem.obj_.setUseOff();
	}
}

void DispGem() 
{
	for (auto& gem : gems)
	{
		objectbase::ObjData obj = gem.obj_;
		if (obj.checkUse())
		{
			draw::Graph(obj.pos.x, obj.pos.y, 0.5f,
				"mapchip",
				config.MAPCELLSZ*(gem.no % 5),
				config.MAPCELLSZ*(gem.no / 5),
				config.MAPCELLSZ, config.MAPCELLSZ);
		}
	}
}

void StepGem()
{
	for (auto& gem : gems)
	{
		objectbase::ObjData obj = gem.obj_;
		if (obj.checkUse())
		{
			if (obj.checkHit(player.obj_))
			{
				se::Play("get");
				gem.obj_.setUseOff();
				gem.no = chip_way;
			}
		}
	}
}

void InitGate()
{
	gate.obj_.setUseOff();
	gate.open = false;
}

void DispGate()
{
	objectbase::ObjData obj = gate.obj_;
	if (obj.checkUse())
	{
		draw::Graph(obj.pos.x, obj.pos.y, 0.5f,
			"mapchip",
			config.MAPCELLSZ*(gate.no % 5),
			config.MAPCELLSZ*(gate.no / 5),
			config.MAPCELLSZ, config.MAPCELLSZ);
	}
}

void CreateGate(int x, int y)
{
	objectbase::ObjData& obj = gate.obj_;
	if (!obj.checkUse())
	{
		obj.init
		(x*config.MAPCELLSZ + (config.MAPCELLSZ / 2), y*config.MAPCELLSZ + (config.MAPCELLSZ / 2), 0.0f,
			0.0f, 0.0f, 0.0f,
			"mapchip", 0, 0, 0,
			config.MAPCELLSZ, config.MAPCELLSZ);
		gate.no = chip_GateC;
		obj.setUseOn();
	}
}

void CreateKey(int x, int y)
{
	objectbase::ObjData& obj = key.obj_;
	if (!obj.checkUse())
	{
		obj.init
		(x*config.MAPCELLSZ + (config.MAPCELLSZ / 2), y*config.MAPCELLSZ + (config.MAPCELLSZ / 2), 0.0f,
			0.0f, 0.0f, 0.0f,
			"mapchip", 0, 0, 0,
			config.MAPCELLSZ, config.MAPCELLSZ);
		key.no = chip_Key;

	}
}

void CreateGem(int x, int y)
{
	for (auto& gem : gems)
	{
		objectbase::ObjData& obj = gem.obj_;
		if (!obj.checkUse())
		{
			obj.init
			(x*config.MAPCELLSZ + (config.MAPCELLSZ / 2), y*config.MAPCELLSZ + (config.MAPCELLSZ / 2), 0.0f,
				0.0f, 0.0f, 0.0f,
				"mapchip", 0, 0, 0,
				config.MAPCELLSZ, config.MAPCELLSZ);
			gem.no = chip_Gem;
			obj.setUseOn();
			break;
		}
	}
}

bool IsGetGem() //宝石全部取ったら
{
	for (auto& gem : gems)
	{
		if (gem.no == ChipNo::chip_Gem)
		{
			return false;
		}
	}
	return true;
}