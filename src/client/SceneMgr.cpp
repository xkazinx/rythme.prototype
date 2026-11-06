#include "pch.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr()
{
	Add(_game);
	Set(ID::Game);
}

SceneMgr::~SceneMgr()
{

}

void SceneMgr::Set(ID id)
{
	using enum ID;
	switch(id)
	{
	case Game:		Manager::Set(_game);	break;
	default:
		PrintError("SceneMgr::Set: no scene for {}", Format(id));
		break;
	}
}
