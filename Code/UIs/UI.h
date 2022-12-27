#pragma once

#include <glm/glm.hpp>
#include "../Input/Input.h"

class UIComponent;
class Scene;

enum UIAlignment
{
	NONE,
	CENTER,
	TOP_LEFT,
	TOP_RIGHT,
	TOP_MIDDLE,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_MIDDLE
};

class UI
{
public:
	UI(Scene* scene);
	~UI();

	virtual void Init();
	virtual void ProcessInput(Input& input);
	virtual void Update(float deltaTime);
	virtual void Render();

	void AddComponent(UIComponent* component);
	glm::vec2 GetScreenSizePercentage(int width, int height);

private:
	int defaultScreenWidth;
	int defaultScreenHeight;
	Scene* m_scene;
	std::vector<UIComponent*> m_components;
};