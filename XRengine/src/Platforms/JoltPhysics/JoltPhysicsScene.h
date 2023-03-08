#pragma once

#include "xre\Physics\PhysicsScene.h"

namespace XRE {
    class JoltPhysicsScene: public PhysicsScene {
    public:
        JoltPhysicsScene();
        virtual ~JoltPhysicsScene();
        const glm::vec3& getGravity() const { return m_Config.m_Gravity; }


        uint32_t CreateRigidBody(TransformComponent& tc, const RigidBodyComponent& rbc);
        void     RemoveRigidBody(uint32_t body_id);
        void     ResetRigidBody(TransformComponent& tc, RigidBodyComponent& rbc);
        void     UpdateRigidBody(TransformComponent& tc, RigidBodyComponent& rbc);

        static XRef<PhysicsScene> Create();

        void OnUpdate(float dt);
    protected:

        JoltPhysics m_physics;

        PhysicsConfig m_Config;

        //��ɾ���б�ÿ��Updateʱ���
        std::vector<uint32_t> m_PendingRemove;

    };
}