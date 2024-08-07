#include "AppWindow.h"
#include "InputSystem.h"

int main()
{
	//Setup
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...)
	{
		return -1;
	}

	//App run
	try
	{
		AppWindow app;
		while (app.isRun());
	}
	catch(...)
	{
		GraphicsEngine::release();
		InputSystem::release();
		return -1;
	}

	//Release
	GraphicsEngine::release();
	InputSystem::release();
	return 0;
}