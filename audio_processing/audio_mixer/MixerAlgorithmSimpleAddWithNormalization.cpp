#include <iostream>
#include <typeinfo>


#include "MixerAlgorithmSimpleAddWithNormalization.hpp"


const MixerAlgorithmDataElement MixerAlgorithmSimpleAddWithNormalization::s_mixerAlgorithmDataElement(MixerAlgorithmSimpleAddWithNormalization::s_nrOfSamplesPerChunk);


void MixerAlgorithmSimpleAddWithNormalization::printAlgorithmConfiguration(void) const
{
    MixerAlgorithm::printAlgorithmConfiguration();
}


/**
 * @brief mix samples as simple as possible with normalization, no clipping
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmSimpleAddWithNormalization::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int64_t sampleSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            sampleSumBuffer[chunkIndex] += inputSampleBufferArray[streamIndex][chunkIndex];
        }

        //normalize and store
        outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] / nrOfStreams;
    }
}
