#ifndef MIXERALGORITHM_HPP
#define MIXERALGORITHM_HPP

#include <string>
#include <stdint.h>
#include "MixerAlgorithmDataElement.hpp"


//TODO: add generic to have a variable sample size

class MixerAlgorithm
{
public:
    MixerAlgorithm(std::string& algorithmName) : m_algorithmIdentifier(algorithmName) {};
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer) = 0;
    virtual MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) = 0;
    std::string getAlgorithmIdentifier() const {return m_algorithmIdentifier;};

private:
    MixerAlgorithm(const MixerAlgorithm&);
    MixerAlgorithm& operator=(const MixerAlgorithm&);

private:
    std::string m_algorithmIdentifier;
};

#endif


