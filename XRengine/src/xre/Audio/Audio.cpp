#include "Audio.h"
#include "alManager.h"


namespace XRE {
	AudioClip::~AudioClip()
	{
		alManager::ReleaseBuffer(m_BufferID);
	}
	AudioClip::AudioClip(std::string fp):
		m_FilePath(fp)
	{
		m_BufferID = alManager::LoadWaveToBuffer(fp.c_str());
	}
	XRef<AudioClip> AudioClip::Create(std::string path)
	{
		return XMakeRef<AudioClip>(path);
	}
	XRef<AudioInstance> AudioClip::Intantiate()
	{
		uint32_t sid = alManager::BindBufferToSrc(m_BufferID);
		if (sid == MAX_BUFFER_SIZE)return nullptr;
		XRef<AudioInstance> ai = XMakeRef<AudioInstance>(sid);
		
		return ai;
	}
	
	bool AudioInstance::TestEnded()
	{
		if (m_Loop == false && alManager::IsSrcEnded(m_SrcID)) {
			Stop();
			return true;
		}
		return false;
	}

	void AudioInstance::Play()
	{
		
		alManager::Play(m_SrcID);
		alManager::SetSrcLoop(m_SrcID,m_Loop);
		alManager::SetSourcePhy(m_SrcID, 1.0f, 100.0f, 3.0f);
		//alManager::SetSourcePos(m_ID, pos);
		//alManager::SetListenerPos(glm::vec3(100.0f,0,0));
		
	}

	void AudioInstance::Pause()
	{
		alManager::Pause(m_SrcID);
	}

	void AudioInstance::Stop()
	{
		alManager::Stop(m_SrcID);
	}

	void AudioInstance::SetPos(glm::vec3 pos)
	{
		alManager::SetSourcePos(m_SrcID, pos);
	}

}