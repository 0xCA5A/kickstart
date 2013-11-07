#include <iostream>
#include <typeinfo>

#include <limits>
#include "MixerAlgorithmRMSGainBeforeAndAfterSum.hpp"
#include "RMSCalculator.hpp"


// TODO:
// make this generic (template)


//FIXME: the number of streams to mix has to be knows here to prepare the structures in a proper way...
// * define max channels and init structures for max channels (dirty)
// * init structures on first access (dirty)
MixerAlgorithmRMSGainBeforeAndAfterSum::MixerAlgorithmRMSGainBeforeAndAfterSum(std::string& algorithmName)
    : MixerAlgorithm(algorithmName), m_mixerAlgorithmDataElement(__NR_OF_SAMPLES_PER_CHUNK)
{
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
void __attribute__((optimize("O3"))) MixerAlgorithmRMSGainBeforeAndAfterSum::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int16_t sampleSumBuffer[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
        uint32_t rmsSum = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];
            m_inputSignalRMSCalculatorArray[streamIndex].putSample(&originalSampleValue);
            rmsSum += m_inputSignalRMSCalculatorArray[streamIndex].getRMSValue();
        }

        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];

            float channelGain;
            if (rmsSum == 0) {
                channelGain = 1;
            } else {
                channelGain = (float)m_inputSignalRMSCalculatorArray[streamIndex].getRMSValue() / rmsSum;
            }

            sampleSumBuffer[chunkIndex] += channelGain * originalSampleValue;
        }

        // output sample gain
        m_outputSignalRMSCalculator.putSample(&sampleSumBuffer[chunkIndex]);
        float outputGain = (__MAXIMUM_OUTPUT_GAIN_VALUE_AS_FLOAT) / ((float)m_outputSignalRMSCalculator.getRMSValue() / (float)std::numeric_limits<int16_t>::max());

        //store
        outputSampleBuffer[chunkIndex] = (int16_t)(outputGain * sampleSumBuffer[chunkIndex]);
    }
}

