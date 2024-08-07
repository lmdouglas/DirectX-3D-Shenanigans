#pragma once
#include <Windows.h>
#include <d3d11.h>

class window
{
public:
	window();

	//Check if running
	bool isRun();

	//Functions
	RECT getClientWindowRect();

	//Events
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	//Release
	~window();
protected:
	HWND m_hwnd; //Window handle
	bool m_is_run; //Flags application is running
	bool m_is_init = false;

private:
	//Run
	bool broadcast();
};

