#pragma once
#include "xre\Core\Input.h"
#include "xre\Audio\alManager.h"

namespace XRE {
	class Node_OnKeyEvent:public BluePrintNode {
	public:
		Node_OnKeyEvent() {
			m_NodeTypeID = 81;
			m_Title = u8"键盘输入";
			m_Color = Green;
		}

		void Initialize() override {
			AddOutput(FieldType::Field_Bool);
			AddVar(Field_Int, &keycode);
			m_Variants[0].m_Name = u8"##Key";
		}

		void Process() override {
			
			m_Outputs[0]->ValueBool = Input::IsKeyPressed(keycode);
			
		}
		
		int keycode = -1;
		
	};

	class Node_OnMouseEvent: public BluePrintNode {
	public:
		Node_OnMouseEvent() {
			m_NodeTypeID = 82;
			m_Title = u8"鼠标输入";
			m_Color = Yellow;
		}

		void Initialize() override {
			AddOutput(FieldType::Field_Bool);
			m_Outputs[0]->m_Name = u8"按下";
			AddOutput(FieldType::Field_Float);
			AddOutput(FieldType::Field_Float);
			m_Outputs[1]->m_Name = u8"x";
			m_Outputs[2]->m_Name = u8"y";
			AddVar(Field_Int, &mousecode);
			m_Variants[0].m_Name = u8"##Mouse";
		}

		void Process() override {
			m_Outputs[0]->ValueBool = Input::IsMouseButtonPressed(mousecode);
		}
		int mousecode;
	};


	class Node_PlayAudio : public BluePrintNode {
		bool m_loop;
	public:
		Node_PlayAudio() {
			m_NodeTypeID = 85;
			m_Title = u8"播放声音";
			m_Color = Blue;
		}

		void Initialize() override {
			AddInput(FieldType::Field_Int);
			m_Inputs[0]->m_Name = u8"对象";

			AddInput(FieldType::Field_String);
			m_Inputs[1]->m_Name = u8"音频源";

			AddVar(FieldType::Field_Bool,&m_loop);
			m_Variants[0].m_Name = u8"循环播放";
			
			CreateControlFlow();
		}

		void Process() override {
			if (m_Inputs[0]->GetValue<int>() >= 0 && m_BluePrint->m_sc->GetObj(m_Inputs[0]->GetValue<int>())) {
				auto ent = entt::entity(m_Inputs[0]->GetValue<int>());
				GameObject go(ent, m_BluePrint->m_sc);
				
				auto ai = ResourceManager::GetAudioClip(m_Inputs[1]->GetValue<string>())->Intantiate();
				
				if (go.HasComponent<AudioSourceComponent>() == false) {
					//go.AddComponent<AudioSourceComponent>();
					XRE_ERROR("对象不含有声音源组件");
				}
				else {
					if (ai != nullptr) {
						go.GetComponent<AudioSourceComponent>().auInstances.push_back(ai);
						ai->m_Loop = m_loop;
						ai->Play();

						ai->SetPos(go.GetComponent<TransformComponent>().GetGlobalTranslation());
					}
					
				}
				
				
			}
		}
		int mousecode;
	};
}