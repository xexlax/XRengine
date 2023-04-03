#pragma once

namespace XRE {
	class AudioClip {


	public:
		static XRef<AudioClip> Create();


	private:
		std::string m_FilePath = "";
	};
}