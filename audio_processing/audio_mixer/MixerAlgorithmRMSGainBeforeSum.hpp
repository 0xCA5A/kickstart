#ifndef MIXERALGORITHMRMSGAINBEFORESUM_HPP
#define MIXERALGORITHMRMSGAINBEFORESUM_HPP


#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"
#include "RMSCalculator.hpp"


#define __NR_OF_SAMPLES_PER_CHUNK 1



#define FIXME_MAX_NR_OF_CHANNELS 24
#define NR_OF_RMSCALCULATOR_TAP_ELEMENTS  1024



/**
 * @brief implementation of mixer that gains a signal depending on the RMS value before the sum
 *
 */
class MixerAlgorithmRMSGainBeforeSum : public MixerAlgorithm
{

public:
    MixerAlgorithmRMSGainBeforeSum(std::string& algorithmName);
//         : MixerAlgorithm(algorithmName), m_mixerAlgorithmDataElement(__NR_OF_SAMPLES_PER_CHUNK);
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return m_mixerAlgorithmDataElement;};

private:
    MixerAlgorithmDataElement m_mixerAlgorithmDataElement;
    RMSCalculator<int16_t, NR_OF_RMSCALCULATOR_TAP_ELEMENTS> m_RMSCalculatorArray[FIXME_MAX_NR_OF_CHANNELS];
    int16_t m_channelGainArray[FIXME_MAX_NR_OF_CHANNELS];
};

#endif