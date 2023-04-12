#pragma once
#include <string>
#include <glm/glm.hpp>

namespace XRE {
	class AudioInstance;

	class AudioClip {


	public:

		AudioClip() = default;
		~AudioClip();
		AudioClip(std::string fp);
		static XRef<AudioClip> Create(std::string path);

		XRef<AudioInstance> Intantiate();


		
		uint32_t m_BufferID;
		

		template <class AR>
		void serialize(AR& ar) {
			ar(m_FilePath);
		}
		
	private:
		
		std::string m_FilePath = "";
	};
	class AudioInstance {
		uint32_t m_SrcID;
	public:
		AudioInstance(uint32_t id) : m_SrcID(id) {};
		bool TestEnded();
		void Play();
		void Pause();
		void Stop();
		void SetPos(glm::vec3  pos);
		bool m_PlayState;
		float m_Strength = 1.0f;
		bool m_Loop = false;
		float m_TimePosition;
	};
	
}