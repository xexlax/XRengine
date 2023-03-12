#pragma once


#include <glm\glm.hpp>

#include "xre\ECS\Components.h"
#include "xre\Physics\Collision.h"




namespace XRE {

    
    

    class PhysicsScene {
    public:
        virtual ~PhysicsScene() = default;

        virtual uint32_t CreateRigidBody(TransformComponent& tc, const RigidBodyComponent& rbc )=0;
        virtual void     RemoveRigidBody(uint32_t body_id)=0;

        //在每一步模拟前更新RB的状态
        virtual void     ResetRigidBody(TransformComponent& tc, RigidBodyComponent& rbc)=0;
        //每一步模拟后更新Transform的状态
        virtual void     UpdateRigidBody(TransformComponent& tc, RigidBodyComponent& rbc)=0;

        virtual bool     RayCast(glm::vec3 origin, glm::vec3 direction, float max_length, std::vector<PhysicsHitInfo>& output)=0;

        static XRef<PhysicsScene> Create();
        
        virtual void OnUpdate(float dt)=0;


    };
}