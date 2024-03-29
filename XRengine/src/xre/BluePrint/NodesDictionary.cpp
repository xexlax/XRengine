#pragma once

#include "NodesDictionary.h"
#include "Nodes_Basic.h"
#include "Nodes_Math.h"
#include "Nodes_ControlFlow.h"
#include "Nodes_IO.h"
#include "Nodes_ComponentUtils.h"
namespace XRE::Nodes {

	

	XRef<BluePrintNode> MakeNode(int id) {
		
		switch (id)
		{
			//Basic
		case 0:
			return XMakeRef<Node_Desc>();
		case 1:
			return XMakeRef<Node_Debug>();
		case 2:
			return XMakeRef<Node_DeltaTime>();
		case 3:
			return XMakeRef<Node_ConstInt>();
		case 4:
			return XMakeRef<Node_ConstBool>();
		case 5:
			return XMakeRef<Node_ConstFloat>();
		case 6:
			return XMakeRef<Node_ConstString>();
		case 11:
			return XMakeRef<Node_GetField>();
		case 12:
			return XMakeRef<Node_SetField>();
		case 13:
			return XMakeRef<Node_TotalTime>();
		case 14:
			return XMakeRef<Node_Rand>();
		case 21:
			return XMakeRef<Node_Plus>();
		case 22:
			return XMakeRef<Node_Minus>();
		case 23:
			return XMakeRef<Node_Multiply>();
		case 24:
			return XMakeRef<Node_Division>();
		case 25:
			return XMakeRef<Node_Mod>();
		case 26:
			return XMakeRef<Node_Sin>();
		case 27:
			return XMakeRef<Node_Cos>();
		case 28:
			return XMakeRef<Node_Tan>();
		case 29:
			return XMakeRef<Node_RadToDegree>();
		case 30:
			return XMakeRef<Node_DegreeToRad>();
		//String
		case 36:
			return XMakeRef<Node_StrCat>();
		case 37:
			return XMakeRef<Node_StrCmp>();
		case 38:
			return XMakeRef<Node_StrFind>();
		//Logic
		case 41:
			return XMakeRef<Node_And>();
		case 42:
			return XMakeRef<Node_Or>();
		case 43:
			return XMakeRef<Node_Not>();
		case 44:
			return XMakeRef<Node_Equal>();
		case 45:
			return XMakeRef<Node_Compare>();
		case 46:
			return XMakeRef<Node_EqualOrCompare>();
		case 47:
			return XMakeRef<Node_UnEqual>();

		case 51:
			return XMakeRef<Node_Branch>();
		case 52:
			return XMakeRef<Node_OnSceneBegin>();
		case 53:
			return XMakeRef<Node_OnCollision>();

		case 54:
			return XMakeRef<Node_OnTrigger>();

		case 59:
			return XMakeRef<Node_GetByName>();
		case 60:
			return XMakeRef<Node_Self>();
		//Transform
		case 61:
			return XMakeRef<Node_GetPosition>();
		case 62:
			return XMakeRef<Node_SetPosition>();
		case 63:
			return XMakeRef<Node_GetRotation>();
		case 64:
			return XMakeRef<Node_SetRotation>();
		case 65:
			return XMakeRef<Node_GetScale>();
		case 66:
			return XMakeRef<Node_SetScale>();
		case 67:
			return XMakeRef<Node_SetLinearVelocity>();
		case 68:
			return XMakeRef<Node_SetAngularVelocity>();

		case 69:
			return XMakeRef<Node_Create>();
		case 70:
			return XMakeRef<Node_Destroy>();
		case 71:
			return XMakeRef<Node_GetBluePrintField>();
		case 72:
			return XMakeRef<Node_SetBluePrintField>();
		case 73:
			return XMakeRef<Node_SetMaterial>();
		case 74:
			return XMakeRef<Node_SetRigidBodyMotion>();
		case 75:
			return XMakeRef<Node_SetPointLight>();

		//Input
		case 81:
			return XMakeRef<Node_OnKeyEvent>();
		case 82:
			return XMakeRef<Node_OnMouseEvent>();

		case 85:
			return XMakeRef<Node_PlayAudio>();
		default:
			return nullptr;
		}
	}
}