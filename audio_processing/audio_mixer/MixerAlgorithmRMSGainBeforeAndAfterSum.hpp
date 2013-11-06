#ifndef MIXERALGORITHMRMSGAINBEFOREANDAFTERSUM_HPP
#define MIXERALGORITHMRMSGAINBEFOREANDAFTERSUM_HPP

#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"
#include "RMSCalculator.hpp"


#define __NR_OF_SAMPLES_PER_CHUNK 1

// 16000 samples per second @ 16khz sampling rate
// 1ms ~ 16 samples - 64ms ~ 1024 samples
#define __NR_OF_INPUT_SIGNAL_RMSCALCULATOR_BUFFER_SIZE_IN_SAMPLES  1024
#define __NR_OF_OUTPUT_SIGNAL_RMSCALCULATOR_BUFFER_SIZE_IN_SAMPLES  1024

#define __MAXIMUM_OUTPUT_GAIN_VALUE_AS_FLOAT 0.5

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
    virtual MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return m_mixerAlgorithmDataElement;};

private:
    MixerAlgorithmRMSGainBeforeAndAfterSum(const MixerAlgorithmRMSGainBeforeAndAfterSum&);
    MixerAlgorithmRMSGainBeforeAndAfterSum& operator=(const MixerAlgorithmRMSGainBeforeAndAfterSum&);

private:
    MixerAlgorithmDataElement m_mixerAlgorithmDataElement;
    RMSCalculator<int16_t, __NR_OF_INPUT_SIGNAL_RMSCALCULATOR_BUFFER_SIZE_IN_SAMPLES> m_inputSignalRMSCalculatorArray[FIXME_FIXME_FIXME_MAX_NR_OF_CHANNELS];
    RMSCalculator<int16_t, __NR_OF_OUTPUT_SIGNAL_RMSCALCULATOR_BUFFER_SIZE_IN_SAMPLES> m_outputSignalRMSCalculator;
};

#endif