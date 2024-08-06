#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_first_time)
	{
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if (current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y)
	{
		//Mouse has moved
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->onMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);


	if (::GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			// KEY IS DOWN
			if (m_keys_state[i] & 0x88)
			{
				std::unordered_set<InputListener*, InputListener*>::iterator it = m_set_listeners.begin();
				
				while (it != m_set_listeners.end())
				{

					//Check mouse buttons
					if (i == VK_LBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else
					{
						(*it)->onKeyDown(i);
					}
					++it;
				}
			}
			else //KEY IS UP
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					std::unordered_set<InputListener*, InputListener*>::iterator it = m_set_listeners.begin();

					while (it != m_set_listeners.end())
					{
						//Check mouse buttons
						if (i == VK_LBUTTON)
						{
							if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == VK_RBUTTON)
						{
							if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else
						{
							(*it)->onKeyUp(i);
						}
						++it;
					}
				}

			}
		}
		//Store current keys state to old keys state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
	//m_map_listeners.insert
	//(std::make_pair<InputListener*, InputListener*>
	//	(std::forward<InputListener*>(listener), std::forward<InputListener*>(listener))
	//);
	return;
}

void InputSystem::removeListener(InputListener* listener)
{
	m_set_listeners.erase(listener);
	//std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.find(listener);
	//if (it != m_map_listeners.end())
	//{
	//	m_map_listeners.erase(it);
	//}
	return;
}

void InputSystem::setCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
