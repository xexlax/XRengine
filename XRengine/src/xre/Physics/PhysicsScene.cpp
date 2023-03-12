#include "PhysicsScene.h"
#include "PhysicsAPI.h"
#include "pch.h"
#include "xre\Core\Macros.h"
#include <xre\Utils\Math.h>

#include "Platforms\JoltPhysics\JoltPhysicsScene.h"
#pragma once

namespace XRE {
	

	

	
	

	XRef<PhysicsScene> PhysicsScene::Create()
	{

		switch (PhysicsAPI::GetAPI())
		{
		case PhysicsAPI::API::Jolt:
			return XMakeRef<JoltPhysicsScene>();
		default:
			XRE_CORE_ASSERT(false, "不存在的物理API")
			break;
		}
		
	}

}