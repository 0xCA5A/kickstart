#include <iostream>
#include <typeinfo>

#include <limits>
#include "MixerAlgorithmSimpleAddWithClipping.hpp"


const MixerAlgorithmDataElement MixerAlgorithmSimpleAddWithClipping::s_mixerAlgorithmDataElement(MixerAlgorithmSimpleAddWithClipping::s_nrOfSamplesPerChunk);


void MixerAlgorithmSimpleAddWithClipping::printAlgorithmConfiguration(void) const
{
    MixerAlgorithm::printAlgorithmConfiguration();
}


/**
 * @brief mix samples as simple as possible
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmSimpleAddWithClipping::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int64_t sampleSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            //detect overflow
            if ((inputSampleBufferArray[streamIndex][chunkIndex] > 0) && (sampleSumBuffer[chunkIndex] > std::numeric_limits<int16_t>::max() - inputSampleBufferArray[streamIndex][chunkIndex]))
            {
                sampleSumBuffer[chunkIndex] = std::numeric_limits<int16_t>::max();
                continue;
            }

            //underflow
            if ((inputSampleBufferArray[streamIndex][chunkIndex] < 0) && (sampleSumBuffer[chunkIndex] < std::numeric_limits<int16_t>::min() - inputSampleBufferArray[streamIndex][chunkIndex]))
            {
                sampleSumBuffer[chunkIndex] = std::numeric_limits<int16_t>::min();
                continue;
            }

            //default case
            sampleSumBuffer[chunkIndex] += inputSampleBufferArray[streamIndex][chunkIndex];
        }

        //store values
        outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex];
    }
}

