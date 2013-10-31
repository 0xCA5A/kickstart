#include <iostream>
#include <typeinfo>

#include "MixerAlgorithmSimpleAddWithNormalization.hpp"


void __attribute__((optimize("O3"))) MixerAlgorithmSimpleAddWithNormalization::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int64_t sampleSumBuffer[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
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

