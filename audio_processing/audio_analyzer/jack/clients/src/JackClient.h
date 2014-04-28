#ifndef _JACKCLIENT_H_
#define _JACKCLIENT_H_

#include <jack/jack.h>
#include <string>

#include "UnCopyable.h"
// #include "RingBuffer.h"
#include <jack/ringbuffer.h>


class JackClient : UnCopyable
{
    public:
        bool setup(volatile bool* p_runFlag, uint32_t nrOfFramesInRingBuffer);
        bool activateClientAndAutoConnectPorts();
        static void shutdown();
        jack_default_audio_sample_t getAudioSampleAVG() {
            return m_lastAudioSample;
        }
        uint32_t getNumberOfFramesPerPeriod() {
            return (m_p_jackClient) ? jack_get_buffer_size(m_p_jackClient) : 0;
        }
        uint32_t getSampleRate() {
            return (m_p_jackClient) ? jack_get_sample_rate(m_p_jackClient) : 0;
        }
        jack_ringbuffer_t* getRingBuffer() {
            return m_p_ringBuffer;
        }

    private:
        bool registerPorts();
        bool autoConnectPortsToMPG123();
        static int processJackCallback(jack_nframes_t nframes, void* arg);
        static void shutdownJackCallback(void *arg);
        static void connectionJackCallback(jack_port_id_t a, jack_port_id_t b, int connect, void *arg);

    private:
        jack_ringbuffer_t* m_p_ringBuffer;
        static volatile bool* m_p_runFlag;
        static jack_client_t* m_p_jackClient;
        static jack_port_t* m_p_leftChannelInputPort;
        static jack_port_t* m_p_rightChannelInputPort;
        static jack_default_audio_sample_t m_lastAudioSample;

        static const std::string s_jackClientLeftChannelInputPortName;
        static const std::string s_jackClientRightChannelInputPortName;
        static const std::string s_jackClientName;
        static const std::string s_portNameToConnectLeftChannelRegex;
        static const std::string s_portNameToConnectRightChannelRegex;
};


#endif