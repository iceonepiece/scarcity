#pragma once

#include "../Events/Event.h"

class Layer
{
public:
	virtual void OnEvent(Event& event) = 0;
};