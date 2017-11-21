#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED
#include <GL\glfw3.h>


class Display
{
	public:
		static Display& Get();
		void Clear();
		void Update();
		void GetMouseInput(double& xPos, double& yPos);
		void SetMousePosition(double x, double y);
		void HandleMouseButton(int button, int action, int mods);
		bool CheckMouseButton(int key);
		bool CheckKeyPress(int key);
		bool CheckKeyRelease(int key);
		bool WindowShouldClose();
	private:
		bool lmbPressed;
		bool rmbPressed;
		Display();
};

#endif //DISPLAY_H_INCLUDED