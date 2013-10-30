#ifndef MIXERALGORITHM_HPP
#define MIXERALGORITHM_HPP

#include <stdint.h>


class MixerAlgorithm
{

public:
    MixerAlgorithm(std::string& algorithmName) : m_algorithmName(algorithmName) {};
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, const uint32_t nrOfSamplesPerChunk, int16_t* const outputSampleBuffer) = 0;
    virtual uint32_t getNrOfSamplesPerChunk(void) = 0;
    std::string getAlgorithmName() {return m_algorithmName;};

private:
    std::string m_algorithmName;

};

#endif


