#include "AppWindow.h"

int main()
{
	AppWindow app;
	if (app.init())
	{
		while (app.isRun())
		{
			app.broadast();
		}
	}
	return 0;
}