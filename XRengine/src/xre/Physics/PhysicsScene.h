#pragma once


#include <glm\glm.hpp>
#include "PhysicsUtils.h"
#include "PhysicsConfig.h"
#include "xre\ECS\Components.h"

namespace JPH
{
    class PhysicsSystem;
    class JobSystem;
    class TempAllocator;
    class BroadPhaseLayerInterface;
#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
    class DebugRenderer;
#endif
} 


namespace XRE {
    struct JoltPhysics
    {
        JPH::PhysicsSystem* m_PhysicsSystem{ nullptr };
        JPH::JobSystem* m_JobSystem{ nullptr };
        JPH::TempAllocator* m_TempAllocator{ nullptr };
        JPH::BroadPhaseLayerInterface* m_BroadPhaseLayerInterface{ nullptr };
        JPH::ObjectVsBroadPhaseLayerFilter* m_OVBPhaseLayerFilter{ nullptr };
        JPH::ObjectLayerPairFilter* m_ObjectLayerPairFilter{ nullptr };
        

        int m_collision_steps{ 1 };
        int m_integration_substeps{ 1 };

        
    };

    class PhysicsScene {
    public:
        PhysicsScene();
        virtual ~PhysicsScene();
        void Init();
        const glm::vec3& getGravity() const { return m_Config.m_Gravity; }


        uint32_t CreateRigidBody(const TransformComponent& tc, const RigidBodyComponent& rbc );
        void     RemoveRigidBody(uint32_t body_id);
        
        void OnUpdate(float dt);
    protected:
  
        JoltPhysics m_physics;

        PhysicsConfig m_Config;


        //待删除列表，每次Update时清除
        std::vector<uint32_t> m_PendingRemove;

        JPH::Body* m_floor = nullptr;
        JPH::BodyID m_sphere_id;
    };
}