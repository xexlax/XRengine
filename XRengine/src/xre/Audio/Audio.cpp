#include "Audio.h"
#include "alManager.h"

namespace XRE {
	AudioClip::AudioClip(std::string fp):
		m_FilePath(fp)
	{
	}
	XRef<AudioClip> AudioClip::Create(std::string path)
	{
		return XMakeRef<AudioClip>(path);
	}
	XRef<AudioInstance> AudioClip::Intantiate()
	{
		uint32_t m_ID = alManager::LoadWaveToBuffer(m_FilePath.c_str());
		XRef<AudioInstance> ai = XMakeRef<AudioInstance>(m_ID);
		
		return ai;
	}
	
}