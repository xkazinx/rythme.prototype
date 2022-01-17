#include "stdafx.h"
// Self
#include "App.h"
#include "SceneMgr.h"
#include "CommandMgr.h"

// Lib
#include <LPP_API.h>

void lppPrePatch()
{
}

void lppPostPatch()
{
	Print("LivePP: Patch");
	g_cmd->Setup();
}

LPP_PREPATCH_HOOK(lppPrePatch);
LPP_POSTPATCH_HOOK(lppPostPatch);

App::App() // NOLINT(cppcoreguidelines-pro-type-member-init)
{
	_livePP = lpp::lppLoadAndRegister(L"D:\\LIBPATH\\LivePP", "Kazimgr");
	lpp::lppEnableAllCallingModulesAsync(_livePP);

	g_app = this;
	Global::Initialize();

	InitializeWindow(g_res_x, g_res_y);

	ImGui::SFML::Init(_window);
}

App::~App() // NOLINT(modernize-use-equals-default)
{
	ImGui::SFML::Shutdown();
	Global::Deinitialize();

	lpp::lppShutdown(_livePP);
	::FreeLibrary(_livePP);
}

void App::ToggleConsole()
{
	const HWND hwnd  = GetConsoleWindow();
	int        state = SW_SHOW;

	if(::IsWindowVisible(hwnd))
		state = SW_HIDE;

	ShowWindow(hwnd, state);

	//if(state == SW_SHOW)
		//RequestFocus(hwnd);
}

void App::InitializeWindow([[maybe_unused]] const u16 res_x, [[maybe_unused]] u16 res_y)
{
	// OpenGL settings
	ContextSettings settings;
	{
		settings.depthBits         = 0;
		settings.stencilBits       = 0;
		settings.antialiasingLevel = 0;
		settings.majorVersion      = 3;
		settings.minorVersion      = 2;
	}

	if(static bool first = true;
		first || _window.getSize().x != g_res_x)
	{
		first = false;
		_window.create(VideoMode(g_res_x, g_res_y), kAppName, sf::Style::None, settings);
	}

	_window.setVisible(res_x != 0);

	// https://www.sfml-dev.org/tutorials/2.5/window-opengl.php
	_window.setActive(res_x != 0);
	_window.setVerticalSyncEnabled(false); //res_x != 0);
	_window.setFramerateLimit(60);

	AdjustViewport(_window.getSize().x, _window.getSize().y);
}

void App::AdjustViewport(const GLsizei & x, const GLsizei & y)
{
	glViewport(0, 0, x, y);
}

int App::Tick()
{
	sf::RenderStates states;
	sf::Clock        delta_clock;

	while(_window.isOpen())
	{
		// Events
		{
			if(!HandleEvents())
			{
				Log::Debug("if (!HandleEvents())");
				break;
			}
		}

		// Update
		{
			g_scene->Tick();
			ImGui::SFML::Update(_window, delta_clock.restart());
		}

		// Render
		{
			_window.clear(sf::Color::Black);
			{
				g_scene->Render(_window, states);
				ImGui::SFML::Render(_window);
			}
			_window.display();
		}

		this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}

bool App::HandleEvents()
{
	sf::Event ev;

	while(_window.pollEvent(ev))
	{
		ImGui::SFML::ProcessEvent(ev);

		if(ev.type == sf::Event::Closed)
		{
			_window.close();
			return false;
		}

		if(ev.type == sf::Event::Resized)
		{
			AdjustViewport(ev.size.width, ev.size.height);
		}
		else
		{
			g_scene->HandleEvent(ev);
		}
	}

	return true;
}