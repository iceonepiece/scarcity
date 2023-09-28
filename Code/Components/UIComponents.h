#pragma once

struct CanvasComponent
{
	static std::string Name() { return "Canvas"; }

	float width;
	float height;
};

static void DoSerialize(const CanvasComponent& canvas, json& entityJson)
{

}

static void DoDeserialize(CanvasComponent& canvas, json& cameraJson)
{

}

struct TextComponent
{

};

struct ImageComponent
{

};

struct ButtonComponent
{

};