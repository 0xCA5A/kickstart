#ifndef MIXERALGORITHMSIMPLEADDWITHCLIPPING_HPP
#define MIXERALGORITHMSIMPLEADDWITHCLIPPING_HPP


#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"


/**
 * @brief implementation of simple fix point sample add algorithm with hard clipping (no normalization)
 *
 */
class MixerAlgorithmSimpleAddWithClipping : public MixerAlgorithm
{

public:
    MixerAlgorithmSimpleAddWithClipping(std::string& algorithmName)
        : MixerAlgorithm(algorithmName) {}
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual void printAlgorithmConfiguration(void) const;
    virtual inline const MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return s_mixerAlgorithmDataElement;};

private:
    static const MixerAlgorithmDataElement s_mixerAlgorithmDataElement;
    static const uint32_t s_nrOfSamplesPerChunk = 1;
};

#endif