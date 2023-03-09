#pragma once

#include <glm/glm.hpp>
#include <cereal\archives\json.hpp>
#include "xre\ECS\Components.h"

namespace XRE {


	struct PhysicsShape {


		enum ShapeType {
			Box, Sphere, Capsule
		} m_Type;

		void SetType(int i) {
			m_Type = ShapeType(i);
		}


		//m_Radius = m_Size.x;
		//m_Length = m_Size.y;
		glm::vec3 m_Size;
		
		glm::vec3 m_Offset;
		glm::vec3 m_Rotation;


		template <class Archive>
		void serialize(Archive& ar)
		{
			ar( m_Size.x, m_Size.y,m_Size.z, 
				m_Offset.x, m_Offset.y ,m_Offset.z,
				m_Rotation.x,m_Rotation.y,m_Rotation.z);
		}



		PhysicsShape() {
			m_Type = Box;
			m_Size = glm::vec3(1.0f);
			m_Offset = glm::vec3(0.0f);
			m_Rotation = glm::vec3(0.0f);
		}

	};



}