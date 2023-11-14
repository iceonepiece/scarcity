#pragma once

//#include <ImGuiNodeEditor/imgui_node_editor.h>
//#include <imnodes/imnodes.h>
#include <vector>
#include <ImGuiNodes/ImNodesEz.h>
#include <map>
#include <string>

//namespace ed = ax::NodeEditor;

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
struct MyNode
{
	/// Title which will be displayed at the center-top of the node.
	const char* Title = nullptr;
	/// Flag indicating that node is selected by the user.
	bool Selected = false;
	/// Node position on the canvas.
	ImVec2 Pos{};
	/// List of node connections.
	std::vector<Connection> Connections{};
	/// A list of input slots current node has.
	std::vector<ImNodes::Ez::SlotInfo> InputSlots{};
	/// A list of output slots current node has.
	std::vector<ImNodes::Ez::SlotInfo> OutputSlots{};

	explicit MyNode(const char* title,
		const std::vector<ImNodes::Ez::SlotInfo>&& input_slots,
		const std::vector<ImNodes::Ez::SlotInfo>&& output_slots)
	{
		Title = title;
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

struct Node
{
	int   id;
	float value;

	Node() = default;

	Node(const int i, const float v) : id(i), value(v) {}
};

struct Link
{
	int id;
	int start_attr, end_attr;
};

class ImGuiNodeEditor
{
public:
	/*
	struct LinkInfo
	{
		ed::LinkId Id;
		ed::PinId  InputId;
		ed::PinId  OutputId;
	};
	*/

	ImGuiNodeEditor();
	~ImGuiNodeEditor();

	void Render();

private:
	ImNodes::Ez::Context* m_context;

	
	std::map<std::string, MyNode* (*)()> available_nodes{
		{
			"Compose",
			[]()
			{
				return new MyNode(
					"Compose",
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


	std::vector<MyNode*> nodes;
	/*
	std::vector<Node> nodes_;
	std::vector<Link> links_;
	int               current_id_;

	bool m_firstFrame = true;
	ed::EditorContext* m_context = nullptr;
	ImVector<LinkInfo>   m_links;
	int                  m_nextLinkId = 100;
	*/
};