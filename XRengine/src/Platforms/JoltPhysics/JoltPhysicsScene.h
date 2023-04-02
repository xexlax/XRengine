#pragma once

#include "xre\Physics\PhysicsScene.h"
#include "JoltPhysicsConfig.h"
#include "JoltPhysicsUtils.h"

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
    class JoltPhysicsScene: public PhysicsScene {
    public:
        JoltPhysicsScene();
        virtual ~JoltPhysicsScene();


        uint32_t CreateRigidBody(TransformComponent& tc, const RigidBodyComponent& rbc);
        void     RemoveRigidBody(uint32_t body_id);
        void     ResetRigidBody(TransformComponent& tc, RigidBodyComponent& rbc);
        void     UpdateRigidBody(TransformComponent& tc, RigidBodyComponent& rbc);
        bool     RayCast(glm::vec3 origin, glm::vec3 direction, float max_length, std::vector<PhysicsHitInfo>& output);
        void     UpdateCollision(TransformComponent& tc, RigidBodyComponent& rbc);

        

        void OnUpdate(float dt);
    protected:

        JoltPhysics m_physics;
        JPH::MyContactListener m_contact_listener;
        
      
        JPH::ShapeRefC CreateShape(TransformComponent& tc,const RigidBodyComponent& rbc);

        JoltPhysicsConfig m_Config;

        //待删除列表，每次Update时清除
        std::vector<uint32_t> m_PendingRemove;

    };
}