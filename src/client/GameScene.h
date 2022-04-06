#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	void OnHotpatch();

	static constexpr auto kID = SceneID::Game;

	void Tick() override;
	
	void Render(sf::RenderTarget& tgt, const sf::RenderStates& states) const override;
	void PressButton(ButtonIdx idx, bool press);
	bool HandleEvent(const sf::Event& ev) override;
	void ProcessUI() override;

	bool ProcessCommand();
	string _command;
	bool _success = false;
	bool _just_opened = true;

	sf::RectangleShape _board;

	class Button
	{
	public:
		Button() = default;
		Button(ButtonIdx idx) : _idx(idx)
		{
			Release();
		}

		ButtonIdx _idx;
		sf::CircleShape _shape;
		
		void Press()
		{
			_pressed = true;

			_shape.setOutlineColor(sf::Color::Yellow);
		}

		void Release()
		{/*
			if (!_pressed)
			{
				PrintError("Trying to Release a non-pressed Button: {}", _idx);
				return;
			}
			*/

			_pressed = false;
			_shape.setOutlineColor(sf::Color(150, 150, 150));
		}

		bool _pressed = false;
	};

	array<Button, kButtonMax> _btn;
};