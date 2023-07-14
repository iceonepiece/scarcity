#pragma once

#include "Input/KeyCodes.h"

class InputS
{
	virtual void Poll() = 0;
	virtual bool GetKey(KeyCode key) = 0;
	virtual bool GetKeyDown(KeyCode key) = 0;
	virtual bool GetKeyUp(KeyCode key) = 0;
};