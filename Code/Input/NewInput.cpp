#include "NewInput.h"

NewInput::NewInput(const std::vector<KeyCode>& keyCodes)
    : m_keyCodes(keyCodes)
{
}

void NewInput::UpdateKeyState(KeyCode key, bool isPressed)
{
    if (m_keyStates.find(key) == m_keyStates.end()) {
        m_keyStates[key] = 0; // Initialize with 0 if not found
    }

    char& state = m_keyStates[key];

    // Shift the previous state to the left and set the current state
    state = (state << 1) | (isPressed ? 1 : 0);
}

bool NewInput::GetKey(KeyCode key)
{
    auto it = m_keyStates.find(key);
    if (it != m_keyStates.end())
        return it->second & 0x1;

    return false;
}

bool NewInput::GetKeyDown(KeyCode key)
{
    auto it = m_keyStates.find(key);
    if (it != m_keyStates.end())
        return (it->second & 0x3) == 0x1;

    return false;
}

bool NewInput::GetKeyUp(KeyCode key)
{
    auto it = m_keyStates.find(key);
    if (it != m_keyStates.end())
        return (it->second & 0x3) == 0x2;

    return false;
}