#pragma once

class EditorLayer;
class EntityManager;

class ImGuiHierarchy
{
public:
	ImGuiHierarchy(EditorLayer& editor);
	void Render();
	void RenderEntity(EntityManager& manager, entt::entity entity);

private:
	EditorLayer& m_editor;
};