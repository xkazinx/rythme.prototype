#include "pch.h"
#include "GameScene.h"
#include "App.h"
#include "CommandMgr.h"

GameScene::GameScene() : 
	_command(kCommandSz, '\0')
{
	OnHotpatch();
}

GameScene::~GameScene()
{

}

void GameScene::OnHotpatch()
{
	_board.setFillColor(sf::Color(50, 50, 50));
	_board.setOutlineColor(sf::Color::Yellow);
	_board.setOutlineThickness(1.f);

	
	_board.setSize({g_res_x - (g_res_x / 4.f), (float)g_res_y });
	auto sz = _board.getSize();

	_board.setPosition({(g_res_x / 2.f) - (sz.x / 2.f), 0.f});

	auto pos = _board.getPosition();
	for(szt i = 0; i < _btn.size(); ++i)
	{
		auto idx = (u8)i;
		auto & b = _btn[i];
		b= { (u8)idx++ };
		b._shape.setFillColor(sf::Color(255 / idx, 255 / idx, 255 / idx));
		--idx;
		
		b._shape.setOutlineThickness(2.f);
		auto tick = b._shape.getOutlineThickness();
		auto tick2 = tick * 2;
		b._shape.setRadius((sz.x - (tick2 * kButtonMax)) / kButtonMax / 2.f);
		auto r = b._shape.getRadius() + tick;
		auto r2 = r * 2.f;

		b._shape.setPosition({pos.x + (r2 * idx), g_res_y - r2});
	}
}

void GameScene::Tick()
{

}

void GameScene::ProcessUI()
{
	/*ImGui::Begin("Test UI", nullptr, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

	if (ImGui::GetWindowSize().x != g_res_x)
	{
		ImGui::SetWindowPos({0, 0});
		ImGui::SetWindowSize({(float)g_res_x, (float)g_res_y});
	}

	ImGui::End();*/
}

bool GameScene::ProcessCommand()
{
	const auto cmd = CommandMgr::Process(_command);
	const auto res = cmd != nullptr;

	PrintDebug("Process command \"{}\": {}", _command.c_str(), 
		Format(res, FormatType::bool_State) // #todo Why doesn't _command.c_str() work as argument?
	);

	if(res)
	{
		_just_opened = false;
	}

	return res;
}

void GameScene::Render()
{
	auto & tgt = g_app->_window;
	tgt.draw(_board);

	for(const auto & btn : _btn)
	{
		tgt.draw(btn._shape);
	}

	ProcessUI();
}

void GameScene::PressButton(const ButtonIdx idx, const bool press)
{
	PrintDebug("Pressed {} press {}", static_cast<unsigned>(idx), press);
	if (press)
		_btn[idx].Press();
	else _btn[idx].Release();
}

void GameScene::HandleEvent(const void * const ev)
{
	if (ev)
		HandleKey(*static_cast<const sf::Event *>(ev));
}

bool GameScene::HandleKey(const sf::Event & ev)
{
	const auto * const pressed  = ev.getIf<sf::Event::KeyPressed>();
	const auto * const released = ev.getIf<sf::Event::KeyReleased>();

	if (pressed)
	{
		if (pressed->code == Key::Escape)
		{

			return true;
		}
		else if (pressed->code == Key::D && sf::Keyboard::isKeyPressed(Key::RAlt))
		{
			g_app->ToggleConsole();
			return true;
		}
	}

	if (pressed || released)
	{
		const Key code = pressed ? pressed->code : released->code;
		if (code >= Key::Num0 && code <= Key::Num9)
		{
			PressButton(ButtonIdx(code == Key::Num0 ? 9 : Under(code) - Under(Key::Num0) - 1), pressed != nullptr);
			return true;
		}
	}

	return false;
}

