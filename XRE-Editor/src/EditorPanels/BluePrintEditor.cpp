
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
			return IM_COL32(50, 250, 50, 255);
			break;
		case BluePrintNode::NodeColor::Red:
			return IM_COL32(250, 50, 50, 255);
			break;
		case BluePrintNode::NodeColor::Purple:
			return IM_COL32(150, 50, 200, 255);
			break;
		case BluePrintNode::NodeColor::Orange:
			return IM_COL32(150, 100, 50, 255);
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
		ImGui::Begin(u8"��ͼ�༭��");
		if (ImGui::Button(u8"ɾ����ѡ")) {
			
			for (auto i : m_selectedNodes) {
				if(i>=0)
				m_BluePrint->RemoveNode(m_BluePrint->GetNodeByID(i));
			}
		}
		
		const int hardcoded_node_id = 1;
		
		ImNodes::BeginNodeEditor();

		for (auto x : m_selectedNodes) {
			x = -1;
		}
		ImNodes::GetSelectedNodes(m_selectedNodes);
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

		for (auto node : m_BluePrint->m_Nodes) {
			DrawNode(node);
		}


		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			
			if (ImGui::BeginMenu(u8"����")) {
				
				if (ImGui::MenuItem(u8"����")) {
					m_BluePrint->MakeNode<Node_ConstInt>();
				}
				if (ImGui::MenuItem(u8"����")) {
					m_BluePrint->MakeNode<Node_ConstBool>();
				}
				if (ImGui::MenuItem(u8"������")) {
					m_BluePrint->MakeNode<Node_ConstFloat>();
				}
				if (ImGui::MenuItem(u8"�ַ���")) {
					m_BluePrint->MakeNode<Node_ConstString>();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"����")) {

				if (ImGui::MenuItem(u8"��ȡ����")) {
					m_BluePrint->MakeNode<Node_GetField>();
				}
				if (ImGui::MenuItem(u8"д�����")) {
					m_BluePrint->MakeNode<Node_SetField>();
				}
				
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"������")) {

				if (ImGui::MenuItem(u8"��֧")) {
					m_BluePrint->MakeNode<Node_Branch>();
				}
				
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"����")) {
				if (ImGui::BeginMenu(u8"����")) {
					if (ImGui::MenuItem(u8"+")) {
						m_BluePrint->MakeNode<Node_Plus>();
					}
					if (ImGui::MenuItem(u8"-")) {
						m_BluePrint->MakeNode<Node_Minus>();
					}
					if (ImGui::MenuItem(u8"*")) {
						m_BluePrint->MakeNode<Node_Multiply>();
					}
					if (ImGui::MenuItem(u8"/")) {
						m_BluePrint->MakeNode<Node_Division>();
					}
					if (ImGui::MenuItem(u8"Mod")) {
						m_BluePrint->MakeNode<Node_Mod>();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"�߼�")) {
					if (ImGui::MenuItem(u8"��")) {
						m_BluePrint->MakeNode<Node_And>();
					}
					if (ImGui::MenuItem(u8"��")) {
						m_BluePrint->MakeNode<Node_Or>();
					}
					if (ImGui::MenuItem(u8"��")) {
						m_BluePrint->MakeNode<Node_Not>();
					}
					if (ImGui::MenuItem(u8"����")) {
						m_BluePrint->MakeNode<Node_Equal>();
					}
					if (ImGui::MenuItem(u8"���ڵ���")) {
						m_BluePrint->MakeNode<Node_EqualOrCompare>();
					}
					if (ImGui::MenuItem(u8"����")) {
						m_BluePrint->MakeNode<Node_Compare>();
					}
					if (ImGui::MenuItem(u8"������")) {
						m_BluePrint->MakeNode<Node_UnEqual>();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"����")){
					if (ImGui::MenuItem(u8"sin")) {
						m_BluePrint->MakeNode<Node_Sin>();
					}
					if (ImGui::MenuItem(u8"cos")) {
						m_BluePrint->MakeNode<Node_Cos>();
					}
					if (ImGui::MenuItem(u8"tan")) {
						m_BluePrint->MakeNode<Node_Tan>();
					}
					if (ImGui::MenuItem(u8"�Ƕ�ת����")) {
						m_BluePrint->MakeNode<Node_DegreeToRad>();
					}
					if (ImGui::MenuItem(u8"����ת�Ƕ�")) {
						m_BluePrint->MakeNode<Node_RadToDegree>();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"����")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"�ַ���")) {
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(u8"����")) {
				if (ImGui::MenuItem(u8"������")) {
					m_BluePrint->MakeNode<Node_Self>();
				}
				if (ImGui::MenuItem(u8"����ID��ȡ")) {
				}
				if (ImGui::MenuItem(u8"�������ƻ�ȡ")) {
				}
				if (ImGui::MenuItem(u8"��������")) {
				}
				if (ImGui::MenuItem(u8"ɾ������")) {
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(u8"���")) {
				if (ImGui::BeginMenu(u8"�任")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"������Ⱦ")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"����")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"���Դ")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"ƽ�й�")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"�����")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"����")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu(u8"��ͼ")) {
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"����/���")) {


				if (ImGui::MenuItem(u8"�������")) {
					m_BluePrint->MakeNode<Node_Debug>();
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
				XUI::DragInt(v.m_Name, (int*)v.m_Handle);
				break;
			case FieldType::Field_Float:
				XUI::DragFloat(v.m_Name, (float*)v.m_Handle);
				break;
			case FieldType::Field_Field:
				break;

			default:
				break;
			}
		}
		ImGui::PopItemWidth();

		
		ImNodes::EndNode();

	}
	
}