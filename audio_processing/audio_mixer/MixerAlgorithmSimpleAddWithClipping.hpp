#ifndef MIXERALGORITHMSIMPLEADDWITHCLIPPING_HPP
#define MIXERALGORITHMSIMPLEADDWITHCLIPPING_HPP


#include "MixerAlgorithm.hpp"

/**
 * @brief implementation of simple sample add algorithm with hard clipping
 *
 */
class MixerAlgorithmSimpleAddWithClipping : public MixerAlgorithm
{

public:
    MixerAlgorithmSimpleAddWithClipping(std::string& algorithmName)
    : MixerAlgorithm(algorithmName) {}
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, const uint32_t nrOfSamplesPerChunk, int16_t* const outputSampleBuffer);
    virtual uint32_t getNrOfSamplesPerChunk(void);

};

#endif