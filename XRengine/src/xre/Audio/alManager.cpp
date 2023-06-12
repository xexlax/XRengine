#include "alManager.h"
#include "pch.h"
#include "GLFW\glfw3.h"


alManager* alManager::s_ALManager = nullptr;
//以下文件导入函数从开源社区无共享协议修改而来，可以读取文件的信息并且绑定指定bufferID。
bool alManager::loadWavFile(const std::string filename, ALuint buffer, ALsizei* size, ALsizei* frequency, ALenum* format)
{
    FILE* soundFile = NULL;
    WAVE_Format wave_format;
    RIFF_Header riff_header;
    WAVE_Data wave_data;
    unsigned char* data;
    float dt = glfwGetTime();
    XRE_INFO("reading wav:{0}", filename);
    try {

        soundFile = fopen(filename.c_str(), "rb");
        if (!soundFile)
            throw (filename);
        // Read in the first chunk into the struct
        fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

        //check for RIFF and WAVE tag in memeory
        if ((riff_header.chunkID[0] != 'R' ||
            riff_header.chunkID[1] != 'I' ||
            riff_header.chunkID[2] != 'F' ||
            riff_header.chunkID[3] != 'F') ||
            (riff_header.format[0] != 'W' ||
                riff_header.format[1] != 'A' ||
                riff_header.format[2] != 'V' ||
                riff_header.format[3] != 'E'))
        {
            throw ("Invalid RIFF or WAVE Header");
        }

        //Read in the 2nd chunk for the wave info
        fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
        //check for fmt tag in memory
        if (wave_format.subChunkID[0] != 'f' ||
            wave_format.subChunkID[1] != 'm' ||
            wave_format.subChunkID[2] != 't' ||
            wave_format.subChunkID[3] != ' ')
        {
            throw ("Invalid Wave Format");
        }

        //check for extra parameters;
        if (wave_format.subChunkSize > 16)
            fseek(soundFile, sizeof(short), SEEK_CUR);

        //Read in the the last byte of data before the sound file
        fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        //check for data tag in memory
        /*if (wave_data.subChunkID[0] != 'd' ||
            wave_data.subChunkID[1] != 'a' ||
            wave_data.subChunkID[2] != 't' ||
            wave_data.subChunkID[3] != 'a')
            if (wave_data.subChunkID[0] != 'L' ||
                wave_data.subChunkID[1] != 'I' ||
                wave_data.subChunkID[2] != 'S' ||
                wave_data.subChunkID[3] != 'T')
        {

            XRE_ERROR("{0},{1},{2},{3}", wave_data.subChunkID[0],wave_data.subChunkID[1] , wave_data.subChunkID[2] ,wave_data.subChunkID[3]);
            
            throw ("Invalid data header");
        }*/

        //Allocate memory for data
        data = new unsigned char[wave_data.subChunk2Size];

        // Read in the sound data into the soundData variable
        if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
            throw ("error loading WAVE data into struct!");

        //Now we set the variables that we passed in with the
        //data from the structs
        *size = wave_data.subChunk2Size;
        *frequency = wave_format.sampleRate;
        //The format is worked out by looking at the number of
        //channels and the bits per sample.
        if (wave_format.numChannels == 1)
        {
            if (wave_format.bitsPerSample == 8)
                *format = AL_FORMAT_MONO8;
            else if (wave_format.bitsPerSample == 16)
                *format = AL_FORMAT_MONO16;
        }
        else if (wave_format.numChannels == 2)
        {
            if (wave_format.bitsPerSample == 8)
                *format = AL_FORMAT_STEREO8;
            else if (wave_format.bitsPerSample == 16)
                *format = AL_FORMAT_STEREO16;
        }
        //now we put our data into the openAL buffer and
        //check for success
        alBufferData(buffer, *format, (void*)data, *size, *frequency);
        //errorCheck();
        //clean up and return true if successful
        fclose(soundFile);

        XRE_CORE_INFO("{0}:{1}", filename, glfwGetTime() - dt);
        return true;
    }
    catch (char* error)
    {
        XRE_ERROR(" {0}: trying to load {1}", error, filename);
        if (soundFile != NULL)
            fclose(soundFile);
        system("pause");
        return false;
    }


}

alManager::alManager()
{
    //撒也没有
}

void alManager::loadWav2Buffer(ALuint bufferI, const char* fileName)
{
    ALsizei size;
    ALsizei freq;
    ALenum format;
    loadWavFile(fileName, buffers[bufferI], &size, &freq, &format);//读取到bufferID。
    buffersAvail[bufferI] = false;
    
}

void alManager::setListenerPos(glm::vec3 pos)
{

   
    
  
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    
    
    
}

void alManager::setListenerOrientation(glm::vec3 front, glm::vec3 up)
{
    ALfloat o[6] = { front.x,front.y,front.z,up.x,up.y,up.z };
    
    alListenerfv(AL_ORIENTATION,o );
}

void alManager::setSourcePos(int sI, glm::vec3 pos)
{
    //设置位置
    alSource3f(sources[sI], AL_POSITION, pos.x, pos.y, pos.z);
   
    
}

void alManager::setSourcePhy(int sI, float Gain, float maxDis, float halfDistance, float rollOff)
{
    
    //设置放大系数、最大距离、半衰距离以及衰减参数
    alSourcef(sources[sI], AL_GAIN, Gain);
    alSourcef(sources[sI], AL_MAX_DISTANCE, maxDis);
    alSourcef(sources[sI], AL_ROLLOFF_FACTOR, rollOff);
    alSourcef(sources[sI], AL_REFERENCE_DISTANCE, halfDistance);
}

void alManager::setSourceLoop(int sI, bool loop)
{

    alSourcei(sources[sI], AL_LOOPING,loop? AL_TRUE :AL_FALSE);
}

void alManager::play(int sI)
{
    
    alSourcePlay(sources[sI]);//播放指定音源
    sourcesAvail[sI] = false;
}

void alManager::stop(int sI)
{
    alSourceStop(sources[sI]);
    sourcesAvail[sI] = true;
}

void alManager::pause(int sI)
{
    alSourcePause(sources[sI]);
}

bool alManager::isSrcEnd(int sI)
{
    int state;
    alGetSourcei(sources[sI], AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

int alManager::getSourceId(int sI)
{
    return sources[sI];//返回音源ID
}


/*
Copyright (c) 2018, yinan Miao;
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Beihang Univ. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

void alManager::init()
{
    device = alcOpenDevice(NULL);//打开默认设备
    if (device)
    {
        context = alcCreateContext(device, NULL);//打开默认上下文
        alcMakeContextCurrent(context);
    }
    bEAX = alIsExtensionPresent("EAX2.0");//检查EAX
    alGetError();
    alGenBuffers(MAX_BUFFER_SIZE, buffers);//分配足够多的bufferID
    alGenSources(MAX_BUFFER_SIZE, sources);//分配足够多的音源ID
    
    for (auto b : buffersAvail) b = true;
    for (auto b : sourcesAvail) b = true;
    
    //设置默认的位置
    ALfloat listenerPos[] = { 0.0,0.0,0.0 };
    ALfloat listenerVel[] = { 0.0,0.0,0.0 };
    ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    

}

void alManager::end()
{
    for (int i = 0;i < MAX_BUFFER_SIZE;i++)
    {
        alSourceStop(sources[i]);//循环关闭所有的音源
    }
    alDeleteBuffers(MAX_BUFFER_SIZE, buffers);//注销bufferID
    alDeleteSources(MAX_BUFFER_SIZE, sources);//注销音源ID
    context = alcGetCurrentContext();//获取上下文
    device = alcGetContextsDevice(context);//获取设备
    alcMakeContextCurrent(NULL);//释放内存
    alcDestroyContext(context);//关闭上下文
    alcCloseDevice(device);//关闭设备
}
