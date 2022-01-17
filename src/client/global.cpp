#include "stdafx.h"
#include "Global.h"
#include "SceneMgr.h"
#include "CommandMgr.h"

namespace Global
{
	App* g_app;
	unique_ptr<Random> g_rand;
	unique_ptr<SceneMgr> g_scene;
	unique_ptr<CommandMgr> g_cmd;
	u16 g_res_x = 1024, g_res_y = 768;

	void Initialize()
	{
		// unordered
		g_rand  = make_unique<Random>();
		g_scene = make_unique<SceneMgr>();
		g_cmd   = make_unique<CommandMgr>();

		// ordered
	}

	void Deinitialize()
	{

	}
}

namespace font
{
	FontNames kNames = 
	{
		"arial.ttf",
	};
}