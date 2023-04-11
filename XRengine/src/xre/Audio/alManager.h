#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <glm/glm.hpp>

//����ʹ��openAL����3Dʵʱ��Ч�ķ�װ��
//���openAL�Ĺ��ܽ����˼򻯷�װ
//ÿ����Դֻ֧��һ��wav�ļ��Ĳ��ţ�����Ҳ���ߣ����ǽӿ�ʮ�ּ��
//�����Բ���24·3D��Ƶ
//�����趨��λ�ú��������������������������
//���ߣ�yinan Miao from Beihang Univ (XRE���иĶ���
//����.h��.cppһ�ף�����BSD����Э�顣����Ӧ�����κ���������Ҫ��Դ����ֲ�ע�����ߣ�

using namespace std;

#define MAX_BUFFER_SIZE 24//���֧��24·�������޸�

//---------------------------------------------------------------
//WAV����������أ��Ӷ��޹ػ�alut����������ģ��
struct WAVE_Data {//Wav�ļ�������ģ��
    char subChunkID[4]; //should contain the word data
    long subChunk2Size; //Stores the size of the data block
};

struct WAVE_Format {//wav�ļ����ݲ�������
    char subChunkID[4];
    long subChunkSize;
    short audioFormat;
    short numChannels;
    long sampleRate;
    long byteRate;
    short blockAlign;
    short bitsPerSample;
};

struct RIFF_Header {//RIFF���׼ģ��
    char chunkID[4];
    long chunkSize;//size not including chunkSize or chunkID
    char format[4];
};
//----------------------------------------------------------------

class alManager
{
    static alManager* s_ALManager;
private:
    ALCdevice* device;//�豸ָ��
    ALCcontext* context;//������ָ��
    ALCboolean bEAX;//������Ч��չ��������׼��

    uint32_t bufferid = 0;
    ALuint buffers[MAX_BUFFER_SIZE + 1];//���ڴ�����Ƶ���ݵĻ���ID
    ALuint sources[MAX_BUFFER_SIZE + 1];//���ڴ�����ԴID
    bool loadWavFile(const string filename, ALuint buffer, ALsizei* size, ALsizei* frequency, ALenum* format);//�����ļ�������wav�ļ���ָ����bufferID���С�
    /*
    Copyright (c) 2018, yinan Miao;
    All rights reserved.
    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    Neither the name of the Beihang Univ. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    */

    void loadWav2Buffer(ALuint bufferI, const char* fileName);//�����ļ�������ָ��wav�ļ������Ұ󶨵�ָ��ID��buffer����Դ��
    void setListenerPos(glm::vec3 pos, glm::vec3 vel, glm::vec3 at, glm::vec3 up);//�������ߵ�λ�á��ٶȡ������������Լ�ͷ������������
    void setSourcePos(int sI, glm::vec3 pos, glm::vec3 vel, glm::vec3 at);//����ָ��ID��Դ��λ�á��ٶȡ�����������
    void setSourcePhy(int sI, float Gain, float maxDis, float halfDistance, float rollOff = 1.0);//����ָ��ID��Դ���������������ϵ������󴫲����롢˥�������Լ���˥������ʵ���ź�= (GAIN-20*log10(1+ROLLOFF*(����-��˥����)/��˥����))ԭʼ�ź�
    void play(int sI);//����ָ��ID����Դ
    int getSourceId(int sI);//�õ�ָ��ID��ʵ����ԴID����openAL�����ѿ���ʹ��������Զ������
    void init();//��ʼ��openAL
    void end();//�ر�openAL


public:
    alManager();//���캯����û��ʵ�����á�����Ϊ��openAL���趨�У�ֻ������һ��OPENAL�������ģ�����ڱ�����趨�в����ظ�������ʼ����
    


    static inline void Init() { 
        s_ALManager = new alManager();
        s_ALManager->init(); 
    }
    static inline void End() { s_ALManager->end(); }
    static int LoadWaveToBuffer(const char* fileName) {
        int id = s_ALManager->bufferid;
        if (s_ALManager->bufferid > MAX_BUFFER_SIZE + 1) {
            s_ALManager->bufferid = 0;
            id = 0;
        }
        s_ALManager->loadWav2Buffer(s_ALManager->bufferid, fileName);
        s_ALManager->bufferid++;

        return id;
        
    }
    static inline void SetListenerPos(glm::vec3 pos, glm::vec3 vel = glm::vec3(0.0f), glm::vec3 at = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0,1.0f,0)) {
        s_ALManager->setListenerPos(pos, vel, at, up);
    }

    static inline void SetSourcePos(int sI, glm::vec3 pos, glm::vec3 vel = glm::vec3(0.0f), glm::vec3 at = glm::vec3(0.0f)) {
        s_ALManager->setSourcePos(sI, pos, vel, at);
    }
    static inline void SetSourcePhy(int sI, float Gain, float maxDis, float halfDistance, float rollOff = 1.0) {
        s_ALManager->setSourcePhy(sI, Gain, maxDis, halfDistance, rollOff);
    }
    static inline void Play(int sI) {
        s_ALManager->play(sI);
    }
    static inline int GetSourceId(int sI) {
        return s_ALManager->getSourceId(sI);
    }
};
