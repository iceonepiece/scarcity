#pragma once

struct Value
{
	virtual ~Value() = default;
};


struct IntValue : public Value
{
	IntValue(int v)
		: value(v)
	{}

	virtual ~IntValue()
	{

	}

	int value;
};

struct BoolValue : public Value
{
	BoolValue(bool v)
		: value(v)
	{}

	virtual ~BoolValue()
	{

	}

	bool value;
};

struct FloatValue : public Value
{

};

struct TriggerValue : public Value
{

};

