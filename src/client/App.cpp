#include "pch.h"

// Self
#include "App.h"
#include "SceneMgr.h"
#include "CommandMgr.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

App::App()
{
	g_app = this;
#ifndef __EMSCRIPTEN__
	SetConsoleTitleA(kAppName);
#endif
	Global::Initialize();

	InitializeWindow(g_res_x, g_res_y);

	ImGui::SFML::Init(_window);
}

App::~App()
{
	ImGui::SFML::Shutdown();
	Global::Deinitialize();
}

void App::ToggleConsole()
{
#ifndef __EMSCRIPTEN__
	const HWND hwnd  = GetConsoleWindow();
	int        state = SW_SHOW;

	if(::IsWindowVisible(hwnd))
		state = SW_HIDE;

	ShowWindow(hwnd, state);

	//if(state == SW_SHOW)
		//RequestFocus(hwnd);
#endif
}

void App::InitializeWindow([[maybe_unused]] const u16 res_x, [[maybe_unused]] const u16 res_y)
{
#ifdef __EMSCRIPTEN__
	// The page sizes the canvas (kazilib web/shell.html) and the browser paces the frames:
	// display() waits for its next one. The controls go under the game, on the page.
	_window.create(VideoMode({g_res_x, g_res_y}), kAppName, sf::Style::Titlebar | sf::Style::Close);
	emscripten_run_script("document.getElementById('bar').insertAdjacentHTML('beforeend', '<span id=\"hint\">Hold 1 to 9 and 0: light the frets.</span>')");
	Print("Game ready: {} frets", kButtonMax);
#else
	// OpenGL settings
	ContextSettings settings;
	{
		settings.depthBits         = 0;
		settings.stencilBits       = 0;
		settings.antiAliasingLevel = 0;
		settings.majorVersion      = 3;
		settings.minorVersion      = 2;
	}

	if(static bool first = true;
		first || _window.getSize().x != g_res_x)
	{
		first = false;
		_window.create(VideoMode({g_res_x, g_res_y}), kAppName, sf::Style::None, sf::State::Windowed, settings);
	}

	_window.setVisible(res_x != 0);

	// https://www.sfml-dev.org/tutorials/3.0/window/opengl/
	(void)_window.setActive(res_x != 0);
	_window.setVerticalSyncEnabled(false); //res_x != 0);
	_window.setFramerateLimit(60);

	AdjustViewport(_window.getSize().x, _window.getSize().y);
#endif
}

#ifndef __EMSCRIPTEN__
void App::AdjustViewport(const GLsizei x, const GLsizei y)
{
	glViewport(0, 0, x, y);
}
#endif

int App::Tick()
{
	sf::Clock delta_clock;

	while(_window.isOpen())
	{
		// Events
		{
			if(!HandleEvents())
			{
				PrintDebug("if (!HandleEvents())");
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
				g_scene->Render();
				ImGui::SFML::Render(_window);
			}
			_window.display();
		}

#ifndef __EMSCRIPTEN__
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
	}

	return 0;
}

bool App::HandleEvents()
{
	while(const std::optional ev = _window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(*ev);

		if(ev->is<sf::Event::Closed>())
		{
			_window.close();
			return false;
		}

#ifndef __EMSCRIPTEN__
		if(const auto * const resized = ev->getIf<sf::Event::Resized>())
		{
			AdjustViewport(resized->size.x, resized->size.y);
		}
		else
#endif
		{
			g_scene->HandleEvent(&*ev);
		}
	}

	return true;
}
