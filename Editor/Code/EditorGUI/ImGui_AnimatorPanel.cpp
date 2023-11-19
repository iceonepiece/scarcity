#include "ImGui_AnimatorPanel.h"
#include <iostream>
#include <IconsFontAwesome6.h>
#include "../EditorLayer.h"

ImGui_AnimatorPanel::ImGui_AnimatorPanel(EditorLayer& editor)
    : m_editor(editor)
    , m_animController(s_animController)
{
    m_context = ImNodes::Ez::CreateContext();
}

ImGui_AnimatorPanel::~ImGui_AnimatorPanel()
{
    for (auto node : nodes)
        delete node;

    ImNodes::Ez::FreeContext(m_context);
}

void ImGui_AnimatorPanel::SetAnimatorController(AnimatorController& animController)
{
    m_animController = animController;
}

void ImGui_AnimatorPanel::ClearSelection()
{
    m_toClear = true;
}

struct NameEditing
{
    std::string oldName;
    std::string newName;
    bool changed = false;
};

void ImGui_AnimatorPanel::Render()
{
    static std::string currentNameEditing = "";
    unsigned int nodeCount = 0;
    Node* selectedNode = nullptr;
    static int selected_fish = -1;

    bool openPopup = false;
    static int editingIndex = -1;

    if (ImGui::Begin("Animator", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImGui::BeginChild("Parameters", ImVec2(220, 0), false, 0);

        auto& parameters = m_animController.GetParameters();

        NameEditing nameEditing;

        const char* names[] = { "Float", "Int", "Bool", "Trigger" };
        static bool toggles[] = { true, false, false, false };

        if (ImGui::Button(ICON_FA_PLUS ICON_FA_CARET_DOWN))
            ImGui::OpenPopup("my_select_popup");

        ImGui::SameLine();
        ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);


        if (ImGui::BeginPopup("my_select_popup"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
            {
                if (ImGui::Selectable(names[i]))
                {
                    selected_fish = i;
                    openPopup = true;

                    if (selected_fish == 0)
                        m_animController.AddParameter("", 0.0f);
                    else if (selected_fish == 1)
                        m_animController.AddParameter("", 0);
                    else if (selected_fish == 2)
                        m_animController.AddParameter("", false);
                    else if (selected_fish == 3)
                        m_animController.AddParameter("", Trigger{});

                    editingIndex = m_animController.GetParameters().size() - 1;
                }
            }

            ImGui::EndPopup();
        }

        if (ImGui::BeginTable("table1", 2))
        {
            for (auto& param : parameters)
            {
                ParameterType value = param.value;

                ImGui::TableNextRow();

                ImGui::PushID(param.name.c_str());

                ImGui::TableSetColumnIndex(0);

                //std::string text = ICON_FA_GEAR + " " + param.name;

                if (ImGui::Button(ICON_FA_GEAR))
                {
                    openPopup = true;
                }

                ImGui::SameLine();
                ImGui::Text(param.name.c_str());

                ImGui::TableSetColumnIndex(1);

                if (auto ptr = std::get_if<float>(&param.value)) {
                    ImGui::InputFloat("###value", ptr);

                }
                else if (auto ptr = std::get_if<int>(&param.value)) {
                    ImGui::InputInt("###value", ptr, 0);
                }
                else if (auto ptr = std::get_if<bool>(&param.value)) {
                    ImGui::Checkbox("###value", ptr);
                }
                else if (auto ptr = std::get_if<Trigger>(&param.value)) {
                    ImGui::RadioButton("###value", false);
                }

                ImGui::PopID();
            }

            ImGui::EndTable();
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Viewport");

        ImNodes::Ez::BeginCanvas();

        for (auto it = nodes.begin(); it != nodes.end();)
        {
            Node* node = *it;

            // Start rendering node
            if (ImNodes::Ez::BeginNode(node, node->title.c_str(), &node->Pos, &node->Selected))
            {
                // Render input nodes first (order is important)
                ImNodes::Ez::InputSlots(node->InputSlots.data(), node->InputSlots.size());

                // Custom node content may go here
                //ImGui::Text("Content of %s", node->Title);

                // Render output nodes first (order is important)
                ImNodes::Ez::OutputSlots(node->OutputSlots.data(), node->OutputSlots.size());

                // Store new connections when they are created
                Connection new_connection;
                if (ImNodes::GetNewConnection(&new_connection.InputNode, &new_connection.InputSlot,
                    &new_connection.OutputNode, &new_connection.OutputSlot))
                {
                    ((Node*)new_connection.InputNode)->Connections.push_back(new_connection);
                    ((Node*)new_connection.OutputNode)->Connections.push_back(new_connection);
                }

                // Render output connections of this node
                for (const Connection& connection : node->Connections)
                {
                    // Node contains all it's connections (both from output and to input slots). This means that multiple
                    // nodes will have same connection. We render only output connections and ensure that each connection
                    // will be rendered once.
                    if (connection.OutputNode != node)
                        continue;

                    if (!ImNodes::Connection(connection.InputNode, connection.InputSlot, connection.OutputNode,
                        connection.OutputSlot))
                    {
                        // Remove deleted connections
                        ((Node*)connection.InputNode)->DeleteConnection(connection);
                        ((Node*)connection.OutputNode)->DeleteConnection(connection);
                    }
                }
            }

            // Node rendering is done. This call will render node background based on size of content inside node.
            ImNodes::Ez::EndNode();

            if (node->Selected)
            {
                nodeCount++;
                selectedNode = node;
            }

            if (node->Selected && ImGui::IsKeyPressed(ImGuiKey_Delete) && ImGui::IsWindowFocused())
            {
                // Deletion order is critical: first we delete connections to us
                for (auto& connection : node->Connections)
                {
                    if (connection.OutputNode == node)
                    {
                        ((Node*)connection.InputNode)->DeleteConnection(connection);
                    }
                    else
                    {
                        ((Node*)connection.OutputNode)->DeleteConnection(connection);
                    }
                }
                // Then we delete our own connections, so we don't corrupt the list
                node->Connections.clear();

                delete node;
                it = nodes.erase(it);
            }
            else
                ++it;
        }

        if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
        {
            ImGui::SetWindowFocus();
            ImGui::OpenPopup("NodesContextMenu");
        }

        if (ImGui::BeginPopup("NodesContextMenu"))
        {
            for (const auto& desc : available_nodes)
            {
                if (ImGui::MenuItem(desc.first.c_str()))
                {
                    nodes.push_back(desc.second());
                    ImNodes::AutoPositionNode(nodes.back());
                }
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Reset Zoom"))
                ImNodes::GetCurrentCanvas()->Zoom = 1;

            if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered())
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        if (m_toClear)
        {
            ImNodes::ClearSelection();
            m_toClear = false;
            nodeCount = 0;
        }

        ImNodes::Ez::EndCanvas();

        ImGui::EndChild();
    }


    static std::string newName;
    static std::string prevName;

    if (openPopup)
    {

        if (editingIndex != -1)
        {
            prevName = m_animController.GetParameters()[editingIndex].name;
            newName = prevName;
        }

        ImGui::OpenPopup("Parameter Editor");
    }

    if (ImGui::BeginPopupModal("Parameter Editor"))
    {
        ImGui::InputText("Name", &newName);

        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            if (editingIndex != -1)
                m_animController.ChangeParameterName(prevName, newName);
           // else
              //  m_animController.AddParameter(newName, 0);

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    ImGui::End();

    if (nodeCount == 0 && m_editor.GetSelectedObject().type == EditorObjectType::AnimatorState)
    {
        m_editor.UnselectObject();
    }

    if (nodeCount == 1 && selectedNode != nullptr)
    {
        if (m_editor.GetSelectedObject().objectPtr != selectedNode)
        {
            m_editor.SetSelectedObject(EditorObjectType::AnimatorState, selectedNode);
        }
    }
}