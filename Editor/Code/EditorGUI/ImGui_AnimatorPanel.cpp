#include "ImGui_AnimatorPanel.h"
#include <iostream>
#include "../EditorLayer.h"

ImGui_AnimatorPanel::ImGui_AnimatorPanel(EditorLayer& editor)
    : m_editor(editor)
{
    m_context = ImNodes::Ez::CreateContext();
}

ImGui_AnimatorPanel::~ImGui_AnimatorPanel()
{
    for (auto node : nodes)
        delete node;

    ImNodes::Ez::FreeContext(m_context);
}

void ImGui_AnimatorPanel::Render()
{
    unsigned int nodeCount = 0;
    Node* selectedNode = nullptr;

    if (ImGui::Begin("Animator", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        // We probably need to keep some state, like positions of nodes/slots for rendering connections.
        ImNodes::Ez::BeginCanvas();


        for (auto it = nodes.begin(); it != nodes.end();)
        {
            Node* node = *it;

            // Start rendering node
            if (ImNodes::Ez::BeginNode(node, node->Title, &node->Pos, &node->Selected))
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

        ImNodes::Ez::EndCanvas();
    }
    ImGui::End();

    if (nodeCount == 1 && selectedNode != nullptr)
    {
        std::cout << "Selected: " << selectedNode->Title << '\n';
        //m_editor.SetSelectedAsset()
    }
    else if (nodeCount > 1)
    {
        std::cout << nodeCount << " Animator States\n";
    }
}