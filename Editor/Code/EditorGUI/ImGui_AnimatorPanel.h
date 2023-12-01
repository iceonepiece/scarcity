#pragma once

#include <vector>
#include <ImGuiNodes/ImNodes.h>
#include <ImGuiNodes/ImNodesEz.h>
#include <map>
#include <string>
#include "Animations/AnimatorController.h"

class EditorLayer;

struct Connection
{
	/// `id` that was passed to BeginNode() of input node.
	void* InputNode = nullptr;
	/// Descriptor of input slot.
	const char* InputSlot = nullptr;
	/// `id` that was passed to BeginNode() of output node.
	void* OutputNode = nullptr;
	/// Descriptor of output slot.
	const char* OutputSlot = nullptr;

	bool operator==(const Connection& other) const
	{
		return InputNode == other.InputNode &&
			InputSlot == other.InputSlot &&
			OutputNode == other.OutputNode &&
			OutputSlot == other.OutputSlot;
	}

	bool operator!=(const Connection& other) const
	{
		return !operator ==(other);
	}
};

enum NodeSlotTypes
{
	NodeSlotPosition = 1,   // ID can not be 0
	NodeSlotRotation,
	NodeSlotMatrix,
};

/// A structure holding node state.
struct Node
{
	std::string title = "New State";

	float speed = 1.0f;

	/// Flag indicating that node is selected by the user.
	bool Selected = false;
	/// Node position on the canvas.
	ImVec2 pos{};
	/// List of node connections.
	std::vector<Connection> Connections{};
	/// A list of input slots current node has.
	std::vector<ImNodes::Ez::SlotInfo> InputSlots{};
	/// A list of output slots current node has.
	std::vector<ImNodes::Ez::SlotInfo> OutputSlots{};

	explicit Node(const char* title,
		const std::vector<ImNodes::Ez::SlotInfo>&& input_slots,
		const std::vector<ImNodes::Ez::SlotInfo>&& output_slots)
	{
		title = title;
		InputSlots = input_slots;
		OutputSlots = output_slots;
	}

	/// Deletes connection from this node.
	void DeleteConnection(const Connection& connection)
	{
		for (auto it = Connections.begin(); it != Connections.end(); ++it)
		{
			if (connection == *it)
			{
				Connections.erase(it);
				break;
			}
		}
	}
};
class ImGui_AnimatorPanel
{
public:

	ImGui_AnimatorPanel(EditorLayer& editor);
	~ImGui_AnimatorPanel();

	void Render();

	void RenderAnimatorState(AnimatorState& state, bool defaultState = false);

	void ClearSelection();
	void SetAnimatorController(AnimatorController* controller);

private:
	inline static AnimatorController* s_animController = nullptr;

	ImNodes::Ez::Context* m_context;
	bool m_toClear = false;
	
	std::map<std::string, Node* (*)()> available_nodes{
		{
			"Animator State",
			[]()
			{
				return new Node(
					"Untitled",
					{ {"In", 1} },
					{ {"Out", 1} }
				);
			}
		}

		/*
		{
			"Compose",
			[]() -> MyNode* {
				return new MyNode("Compose", {
					{"Position", NodeSlotPosition}, {"Rotation", NodeSlotRotation}  // Input slots
		}, {
			{"Matrix", NodeSlotMatrix}                                      // Output slots
		}); }},


		{"Decompose", []() -> MyNode* { return new MyNode("Decompose", {
			{"Matrix", NodeSlotMatrix}                                      // Input slots
		}, {
			{"Position", NodeSlotPosition}, {"Rotation", NodeSlotRotation}  // Output slots
		}); }},
		*/
	};

	AnimatorController* m_animController;
	std::vector<Node*> nodes;
	EditorLayer& m_editor;

	size_t m_stateCount = 0;
	AnimatorState* m_selectedState = nullptr;
};