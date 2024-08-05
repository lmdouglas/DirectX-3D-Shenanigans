#pragma once
#include "InputListener.h"
#include <unordered_set>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

public:
	static InputSystem* get();

private:
	//std::map<InputListener*, InputListener*> m_map_listeners;
	std::unordered_set<InputListener*> m_set_listeners;
	unsigned char m_keys_state[256] = {}; //KeyboardState returns state of every key on keyboard
	unsigned char m_old_keys_state[256] = {}; //KeyboardState returns state of every key on keyboard
};

