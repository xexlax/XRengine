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
		case 60:
			return XMakeRef<Node_Self>();
		case 61:
			return XMakeRef<Node_GetPosition>();
		case 62:
			return XMakeRef<Node_SetPosition>();
		
		default:
			return nullptr;
		}
	}
}