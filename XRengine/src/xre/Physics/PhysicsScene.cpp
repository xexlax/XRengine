#include "PhysicsScene.h"
#include "pch.h"
#include "xre\Core\Macros.h"
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
	void PhysicsScene::Init()
	{

		BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));

		// Create the shape
		ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
		ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

		// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
		BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);

		auto m_BodyInterface = &m_physics.m_PhysicsSystem->GetBodyInterface();
		// Create the actual rigid body
		m_floor = m_BodyInterface->CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr

		// Add it to the world
		m_BodyInterface->AddBody(m_floor->GetID(), EActivation::DontActivate);

		// Now create a dynamic body to bounce on the floor
		// Note that this uses the shorthand version of creating and adding a body to the world
		BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0_r, 2.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
		m_sphere_id = m_BodyInterface->CreateAndAddBody(sphere_settings, EActivation::Activate);

		// Now you can interact with the dynamic body, in this case we're going to give it a velocity.
		// (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
		m_BodyInterface->SetLinearVelocity(m_sphere_id, Vec3(0.0f, 0.0f, 0.0f));

	}
	uint32_t PhysicsScene::CreateRigidBody(const TransformComponent& tc, const RigidBodyComponent& rbc)
	{




		auto m_BodyInterface = &m_physics.m_PhysicsSystem->GetBodyInterface();



		return uint32_t();
	}
	void PhysicsScene::RemoveRigidBody(uint32_t body_id)
	{
	}
	void PhysicsScene::OnUpdate(float dt)
	{
		auto m_BodyInterface = &m_physics.m_PhysicsSystem->GetBodyInterface();
		if (!m_BodyInterface->IsActive(m_sphere_id))return;
		// Next step

		// Output current position and velocity of the sphere
		RVec3 position = m_BodyInterface->GetCenterOfMassPosition(m_sphere_id);
		Vec3 velocity = m_BodyInterface->GetLinearVelocity(m_sphere_id);
		XRE_INFO("Position: {0},{1},{2}", position.GetX(), position.GetY(), position.GetZ());
		

		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = 1;

		// If you want more accurate step results you can do multiple sub steps within a collision step. Usually you would set this to 1.
		const int cIntegrationSubSteps = 1;



		// Step the world
		m_physics.m_PhysicsSystem->Update( 1.0f/60.0f, cCollisionSteps, cIntegrationSubSteps, m_physics.m_TempAllocator, m_physics.m_JobSystem);

	}
}