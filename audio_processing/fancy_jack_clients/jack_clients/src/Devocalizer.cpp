include "AudioAnalyzer.h"
#include "JackClient.h"

#include <jack/ringbuffer.h>

#include "fftw3.h"
#include <complex.h>
#include <math.h>

#include "debug.h"

#include <jack/jack.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>



// BAD STUFF!
volatile bool g_runFlag = true;









void signalCallbackHandler(int signum)
{
    DEBUG_PRINT("caught signal %d, exit now\n", signum);
    g_runFlag = false;
}




// NOTE(sam): unchecked quick and dirty implementation on my way to hawaii
int Devocalizer::processJackCallback(jack_nframes_t nFrames, void* arg)
{
    JackClient* jackClient = static_cast<JackClient*>(arg);

    jack_default_audio_sample_t* p_leftChannelInputAudioSamples = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(m_p_leftChannelInputPort, nFrames));
    jack_default_audio_sample_t* p_rightChannelInputAudioSamples = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(m_p_rightChannelInputPort, nFrames));

    jack_default_audio_sample_t* p_leftChannelOutputAudioSamples = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(m_p_leftChannelOutputPort, nFrames));
    jack_default_audio_sample_t* p_rightChannelOutputAudioSamples = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(m_p_rightChannelOutputPort, nFrames));



    // http://makezine.com/projects/make-39/song-devocalizer/
    // use opposite channel as the new ground
    // each stereo channel is added to a inverse copy of the opposite channel

    // process audio samples
    for (uint32_t i = 0; i < nFrames; i++) {

        // add an inverse copy of the right input channel to the left channel samples
        p_leftChannelOutputAudioSamples[i] = p_leftChannelInputAudioSamples[i] + (-1 * p_rightChannelInputAudioSamples[i]);

        // add an inverse copy of the left input channel to the right channel samples
        p_rightChannelOutputAudioSamples[i] = p_rightChannelInputAudioSamples[i] + (-1 * p_leftChannelInputAudioSamples[i]);

    }

    return 0;
}



int main()
{

//     mpg123HMIEqualizer myMpg123HMIEqualizer;

    signal(SIGINT, signalCallbackHandler);
    signal(SIGTERM, signalCallbackHandler);

    EsuHMI esuHMI;


    JackClient jackClient;
    const uint32_t numberOfFramesInRingbuffer = 2;
    if (!jackClient.setup(&g_runFlag, numberOfFramesInRingbuffer)) {
        return -1;
    }

    jack_ringbuffer_t* p_jackRingBuffer = jackClient.getRingBuffer();

    if (!jackClient.activateClientAndAutoConnectPorts()) {
        jackClient.shutdown();
        return -2;
    }


    esuHMI.threadStart();


    uint8_t redLEDValue = 1;
    uint8_t greenLEDValue = 1;
    uint8_t blueLEDValue = 1;

    const uint32_t nrOfSamplesPerPeriod = jackClient.getNumberOfFramesPerPeriod();
    jack_default_audio_sample_t sampleBuffer[nrOfSamplesPerPeriod];

    while(g_runFlag) {

        const uint32_t readSizeInByte = nrOfSamplesPerPeriod * sizeof(jack_default_audio_sample_t);
        if (jack_ringbuffer_read_space(p_jackRingBuffer) >= readSizeInByte) {
            jack_ringbuffer_read(p_jackRingBuffer, reinterpret_cast<char*>(sampleBuffer), readSizeInByte);
        } else {
            usleep(1000);
            continue;
        }


//         updateLEDValuesFFT(sampleBuffer, nrOfSamplesPerPeriod, &redLEDValue, &greenLEDValue, &blueLEDValue);
//         updateLEDValuesRMS(sampleBuffer, nrOfSamplesPerPeriod, &redLEDValue, &greenLEDValue, &blueLEDValue);
    updateLEDValuesFFT2(sampleBuffer, nrOfSamplesPerPeriod, &redLEDValue, &greenLEDValue, &blueLEDValue);

        esuHMI.setLEDPattern(assembleReadyHMIPattern(redLEDValue, greenLEDValue, blueLEDValue));



    }

    DEBUG_PRINT("stop communicating objects");
    jackClient.shutdown();

    esuHMI.threadStop();

    // 16 ms ~ 60hz
    usleep(15000);

    DEBUG_PRINT("exit main gracefully");
    return 0;
}

