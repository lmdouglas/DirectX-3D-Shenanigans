#pragma once
#include <Windows.h>
#include <d3d11.h>

class window
{
public:
	window();
	//initialise the window
	bool init();
	bool broadast();
	//release the window
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();

	virtual void onFocus();
	virtual void onKillFocus();

	~window();
protected:
	HWND m_hwnd; //Window handle
	bool m_is_run; //Flags application is running
};

