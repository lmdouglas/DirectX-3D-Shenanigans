#pragma once
#include <Windows.h>

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

	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();

	~window();
protected:
	HWND m_hwnd;
	bool m_is_run;
};

