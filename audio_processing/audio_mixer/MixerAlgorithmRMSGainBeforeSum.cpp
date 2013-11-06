#include <iostream>
#include <typeinfo>

#include <limits>
#include "MixerAlgorithmRMSGainBeforeSum.hpp"
#include "RMSCalculator.hpp"


// TODO:
// make this generic (template)



MixerAlgorithmRMSGainBeforeSum::MixerAlgorithmRMSGainBeforeSum(std::string& algorithmName)
    : MixerAlgorithm(algorithmName), m_mixerAlgorithmDataElement(__NR_OF_SAMPLES_PER_CHUNK)
{
    //FIXME: the number of streams to mix has to be knows here to prepare the structures in a proper way...
    // * define max channels and init structures for max channels (dirty)
    // * init structures on first access (dirty)

//     RMSCalculator<uint16_t, NR_OF_RMSCALCULATOR_TAP_ELEMENTS> m_RMSCalculatorArray[FIXME_MAX_NR_OF_CHANNELS];

    //flatten channel gain parameter
    for (uint32_t i = 0; i < FIXME_MAX_NR_OF_CHANNELS; i++) {
        m_channelGainArray[i] = 0;
    }

}


/**
 * @brief mix the samples depending on a calculated rms value
 * NOTE: 
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmRMSGainBeforeSum::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int32_t sampleSumBuffer[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
        uint32_t rmsSum = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];
            m_RMSCalculatorArray[streamIndex].putSample(&originalSampleValue);
            rmsSum += m_RMSCalculatorArray[streamIndex].getRMSValue();
        }

        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];

            float channelGain;
            if (rmsSum == 0) {
                channelGain = 1;
            } else {
                channelGain = (float)m_RMSCalculatorArray[streamIndex].getRMSValue() / rmsSum;
            }

            sampleSumBuffer[chunkIndex] += channelGain * originalSampleValue;
        }

        //store
        outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex];
    }
}

