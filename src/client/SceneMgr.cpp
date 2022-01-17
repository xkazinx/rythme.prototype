#include "stdafx.h"
#include "SceneMgr.h"
#include "GameScene.h"

SceneMgr::SceneMgr()
{
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
	case Game:		_ptr = make_unique<GameScene>();	break;
	default: 
		break;
	}
}

void SceneMgr::Render(sf::RenderTarget& tgt, const sf::RenderStates& states) const
{
	_ptr->Render(tgt, states);
	_ptr->ProcessUI();
}

void SceneMgr::Tick()
{
	
}

bool SceneMgr::HandleEvent(const sf::Event& ev) const
{
	return _ptr->HandleEvent(ev);
}
