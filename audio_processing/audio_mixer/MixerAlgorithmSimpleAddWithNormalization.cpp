#include <iostream>
#include <typeinfo>

#include "MixerAlgorithmSimpleAddWithNormalization.hpp"


// short sample1 = ...;
// short sample2 = ...;
// float samplef1 = sample1 / 32768.0f;
// float samplef2 = sample2 / 32768.0f;
// float mixed = samplef1 + sample2f;
// // reduce the volume a bit:
// mixed *= 0.8;
// // hard clipping
// if (mixed > 1.0f) mixed = 1.0f;
// if (mixed < -1.0f) mixed = -1.0f;
// short outputSample = (short)(mixed * 32768.0f)

// source: http://de.softuses.com/42658


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

