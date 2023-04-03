
#include "BluePrintEditor.h"
#include "PanelsManager.h"
#include <ImGui\imgui.h>
#include "Data.h"
#include <imnodes\imnodes.h>
namespace XRE {
	
	int GetFieldColor(FieldType ft)
	{
		switch (ft)
		{
		case XRE::Field_Blank:
			return IM_COL32(50, 50, 50, 255);
			break;
		case XRE::Field_Int:
			return IM_COL32(50, 50, 250, 255);
			break;
		case XRE::Field_Float:
			return IM_COL32(50, 250, 50, 255);
			break;
		case XRE::Field_Bool:
			return IM_COL32(250, 50, 50, 255);
			break;
		case XRE::Field_String:
			return IM_COL32(150, 50, 200, 255);
			break;
		default:
			break;
		}
		return 0;
	}

	int GetNodeColor(BluePrintNode::NodeColor nodecol)
	{
		switch (nodecol)
		{
		case BluePrintNode::NodeColor::Grey:
			return IM_COL32(50, 50, 50, 255);
			break;
		case BluePrintNode::NodeColor::Blue:
			return IM_COL32(50, 50, 250, 255);
			break;
		case BluePrintNode::NodeColor::Green:
			return IM_COL32(50, 150, 0, 255);
			break;
		case BluePrintNode::NodeColor::Red:
			return IM_COL32(200, 60, 0, 255);
			break;
		case BluePrintNode::NodeColor::Pink:
			return IM_COL32(250, 50, 150, 255);
			break;
		case BluePrintNode::NodeColor::Purple:
			return IM_COL32(100, 30, 200, 255);
			break;
		case BluePrintNode::NodeColor::Orange:
			return IM_COL32(200, 100, 0, 255);
			break;
		case BluePrintNode::NodeColor::Yellow:
			return IM_COL32(180, 100, 0, 255);
			break;
		default:
			break;
		}
		return 0;
	}
	BluePrintEditor::BluePrintEditor()
	{
	}

	void BluePrintEditor::OnImGuiRender()
	{
		static glm::vec4 color;
		ImGui::Begin(u8"蓝图编辑器");

		if (m_switch == true) {
			ImGui::SetWindowFocus();
			m_switch = false;
		}
		std::string s = u8"当前:" + m_Info + "," + m_BluePrint->GetFileName();
		ImGui::Text(s.c_str());
		
		if (ImGui::Button(u8"删除所选")) {
			
			for (auto i : m_selectedNodes) {
				auto node = m_BluePrint->GetNodeByID(i);
				for (auto in : node->m_Inputs) {
					if (in->m_Connection != nullptr) {

					}
				}
					m_BluePrint->UnLink(m_BluePrint->m_Links[i].first, m_BluePrint->m_Links[i].second);
				if(i>=0)
				m_BluePrint->RemoveNode(node);
				
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"保存")) {
			
			SaveNodesLayout();
			m_BluePrint->Save();
			

		}
		ImGui::SameLine();
		if (ImGui::Button(u8"保存为")) {
			auto path = FileDialogs::SaveFile("BluePrint (*.bp)\0*.bp\0");
			if (path != "") {
				m_BluePrint->Save(path);
			}
			
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"打开")) {
			auto path = FileDialogs::OpenFile("BluePrint (*.bp)\0*.bp\0");
			if (path != "") {
				m_BluePrint->LoadFromFile(path);
			}
			
			
		}
		
		const int hardcoded_node_id = 1;
		ImGui::BeginChild(u8"Variants",ImVec2(200,400));
		for (auto vv : m_BluePrint->m_Fields) {
			ImGui::PushItemWidth(80);
			auto v = vv.second;
			//Imgui need different name to identify
			string s="##Text" + to_string(v->id);
			string f = "##Value" + to_string(v->id);
			XUI::InputText(s.c_str(),& v->m_FieldName);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			switch (v->m_Type)
			{
			case FieldType::Field_Bool:

				XUI::CheckBox(f.c_str(), (bool*) m_Properties->GetFieldValuePointer(v));
				break;
			case FieldType::Field_Int:
				XUI::DragInt(f.c_str(), (int*)m_Properties->GetFieldValuePointer(v));
				break;
			case FieldType::Field_Float:
				XUI::DragFloat(f.c_str(), (float*)m_Properties->GetFieldValuePointer(v));
				break;
			case FieldType::Field_String:
				XUI::InputText(f.c_str(), (string*)m_Properties->GetFieldValuePointer(v));
				break;
			default:
				break;
			}
			
		}

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if(ImGui::MenuItem("Int") ){
				m_BluePrint->MakeField(FieldType::Field_Int, "val_i");
			}
			if (ImGui::MenuItem("Bool")) {
				m_BluePrint->MakeField(FieldType::Field_Bool, "val_b");
			}
			if (ImGui::MenuItem("Float")) {
				m_BluePrint->MakeField(FieldType::Field_Float, "val_f");
			}
			if (ImGui::MenuItem("String")) {
				m_BluePrint->MakeField(FieldType::Field_String, "val_s");
			}
			ImGui::EndPopup();
		}

		ImGui::EndChild();
		ImGui::SameLine();

		ImNodes::BeginNodeEditor();
		

		for (auto x : m_selectedNodes) {
			x = -1;
		}
		ImNodes::GetSelectedNodes(m_selectedNodes);
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
		ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(200, 200, 200, 255));

		for (auto node : m_BluePrint->m_Nodes) {
			DrawNode(node);
		}
		
		
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			
			
			if (ImGui::BeginMenu(u8"基础")) {
				
				if (ImGui::MenuItem(u8"整数")) {
					AddNode<Node_ConstInt>();
				}
				if (ImGui::MenuItem(u8"布尔")) {
					AddNode<Node_ConstBool>();
				}
				if (ImGui::MenuItem(u8"浮点数")) {
					AddNode<Node_ConstFloat>();
				}
				if (ImGui::MenuItem(u8"字符串")) {
					AddNode<Node_ConstString>();
				}
				if (ImGui::MenuItem(u8"读取变量")) {
					AddNode<Node_GetField>();
				}
				if (ImGui::MenuItem(u8"写入变量")) {
					AddNode<Node_SetField>();
				}
				if (ImGui::MenuItem(u8"时间差")) {
					AddNode<Node_DeltaTime>();
				}
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu(u8"控制流")) {

				if (ImGui::MenuItem(u8"分支")) {
					AddNode<Node_Branch>();
				}
				if (ImGui::MenuItem(u8"开始时")) {
					AddNode<Node_OnSceneBegin>();
				}
				if (ImGui::MenuItem(u8"碰撞")) {
					AddNode<Node_OnCollision>();
				}
				
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"运算")) {
				if (ImGui::MenuItem(u8"+")) {
					AddNode<Node_Plus>();
				}
				if (ImGui::MenuItem(u8"-")) {
					AddNode<Node_Minus>();
				}
				if (ImGui::MenuItem(u8"*")) {
					AddNode<Node_Multiply>();
				}
				if (ImGui::MenuItem(u8"/")) {
					AddNode<Node_Division>();
				}
				if (ImGui::BeginMenu(u8"基础")) {
					
					if (ImGui::MenuItem(u8"Mod")) {
						AddNode<Node_Mod>();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"逻辑")) {
					if (ImGui::MenuItem(u8"与")) {
						AddNode<Node_And>();
					}
					if (ImGui::MenuItem(u8"或")) {
						AddNode<Node_Or>();
					}
					if (ImGui::MenuItem(u8"非")) {
						AddNode<Node_Not>();
					}
					if (ImGui::MenuItem(u8"等于")) {
						AddNode<Node_Equal>();
					}
					if (ImGui::MenuItem(u8"大于等于")) {
						AddNode<Node_EqualOrCompare>();
					}
					if (ImGui::MenuItem(u8"大于")) {
						AddNode<Node_Compare>();
					}
					if (ImGui::MenuItem(u8"不等于")) {
						AddNode<Node_UnEqual>();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"三角")){
					if (ImGui::MenuItem(u8"sin")) {
						AddNode<Node_Sin>();
					}
					if (ImGui::MenuItem(u8"cos")) {
						AddNode<Node_Cos>();
					}
					if (ImGui::MenuItem(u8"tan")) {
						AddNode<Node_Tan>();
					}
					if (ImGui::MenuItem(u8"角度转弧度")) {
						AddNode<Node_DegreeToRad>();
					}
					if (ImGui::MenuItem(u8"弧度转角度")) {
						AddNode<Node_RadToDegree>();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"向量")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"字符串")) {
					if (ImGui::MenuItem(u8"拼接")) {
						AddNode<Node_StrCat>();
					}
					if (ImGui::MenuItem(u8"比较")) {
						AddNode<Node_StrCmp>();
					}
					if (ImGui::MenuItem(u8"查找")) {
						AddNode<Node_StrFind>();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(u8"对象")) {
				if (ImGui::MenuItem(u8"本对象")) {
					AddNode<Node_Self>();
				}
				
				if (ImGui::MenuItem(u8"根据名称获取")) {
					AddNode<Node_GetByName>();
				}
				if (ImGui::MenuItem(u8"创建对象")) {
					AddNode<Node_Create>();
				}
				if (ImGui::MenuItem(u8"删除对象")) {
					AddNode<Node_Destroy>();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(u8"组件")) {
				if (ImGui::BeginMenu(u8"变换")) {
					if (ImGui::MenuItem(u8"获取位置")) {
						AddNode<Node_GetPosition>();
					}
					if (ImGui::MenuItem(u8"设置位置")) {
						AddNode<Node_SetPosition>();
					}
					if (ImGui::MenuItem(u8"获取旋转")) {
						AddNode<Node_GetRotation>();
					}
					if (ImGui::MenuItem(u8"设置旋转")) {
						AddNode<Node_SetRotation>();
					}
					if (ImGui::MenuItem(u8"获取缩放")) {
						AddNode<Node_GetScale>();
					}
					if (ImGui::MenuItem(u8"设置缩放")) {
						AddNode<Node_SetScale>();
					}

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"几何渲染")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"刚体与物理")) {
					if (ImGui::MenuItem(u8"设置线速度")) {
						AddNode<Node_SetLinearVelocity>();
					}

					if (ImGui::MenuItem(u8"设置角速度")) {
						AddNode<Node_SetAngularVelocity>();
					}

					if (ImGui::MenuItem(u8"设置状态")) {
						
						//AddNode<Node_SetAngularVelocity>();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"光源")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"摄像机")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"蓝图")) {
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"输入/输出")) {


				if (ImGui::MenuItem(u8"调试输出")) {
					AddNode<Node_Debug>();
				}

				if (ImGui::MenuItem(u8"键盘输入")) {
					AddNode<Node_OnKeyEvent>();
				}


				ImGui::EndMenu();
			}
			

			ImGui::EndPopup();
		}


		

		// elsewhere in the code...
		for (int i = 0; i < m_BluePrint->m_Links.size(); ++i)
		{
			const std::pair<int, int> p = m_BluePrint->m_Links[i];
			// in this case, we just use the array index of the link
			// as the unique identifier
			
			ImNodes::Link(i, p.first, p.second);


		}

		//SaveNodesLayout();
		ImNodes::PopColorStyle();

		ImNodes::EndNodeEditor();
		int start_attr, end_attr;
		if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
		{
			m_BluePrint->Link(start_attr, end_attr);
		}
		int i;
		if (ImNodes::IsLinkDestroyed(&i)) {

			m_BluePrint->UnLink(m_BluePrint->m_Links[i].first, m_BluePrint->m_Links[i].second);
		
		}


		ImGui::End();

	}
	void BluePrintEditor::DrawNode(XRef<BluePrintNode> n)
	{




		ImNodes::PushColorStyle(ImNodesCol_TitleBar, GetNodeColor(n->m_Color));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, GetNodeColor(n->m_Color));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, GetNodeColor(n->m_Color));
		
		
		ImNodes::BeginNode(n->NodeID); 

		

		
		ImNodes::BeginNodeTitleBar();
		ImGui::TextUnformatted( n->m_Title.c_str());

		if (n->m_FlowPrev) {
			ImNodes::BeginInputAttribute(n->m_FlowPrev->PinID);
			ImNodes::EndInputAttribute();
		}

		if (n->m_FlowProc) {
			ImNodes::BeginOutputAttribute(n->m_FlowProc->PinID);
			ImNodes::EndOutputAttribute();
		}
		

		ImNodes::EndNodeTitleBar();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();

		for (auto i : n->m_Inputs) {
			ImNodes::PushColorStyle(ImNodesCol_Pin, GetFieldColor(i->m_FieldType));
			ImNodes::BeginInputAttribute(i->PinID);
			ImGui::Text(i->m_Name.c_str());
			ImNodes::EndInputAttribute();
			ImNodes::PopColorStyle();
		}

		for (auto o : n->m_Outputs) {
			ImNodes::PushColorStyle(ImNodesCol_Pin,GetFieldColor(o->m_FieldType));
			ImNodes::BeginOutputAttribute(o->PinID);
			ImGui::Text(o->m_Name.c_str());
			ImNodes::EndOutputAttribute();
			ImNodes::PopColorStyle();
		}

		ImGui::PushItemWidth(150);
		
		for (auto v : n->m_Variants) {
			switch (v.m_Type)
			{
			case FieldType::Field_String:
				XUI::InputText(v.m_Name, (std::string*)v.m_Handle);
				break;
			case FieldType::Field_Bool:
				XUI::CheckBox(v.m_Name, (bool*)v.m_Handle);
				break;
			case FieldType::Field_Int:
				if (v.m_Name == u8"##Key") {
					XUI::SelectKey("##key", (int*)v.m_Handle);
				}
				else
				XUI::DragInt(v.m_Name, (int*)v.m_Handle);
				break;
			case FieldType::Field_Float:
				XUI::DragFloat(v.m_Name, (float*)v.m_Handle);
				break;
			case FieldType::Field_Field: {
				vector<std::string> fields;
				
				fields.push_back("##");
				std::string current = n->GetField() == nullptr ? "##" : n->GetField()->m_FieldName;
				for (auto x : m_BluePrint->m_Fields) {
					fields.push_back(x.second->m_FieldName);
				}

				if (ImGui::BeginCombo(u8"变量", current.c_str())) {
					for (int i = 0; i < fields.size(); i++)
					{
						bool isSelected = current == fields[i];
						if (ImGui::Selectable(fields[i].c_str(), isSelected))
						{
							current = fields[i];
							if (i > 0) {
								n->SetField(m_BluePrint->m_Fields[i - 1]);
							}
							else v.m_Handle = nullptr;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}
				
				


				break;

			default:
				break;
			}
		}
		ImGui::PopItemWidth();

		if (ImNodes::IsNodeSelected(n->NodeID) && Input::IsMouseButtonPressed(XRE_MOUSE_BUTTON_LEFT)) {

		}
		else
		ImNodes::SetNodeGridSpacePos(n->NodeID, ImVec2(n->editorPosX, n->editorPosY));
		ImNodes::EndNode();
		auto pos = ImNodes::GetNodeGridSpacePos(n->NodeID);

		n->editorPosX = pos.x;
		n->editorPosY = pos.y;

	}

	void BluePrintEditor::Switch()
	{
		m_switch = true;
	}

	void BluePrintEditor::SaveNodesLayout()
	{
		for (auto n : m_BluePrint->m_Nodes) {
			auto pos = ImNodes::GetNodeEditorSpacePos(n->NodeID);

			n->editorPosX = pos.x;
			n->editorPosY = pos.y;
		}
	}
	
}