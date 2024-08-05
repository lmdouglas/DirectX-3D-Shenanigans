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
					(*it)->onKeyDown(i);
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
						(*it)->onKeyUp(i);
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

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
