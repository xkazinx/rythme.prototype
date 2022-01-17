#pragma once

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Tick() = 0;
	virtual void Render(sf::RenderTarget & tgt, const sf::RenderStates & states) const = 0;
	virtual bool HandleEvent(const sf::Event & ev) = 0;

	virtual void ProcessUI()
	{
	}
};
