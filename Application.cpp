#include "Application.h"
#include "Display.h"
#include "Constants.h"
#include <GL\glfw3.h>

Application::Application()
{ }

void Application::RunMainGameLoop() {
	do {
		Input();

		if (doSimulate || doStep)
			grid.Update();

		Display::Get().Clear();
		grid.Draw();
		Display::Get().Update();
	} 
	while (!Display::Get().CheckKeyPress(GLFW_KEY_ESCAPE) && Display::Get().WindowShouldClose());
}

void Application::Input()
{
	static int prevIdx;
	double xPos, yPos;
	Display::Get().GetMouseInput(xPos, yPos);

	if (xPos <= 0)
		return;
	if (xPos >= SCREEN_WIDTH)
		return;
	if (yPos <= 0)
		return;
	if (yPos >= SCREEN_HEIGHT)
		return;

	int x = xPos / (SCREEN_WIDTH / GRID_WIDTH);
	int y = (SCREEN_HEIGHT - yPos) / (SCREEN_HEIGHT / GRID_HEIGHT);
	int mousePosIdx = x + y * GRID_WIDTH;
	grid.SetCellColour(mousePosIdx, 1.0f, 0.0f, 0.0f, false);

	bool lmb = Display::Get().CheckMouseButton(GLFW_MOUSE_BUTTON_LEFT);
	if (lmb)
 		grid.LiveCell(mousePosIdx);

	bool rmb = Display::Get().CheckMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
	if (rmb)
		grid.KillCell(mousePosIdx);

	if (prevIdx != mousePosIdx)
	{
		if (!lmb)
			grid.ResetCellColour(prevIdx);
		if (!rmb)
			grid.ResetCellColour(prevIdx);
	}

	doStep = false;
	if (Display::Get().CheckKeyPress(GLFW_KEY_S) && stepReleased) {
		doStep = true;
		stepReleased = false;
	}

	if (Display::Get().CheckKeyRelease(GLFW_KEY_S))
		stepReleased = true;
	
	if (Display::Get().CheckKeyPress(GLFW_KEY_SPACE))
		doSimulate = true;

	if (Display::Get().CheckKeyRelease(GLFW_KEY_SPACE))
		doSimulate = false;

	if (Display::Get().CheckKeyPress(GLFW_KEY_R))
		grid.ClearGrid();

	prevIdx = mousePosIdx;
}