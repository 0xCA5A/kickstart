#ifndef MIXERALGORITHMSIMPLEADDWITHNORMALIZATION_HPP
#define MIXERALGORITHMSIMPLEADDWITHNORMALIZATION_HPP


#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"


#define __NR_OF_SAMPLES_PER_CHUNK 1


/**
 * @brief implementation of simple fix point sample add algorithm with hard clipping
 *
 */
class MixerAlgorithmSimpleAddWithNormalization : public MixerAlgorithm
{

public:
    MixerAlgorithmSimpleAddWithNormalization(std::string& algorithmName)
    : MixerAlgorithm(algorithmName), m_mixerAlgorithmDataElement(__NR_OF_SAMPLES_PER_CHUNK) {}
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return m_mixerAlgorithmDataElement;};

private:
    MixerAlgorithmSimpleAddWithNormalization(const MixerAlgorithmSimpleAddWithNormalization&);
    MixerAlgorithmSimpleAddWithNormalization& operator=(const MixerAlgorithmSimpleAddWithNormalization&);

private:
    MixerAlgorithmDataElement m_mixerAlgorithmDataElement;
};

#endif