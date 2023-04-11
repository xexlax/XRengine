#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <glm/glm.hpp>

//用于使用openAL播放3D实时音效的封装类
//针对openAL的功能进行了简化封装
//每个音源只支持一个wav文件的播放，总数也不高，但是接口十分简洁
//最多可以播放24路3D音频
//根据设定的位置和物理参数计算左右声道的音量
//作者：yinan Miao from Beihang Univ (XRE中有改动）
//共计.h与.cpp一套，遵守BSD开发协议。（可应用于任何领域但是需要在源码和手册注明作者）

using namespace std;

#define MAX_BUFFER_SIZE 24//最多支持24路，可以修改

//---------------------------------------------------------------
//WAV数据类型相关，从而无关化alut，减少外引模块
struct WAVE_Data {//Wav文件数据体模块
    char subChunkID[4]; //should contain the word data
    long subChunk2Size; //Stores the size of the data block
};

struct WAVE_Format {//wav文件数据参数类型
    char subChunkID[4];
    long subChunkSize;
    short audioFormat;
    short numChannels;
    long sampleRate;
    long byteRate;
    short blockAlign;
    short bitsPerSample;
};

struct RIFF_Header {//RIFF块标准模型
    char chunkID[4];
    long chunkSize;//size not including chunkSize or chunkID
    char format[4];
};
//----------------------------------------------------------------

class alManager
{
    static alManager* s_ALManager;
private:
    ALCdevice* device;//设备指针
    ALCcontext* context;//上下文指针
    ALCboolean bEAX;//环境音效扩展（声卡标准）

    uint32_t bufferid = 0;
    ALuint buffers[MAX_BUFFER_SIZE + 1];//用于储存音频数据的缓存ID
    ALuint sources[MAX_BUFFER_SIZE + 1];//用于储存音源ID
    bool loadWavFile(const string filename, ALuint buffer, ALsizei* size, ALsizei* frequency, ALenum* format);//根据文件名载入wav文件到指定的bufferID当中。
    /*
    Copyright (c) 2018, yinan Miao;
    All rights reserved.
    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    Neither the name of the Beihang Univ. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    */

    void loadWav2Buffer(ALuint bufferI, const char* fileName);//根据文件名载入指定wav文件，并且绑定到指定ID的buffer和音源。
    void setListenerPos(glm::vec3 pos, glm::vec3 vel, glm::vec3 at, glm::vec3 up);//设置听者的位置、速度、脸朝向向量以及头顶朝向向量。
    void setSourcePos(int sI, glm::vec3 pos, glm::vec3 vel, glm::vec3 at);//设置指定ID音源的位置、速度、朝向向量。
    void setSourcePhy(int sI, float Gain, float maxDis, float halfDistance, float rollOff = 1.0);//设置指定ID音源的物理参数：缩放系数、最大传播距离、衰减参数以及半衰参数。实际信号= (GAIN-20*log10(1+ROLLOFF*(距离-半衰距离)/半衰距离))原始信号
    void play(int sI);//播放指定ID的音源
    int getSourceId(int sI);//得到指定ID的实际音源ID，懂openAL的朋友可以使用其进行自定义操作
    void init();//初始化openAL
    void end();//关闭openAL


public:
    alManager();//构造函数，没有实际作用。（因为在openAL的设定中，只能启动一个OPENAL的上下文，因此在本类的设定中不能重复定义或初始化）
    


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
