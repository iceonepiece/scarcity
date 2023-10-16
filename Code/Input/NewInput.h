#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include <map>
#include <vector>
#include <iostream>

class NewInput
{
public:
    NewInput(const std::vector<KeyCode>& keyCodes, const std::vector<MouseCode>& mouseCodes);
    virtual void Poll() = 0;

    void UpdateKeyState(KeyCode key, bool isPressed);
    void UpdateMouseState(MouseCode code, bool isPressed);

    bool GetKey(KeyCode key);
    bool GetKeyDown(KeyCode key);
    bool GetKeyUp(KeyCode key);

    bool GetMouseButton(MouseCode code);
    bool GetMouseButtonDown(MouseCode code);
    bool GetMouseButtonUp(MouseCode code);

protected:
    std::vector<KeyCode> m_keyCodes;
    std::map<KeyCode, char> m_keyStates;

    std::vector<MouseCode> m_mouseCodes;
    std::map<MouseCode, char> m_mouseStates;
};