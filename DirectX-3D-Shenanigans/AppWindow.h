#pragma once
#include "window.h"

class AppWindow : public window
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
};

