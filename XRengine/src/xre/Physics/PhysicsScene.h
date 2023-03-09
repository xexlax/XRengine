#pragma once


#include <glm\glm.hpp>

#include "xre\ECS\Components.h"




namespace XRE {
    

    class PhysicsScene {
    public:
        virtual ~PhysicsScene() = default;

        virtual uint32_t CreateRigidBody(TransformComponent& tc, const RigidBodyComponent& rbc )=0;
        virtual void     RemoveRigidBody(uint32_t body_id)=0;
        virtual void     ResetRigidBody(TransformComponent& tc, RigidBodyComponent& rbc)=0;
        virtual void     UpdateRigidBody(TransformComponent& tc, RigidBodyComponent& rbc)=0;

        static XRef<PhysicsScene> Create();
        
        virtual void OnUpdate(float dt)=0;


    };
}