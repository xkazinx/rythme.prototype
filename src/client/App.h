#pragma once

class App final
{
public:
	App();
	~App();

	RenderWindow _window{};

	int  Tick();
	bool HandleEvents();

	// Self Window
	void ToggleConsole();

private:
	void InitializeWindow(u16 res_x, u16 res_y);
#ifndef __EMSCRIPTEN__
	static void AdjustViewport(const GLsizei x, const GLsizei y);
#endif
};
