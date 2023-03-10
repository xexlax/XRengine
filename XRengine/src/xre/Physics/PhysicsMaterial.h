#pragma once

#include <glm/glm.hpp>
#include <cereal\archives\json.hpp>
#include <glm/gtx/quaternion.hpp>

namespace XRE{

	struct PhysicsMaterial {

		bool changed = false;

		float m_GravityFactor = 1.0f;
		float m_Friction = 0.2f;
		float m_Restitution = 0.2f;
		float m_LinearDampling = 0.0f;
		float m_AngularDampling = 0.0f;


		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_GravityFactor,m_Friction,m_Restitution,m_LinearDampling,m_AngularDampling);
		}

			
	};
}