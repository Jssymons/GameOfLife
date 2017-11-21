#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <stack>
#include <memory>
#include "Grid.h"

class Application
{
public:
	Application();
	void RunMainGameLoop();
private:
	void Input();
	bool doSimulate = false;
	bool doStep = false;
	bool stepReleased = true;
	Grid grid;
};

#endif //APPLICATION_H_INCLUDED