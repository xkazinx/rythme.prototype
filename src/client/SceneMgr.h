#pragma once
#include "GameScene.h"

// The scenes, on kazilib's scene manager (lib::scene::Manager): one object per scene, Set enters it.
class SceneMgr final : public lib::scene::Manager
{
public:
	using ID = SceneID;

	SceneMgr();
	~SceneMgr();

	GameScene _game;

	void Set(ID id);
};
