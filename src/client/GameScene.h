#pragma once

class GameScene : public lib::scene::Base
{
public:
	GameScene();
	~GameScene() override;
	void OnHotpatch();

	static constexpr auto kID = SceneID::Game;

	void Tick() override;

	void Render() override;
	void PressButton(const ButtonIdx idx, const bool press);
	// ev: the sf::Event the window delivered (lib::scene::Base passes it as void *)
	void HandleEvent(const void * ev) override;
	void ProcessUI();

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

private:
	bool HandleKey(const sf::Event & ev);
};