#ifndef MIXERALGORITHMRMSGAINBEFOREANDAFTERSUM_HPP
#define MIXERALGORITHMRMSGAINBEFOREANDAFTERSUM_HPP

#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"
#include "RMSCalculator.hpp"



#define FIXME_FIXME_FIXME_MAX_NR_OF_CHANNELS 24



/**
 * @brief implementation of mixer that gains a signal depending on the RMS value before the sum
 *
 */
class MixerAlgorithmRMSGainBeforeAndAfterSum : public MixerAlgorithm
{
public:
    MixerAlgorithmRMSGainBeforeAndAfterSum(std::string& algorithmName);
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual void printAlgorithmConfiguration(void) const;
    virtual inline const MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return s_mixerAlgorithmDataElement;};

private:
    MixerAlgorithmRMSGainBeforeAndAfterSum(const MixerAlgorithmRMSGainBeforeAndAfterSum&);
    MixerAlgorithmRMSGainBeforeAndAfterSum& operator=(const MixerAlgorithmRMSGainBeforeAndAfterSum&);

private:
    static const MixerAlgorithmDataElement s_mixerAlgorithmDataElement;
    static const uint32_t s_nrOfSamplesPerChunk = 1;
    // 16000 samples per second @ 16khz sampling rate
    // 1ms ~ 16 samples - 64ms ~ 1024 samples
    static const uint32_t s_inputSignalRMSCalculatorBufferSizeInSample = 1024;
    static const uint32_t s_outputSignalRMSCalculatorBufferSizeInSample = 2048;
    static const float s_staticOutputSampleGain = 0.3;

    RMSCalculator<int16_t, s_inputSignalRMSCalculatorBufferSizeInSample> m_inputSignalRMSCalculatorArray[FIXME_FIXME_FIXME_MAX_NR_OF_CHANNELS];
    RMSCalculator<int16_t, s_outputSignalRMSCalculatorBufferSizeInSample> m_outputSignalRMSCalculator;
};

#endif