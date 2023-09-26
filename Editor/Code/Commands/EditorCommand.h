#pragma once

class EditorCommand
{
public:
	virtual ~EditorCommand() = default;
	virtual void Do() {}
	virtual void Undo() {}
};