#pragma once
#include <string>


namespace XRE {
	class AudioInstance;

	class AudioClip {


	public:

		AudioClip() = default;
		AudioClip(std::string fp);
		static XRef<AudioClip> Create(std::string path);

		XRef<AudioInstance> Intantiate();


		bool m_Loop = false;
		float m_Strength = 1.0f;

		template <class AR>
		void serialize(AR& ar) {
			ar(m_FilePath, m_Loop,m_Strength);
		}
		
	private:
		
		std::string m_FilePath = "";
	};
	class AudioInstance {
		uint32_t m_ID;
	public:
		AudioInstance(uint32_t id) : m_ID(id) {};

		XRef<AudioClip> m_Prototype;
		bool m_PlayState;
		float m_TimePosition;
	};
	
}