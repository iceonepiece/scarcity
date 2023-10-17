#include "NewInput.h"

NewInput::NewInput(const std::vector<KeyCode>& keyCodes, const std::vector<MouseCode>& mouseCodes)
    : m_keyCodes(keyCodes)
    , m_mouseCodes(mouseCodes)
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

void NewInput::UpdateMouseState(MouseCode code, bool isPressed)
{
    if (m_mouseStates.find(code) == m_mouseStates.end()) {
        m_mouseStates[code] = 0; // Initialize with 0 if not found
    }

    char& state = m_mouseStates[code];

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

bool NewInput::GetMouseButton(MouseCode code)
{
    auto it = m_mouseStates.find(code);
    if (it != m_mouseStates.end())
        return it->second & 0x1;

    return false;
}

bool NewInput::GetMouseButtonDown(MouseCode code)
{
    auto it = m_mouseStates.find(code);
    if (it != m_mouseStates.end())
        return (it->second & 0x3) == 0x1;

    return false;
}

bool NewInput::GetMouseButtonUp(MouseCode code)
{
    auto it = m_mouseStates.find(code);
    if (it != m_mouseStates.end())
        return (it->second & 0x3) == 0x2;

    return false;
}

void NewInput::SetCursorPosition(float x, float y)
{
    m_cursorPosition.x = x;
    m_cursorPosition.y = y;
}

glm::vec2 NewInput::GetCursorPosition() const
{
    return m_cursorPosition;
}
