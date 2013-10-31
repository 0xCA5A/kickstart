#include <iostream>
#include <typeinfo>
#include <limits>

#include "MixerAlgorithmSimpleAddWithSmoothNormalization.hpp"


// Solution 1: Quick and Dirty Maths
// http://club15cc.com/code/ios/mixing-audio-without-clipping-limiters-etc




/**
 * @brief use smoothing mix algorithm formula
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmSimpleAddWithSmoothNormalization::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int64_t sampleSumBuffer[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
    int64_t sampleProductBuffer[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
    int64_t coefficientSumBuffer[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
        sampleSumBuffer[chunkIndex] = 0;
        sampleProductBuffer[chunkIndex] = 1;
        coefficientSumBuffer[chunkIndex] = 0;

        uint32_t minimalCount = 0;
        uint32_t maximalCount = 0;


        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            sampleSumBuffer[chunkIndex] += inputSampleBufferArray[streamIndex][chunkIndex];
            sampleProductBuffer[chunkIndex] *= inputSampleBufferArray[streamIndex][chunkIndex];

            //sum up coefficients, used in special cases 'below zero', 'above zero'
            for (uint32_t i = streamIndex + 1; i < nrOfStreams; i++)
            {
                coefficientSumBuffer[chunkIndex] += inputSampleBufferArray[streamIndex][chunkIndex] * inputSampleBufferArray[i][chunkIndex];
            }

            //check if below zero
            if (inputSampleBufferArray[streamIndex][chunkIndex] < 0)
            {
                minimalCount++;
            }

            //check if above zero
            if (inputSampleBufferArray[streamIndex][chunkIndex] > 0)
            {
                maximalCount++;
            }
        }

//         all values are minimal
        if (minimalCount == nrOfStreams)
        {
//             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::min());
            outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::min());
            continue;
        }

        //all values are maximal
        if (maximalCount == nrOfStreams)
        {
//             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::max());
            outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::max());
            continue;
        }


//         if (minimalCount == nrOfStreams)
//         {
//             //             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::min());
//             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - ((coefficientSumBuffer[chunkIndex] + sampleProductBuffer[chunkIndex]) / std::numeric_limits<int16_t>::min());
//             continue;
//         }
// 
//         if (maximalCount == nrOfStreams)
//         {
//             //             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::max());
//             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - ((coefficientSumBuffer[chunkIndex] + sampleProductBuffer[chunkIndex]) / std::numeric_limits<int16_t>::max());
//             continue;
//         }
//

        //normalize and store
        outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex];
    }
}

