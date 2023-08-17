#pragma once

#include "KeyCodes.h"
#include <map>
#include <vector>
#include <iostream>

class NewInput
{
public:
    NewInput(const std::vector<KeyCode>& keyCodes);
    virtual void Poll() = 0;

    void UpdateKeyState(KeyCode key, bool isPressed);

    bool GetKey(KeyCode key);
    bool GetKeyDown(KeyCode key);
    bool GetKeyUp(KeyCode key);

protected:
    std::vector<KeyCode> m_keyCodes;
    std::map<KeyCode, char> m_keyStates;
};