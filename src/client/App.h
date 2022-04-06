#pragma once

class App final : kazin::app::Base
{
public:
	App();
	~App() override;

	RenderWindow _window{};

	int  Tick();
	bool HandleEvents();

	// Self Window
	void ToggleConsole();
	
private:
	void InitializeWindow(u16 res_x, u16 res_y);
	static void AdjustViewport(const GLsizei & x, const GLsizei & y);
};