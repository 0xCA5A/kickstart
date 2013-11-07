#include <iostream>
#include <typeinfo>

#include <limits>
#include "MixerAlgorithmRMSGainAfterSum.hpp"
#include "RMSCalculator.hpp"


// TODO:
// make this generic (template)


//FIXME: the number of streams to mix has to be knows here to prepare the structures in a proper way...
// * define max channels and init structures for max channels (dirty)
// * init structures on first access (dirty)
MixerAlgorithmRMSGainAfterSum::MixerAlgorithmRMSGainAfterSum(std::string& algorithmName)
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
void __attribute__((optimize("O3"))) MixerAlgorithmRMSGainAfterSum::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    //32bit are enough
    int32_t sampleSumBuffer[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];
            sampleSumBuffer[chunkIndex] += originalSampleValue;
        }

        // output sample gain
        m_outputSignalRMSCalculator.putSample(&sampleSumBuffer[chunkIndex]);
        float outputGain = ((__MAXIMUM_OUTPUT_GAIN_VALUE_AS_FLOAT) * std::numeric_limits<int16_t>::max()) / (float)m_outputSignalRMSCalculator.getRMSValue();

        //store
        outputSampleBuffer[chunkIndex] = (int16_t)(outputGain * sampleSumBuffer[chunkIndex]);
    }
}

