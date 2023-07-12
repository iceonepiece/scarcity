#pragma once

#include <memory>
#include "Input/KeyCodes.h"
#include "Input/Input.h"

class InputAPI
{
public:
	static bool GetKey(KeyCode key)
	{
		return s_input->GetKey(key);
	}

	static bool GetKeyDown(KeyCode key)
	{
		return s_input->GetKeyDown(key);
	}

	static bool GetKeyUp(KeyCode key)
	{
		return s_input->GetKeyUp(key);
	}

private:
	static std::unique_ptr<Input> s_input;
};