#ifndef MIXERALGORITHMSIMPLEADDWITHSMOOTHNORMALIZATION_HPP
#define MIXERALGORITHMSIMPLEADDWITHSMOOTHNORMALIZATION_HPP


#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"


#define __NR_OF_SAMPLES_PER_CHUNK 1


/**
 * @brief implementation of fix point sample add algorithm with smooth normalization
 *
 */
class MixerAlgorithmSimpleAddWithSmoothNormalization : public MixerAlgorithm
{

public:
    MixerAlgorithmSimpleAddWithSmoothNormalization(std::string& algorithmName)
        : MixerAlgorithm(algorithmName), m_mixerAlgorithmDataElement(__NR_OF_SAMPLES_PER_CHUNK) {}
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return m_mixerAlgorithmDataElement;};

private:
    MixerAlgorithmDataElement m_mixerAlgorithmDataElement;
};

#endif