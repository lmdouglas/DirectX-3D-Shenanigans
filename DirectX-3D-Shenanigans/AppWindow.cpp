#include "AppWindow.h"

AppWindow::AppWindow()
{

}

AppWindow::~AppWindow()
{

}

void AppWindow::onCreate()
{
	window::onCreate();
	GraphicsEngine::get()->init(); 
}

void AppWindow::onUpdate()
{
	window::onUpdate();
}

void AppWindow::onDestroy()
{
	window::onDestroy();

	GraphicsEngine::get()->release();
}
