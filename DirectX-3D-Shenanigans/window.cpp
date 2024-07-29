#include "window.h"

window* win = nullptr;

window::window()
{

}

//Callback function to process messages sent to the window
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		//Event fired when the window will be created
		win->onCreate();
		break;
	}

	case WM_DESTROY:
	{
		//Event fired when the window will be destroyed
		win->onDestroy();
		::PostQuitMessage(0);
		break;
	}

	default:
		return::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;
}

//Initialise Window
bool window::init()
{

	LPCWSTR classname = L"MyWindowClass";
	LPCWSTR blank = L"";

	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = classname;
	wc.lpszMenuName = blank;
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;


	//Register window. Ff fails, return false
	if (!::RegisterClassEx(&wc))
		return false;

	if (!win)
		win = this;

	//Create window
	m_hwnd = ::CreateWindowEx
	(
		WS_EX_OVERLAPPEDWINDOW,
		classname,
		L"DirectX Application",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL
	);


	//If window failed to create, return false
	if (!m_hwnd)
		return false;

	//Show window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);


	//Flag indicates window is initialised and running
	m_is_run = true;

	return true;
}

bool window::broadast()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	win->onUpdate();

	Sleep(0);

	return true;
}

bool window::release()
{
	//Destroy the window
	if (!::DestroyWindow(m_hwnd))
		return false;

	return true;
}

bool window::isRun()
{
	return m_is_run;
}

void window::onCreate()
{
}

void window::onUpdate()
{
}

void window::onDestroy()
{
	m_is_run = false;
}

window::~window()
{

}