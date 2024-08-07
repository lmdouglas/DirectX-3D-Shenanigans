#pragma once
#include "InputListener.h"
#include <unordered_set>
#include "Point.h"

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);


	void setCursorPosition(const Point& pos);
	void showCursor(bool show);

public:
	static InputSystem* get();
	static void create();
	static void release();

private:
	//std::map<InputListener*, InputListener*> m_map_listeners;
	std::unordered_set<InputListener*> m_set_listeners;
	unsigned char m_keys_state[256] = {}; //KeyboardState returns state of every key on keyboard
	unsigned char m_old_keys_state[256] = {}; //KeyboardState returns state of every key on keyboard

	Point m_old_mouse_pos;
	bool m_first_time = true;
	static InputSystem* m_system;
};

