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
		//collected here...
		window* win = (window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		//and then stored for later lookup
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)win);
		win->setHWND(hwnd);
		win->onCreate();
		break;
	}

	case WM_DESTROY:
	{
		//Event fired when the window will be destroyed
		window* win = (window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		win->onDestroy();
		::PostQuitMessage(0);
		break;
	}

	case WM_SETFOCUS:
	{
		//Event fired when the window is in focus
		window* win = (window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		win->onFocus();
		::PostQuitMessage(0);
		break;
	}

	case WM_KILLFOCUS:
	{
		//Event fired when the window is not in focus
		window* win = (window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		win->onKillFocus();
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
		this
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

	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}

bool window::release()
{
	//Destroy the window
	if (!::DestroyWindow(m_hwnd))
		return false;

	return true;
}

RECT window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void window::setHWND(HWND hwnd)
{
	m_hwnd = hwnd;
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

void window::onFocus()
{
}

void window::onKillFocus()
{
}

window::~window()
{

}