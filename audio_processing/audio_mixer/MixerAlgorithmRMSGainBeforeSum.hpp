#ifndef MIXERALGORITHMRMSGAINBEFORESUM_HPP
#define MIXERALGORITHMRMSGAINBEFORESUM_HPP


#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"
#include "RMSCalculator.hpp"


class MixerAlgorithmRMSGainBeforeSum : public MixerAlgorithm
{

public:
    MixerAlgorithmRMSGainBeforeSum(std::string& algorithmName, uint32_t nrOfStreams = 24);
    ~MixerAlgorithmRMSGainBeforeSum(void);
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual void printAlgorithmConfiguration(void) const;
    virtual inline const MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return s_mixerAlgorithmDataElement;};

private:
    MixerAlgorithmRMSGainBeforeSum(const MixerAlgorithmRMSGainBeforeSum&);
    MixerAlgorithmRMSGainBeforeSum& operator=(const MixerAlgorithmRMSGainBeforeSum&);

private:
    static const MixerAlgorithmDataElement s_mixerAlgorithmDataElement;
    // 16000 samples per second @ 16khz sampling rate
    // 1ms ~ 16 samples - 64ms ~ 1024 samples
    // 1ms ~ 16 samples - 8ms ~ 128 samples
    static const uint32_t s_inputSignalRMSCalculatorBufferSizeInSample = 1024;
    static const float s_staticOutputSampleGainFactor = 0.3;
    static const uint32_t s_nrOfSamplesPerChunk = 1;
    RMSCalculator<int16_t, s_inputSignalRMSCalculatorBufferSizeInSample>* m_inputSignalRMSCalculatorArray;
};

#endif