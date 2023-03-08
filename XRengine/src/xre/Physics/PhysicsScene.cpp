#include "PhysicsScene.h"
#include "pch.h"
#include "xre\Core\Macros.h"
#include <xre\Utils\Math.h>
#pragma once
using namespace JPH;
using namespace JPH::literals;
namespace XRE {

	

	PhysicsScene::PhysicsScene()
	{
		// Register allocation hook
		RegisterDefaultAllocator();

		
		Factory::sInstance = new Factory();

		// Register all Jolt physics types
		RegisterTypes();

		m_physics.m_TempAllocator = new TempAllocatorImpl(10 * 1024 * 1024);
		m_physics.m_JobSystem = new JobSystemThreadPool(m_Config.m_MaxJobCount,m_Config.m_MaxBarrierCount, thread::hardware_concurrency() - 1);

		m_physics.m_BroadPhaseLayerInterface = new BPLayerInterfaceImpl();
		m_physics.m_OVBPhaseLayerFilter = new ObjectVsBroadPhaseLayerFilterImpl();
		m_physics.m_ObjectLayerPairFilter = new ObjectLayerPairFilterImpl();

		m_physics.m_PhysicsSystem = new PhysicsSystem();
		m_physics.m_PhysicsSystem->Init(m_Config.m_MaxBodyCount,m_Config.m_BodyMutexCount,m_Config.m_MaxBodyPairs, 
			m_Config.m_MaxContactConstraints, *m_physics.m_BroadPhaseLayerInterface, *m_physics.m_OVBPhaseLayerFilter, *m_physics.m_ObjectLayerPairFilter);


		// optional Listeners
		
		/*m_physics.m_PhysicsSystem->SetBodyActivationListener(&m_BodyActivationListener);
		m_physics.m_PhysicsSystem->SetContactListener(&m_ContactListener);*/

		// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
		// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
		

	
		// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
		m_physics.m_PhysicsSystem->OptimizeBroadPhase();

	}
	PhysicsScene::~PhysicsScene()
	{
		delete m_physics.m_PhysicsSystem;
		delete m_physics.m_JobSystem;
		delete m_physics.m_TempAllocator;
		delete m_physics.m_BroadPhaseLayerInterface;
		// Unregisters all types with the factory and cleans up the default material
		UnregisterTypes();

		// Destroy the factory
		delete Factory::sInstance;
		Factory::sInstance = nullptr;
	}

	uint32_t PhysicsScene::CreateRigidBody(TransformComponent& tc, const RigidBodyComponent& rbc)
	{
		auto& BodyInterface = m_physics.m_PhysicsSystem->GetBodyInterface();
		ShapeRefC body_shape;
		RVec3 pos(tc.m_Translation.x, tc.m_Translation.y, tc.m_Translation.z);
		auto q = glm::quat(glm::radians(tc.m_Rotation));

		Quat rot(q.x, q.y, q.z, q.w);

		if (rbc.m_RigidBodyShape == RigidBodyComponent::RigidBodyShape::Box) {

			BoxShapeSettings box_shape_settings(Vec3(tc.m_Scale.x/2, tc.m_Scale.y/2, tc.m_Scale.z/2));
			body_shape = box_shape_settings.Create().Get();

		}
		else if (rbc.m_RigidBodyShape == RigidBodyComponent::RigidBodyShape::Sphere) {

			SphereShapeSettings sphere_shape_settings(tc.m_Scale.y / 2);
			body_shape = sphere_shape_settings.Create().Get();

		}

		
		EMotionType emotionType = EMotionType(rbc.m_MotionType);
		uint8_t layer=0;
		if (emotionType == EMotionType::Static) layer = Layers::NON_MOVING;
		else layer = Layers::MOVING;
			 
		BodyCreationSettings body_settings(body_shape, pos, rot, emotionType, layer);
		JPH::BodyID objID = BodyInterface.CreateAndAddBody(body_settings, EActivation::Activate);
		uint32_t id = objID.GetIndexAndSequenceNumber();


		return id;
	}
	void PhysicsScene::RemoveRigidBody(uint32_t body_id)
	{

		m_PendingRemove.push_back(body_id);

	}

	//更新前
	void PhysicsScene::ResetRigidBody(TransformComponent& tc, RigidBodyComponent& rbc)
	{
		BodyID id(rbc.m_PhysicObj);

		auto& bodyInterface = m_physics.m_PhysicsSystem->GetBodyInterface();
		if(bodyInterface.GetMotionType(id)!=EMotionType(rbc.m_MotionType))
		bodyInterface.SetMotionType(id, EMotionType(rbc.m_MotionType), EActivation::Activate);


		

	}
	//更新后
	void PhysicsScene::UpdateRigidBody(TransformComponent& tc,RigidBodyComponent &rbc)
	{
		
			BodyID id(rbc.m_PhysicObj);

			auto& bodyInterface = m_physics.m_PhysicsSystem->GetBodyInterface();
			
			if (bodyInterface.IsActive(id)&& bodyInterface.GetMotionType(id)!=EMotionType::Static) {
				
				JPH::RMat44 trans = bodyInterface.GetCenterOfMassTransform(id);
				glm::mat4 tr;

				for (int i = 0;i < 4;i++) {
					for (int j = 0; j < 4;j++) {
						tr[i][j] = trans.GetColumn4(i)[j];
					}
				}
				glm::vec3 t(1.0f);
				glm::vec3 rot(0.0f);
				Math::DecomposeTransform(tr,tc.m_Translation, rot,t );
				tc.m_Rotation = glm::degrees(rot);
			}

		
	}
	void PhysicsScene::OnUpdate(float dt)
	{
		auto& bodyInterface = m_physics.m_PhysicsSystem->GetBodyInterface();
		// Next step

		
		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = 1;

		// If you want more accurate step results you can do multiple sub steps within a collision step. Usually you would set this to 1.
		const int cIntegrationSubSteps = 1;




		// Step the world
		m_physics.m_PhysicsSystem->Update( dt, cCollisionSteps, cIntegrationSubSteps, m_physics.m_TempAllocator, m_physics.m_JobSystem);

		for (uint32_t body_id : m_PendingRemove)
		{
			XRE_INFO("Remove Body {0}", body_id);
			bodyInterface.RemoveBody(JPH::BodyID(body_id));
			bodyInterface.DestroyBody(JPH::BodyID(body_id));
		}
		m_PendingRemove.clear();


	}
}