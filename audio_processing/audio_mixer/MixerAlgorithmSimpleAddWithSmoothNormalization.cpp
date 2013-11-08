#include <iostream>
#include <typeinfo>
#include <limits>
#include <strings.h>

#include "MixerAlgorithmSimpleAddWithSmoothNormalization.hpp"


// Quick and Dirty Maths
// http://club15cc.com/code/ios/mixing-audio-without-clipping-limiters-etc


const MixerAlgorithmDataElement MixerAlgorithmSimpleAddWithSmoothNormalization::s_mixerAlgorithmDataElement(MixerAlgorithmSimpleAddWithSmoothNormalization::s_nrOfSamplesPerChunk);




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

    int64_t sampleSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
    int64_t sampleProductBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
    int64_t coefficientSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

//     int32_t tempResult[m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
//     bzero(tempResult, m_mixerAlgorithmDataElement.getNrOfSamplesPerChunk() * (sizeof(int16_t)));
    
    for (uint32_t chunkIndex = 0; chunkIndex < s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
//         sampleSumBuffer[chunkIndex] = 0;
//         sampleProductBuffer[chunkIndex] = 1;
//         coefficientSumBuffer[chunkIndex] = 0;
// 
//         uint32_t minimalCount = 0;
//         uint32_t maximalCount = 0;
// 
        int32_t tempMixSampleResult = 0;
// 
//         for (uint32_t streamIndex = 1; streamIndex < nrOfStreams; ++streamIndex)
//         {
// 
//             //minimal samples
//             if (tempMixSampleResult < 0 && inputSampleBufferArray[streamIndex][chunkIndex] < 0)
//             {
//                 tempMixSampleResult = tempMixSampleResult + inputSampleBufferArray[streamIndex][chunkIndex] - (tempMixSampleResult * inputSampleBufferArray[streamIndex][chunkIndex]) / std::numeric_limits<int16_t>::min();
//                 continue;
//             }
// 
//             //maximal samples
//             if (tempMixSampleResult > 0 && inputSampleBufferArray[streamIndex][chunkIndex] > 0)
//             {
//                 tempMixSampleResult = tempMixSampleResult + inputSampleBufferArray[streamIndex][chunkIndex] - (tempMixSampleResult * inputSampleBufferArray[streamIndex][chunkIndex]) / std::numeric_limits<int16_t>::max();
//                 continue;
//             }
// 
//             //default samples
//             tempMixSampleResult = tempMixSampleResult + inputSampleBufferArray[streamIndex][chunkIndex] - (tempMixSampleResult * inputSampleBufferArray[streamIndex][chunkIndex]);
// 
// //             sampleSumBuffer[chunkIndex] += inputSampleBufferArray[streamIndex][chunkIndex];
// //             sampleProductBuffer[chunkIndex] *= inputSampleBufferArray[streamIndex][chunkIndex];
// 
// //             //sum up coefficients, used in special cases 'below zero', 'above zero'
// //             for (uint32_t i = streamIndex + 1; i < nrOfStreams; i++)
// //             {
// //                 coefficientSumBuffer[chunkIndex] += inputSampleBufferArray[streamIndex][chunkIndex] * inputSampleBufferArray[i][chunkIndex];
// //             }
// // 
// //             //check if below zero
// //             if (inputSampleBufferArray[streamIndex][chunkIndex] < 0)
// //             {
// //                 minimalCount++;
// //             }
// // 
// //             //check if above zero
// //             if (inputSampleBufferArray[streamIndex][chunkIndex] > 0)
// //             {
// //                 maximalCount++;
// //             }
            

        const int16_t a = inputSampleBufferArray[0][chunkIndex];
        const int16_t b = inputSampleBufferArray[1][chunkIndex];
        const int16_t c = inputSampleBufferArray[2][chunkIndex];

    int minusCounter = 0;
    for (int i = 0; i < 3; i++)
    {
        if (inputSampleBufferArray[i][chunkIndex] < 0)
        {
            minusCounter++;
        }
    }

    int plusCounter = 0;
    for (int i = 0; i < 3; i++)
    {
        if (inputSampleBufferArray[i][chunkIndex] > 0)
        {
            plusCounter++;
        }
    }



//     std::cout << "minusCounter: " << minusCounter << std::endl;
//     std::cout << "plusCounter: " << plusCounter << std::endl;
    if (minusCounter >= 2)
    {
//         std::cout << "minus" << std::endl;
        tempMixSampleResult = a + b + c + ((a * b * c - a*b - a*c - b*c) / std::numeric_limits<int16_t>::min());
        outputSampleBuffer[chunkIndex] = tempMixSampleResult;
        continue;
    }

    if (plusCounter >= 2)
    {
//         std::cout << "plus" << std::endl;
        tempMixSampleResult = a + b + c + ((a * b * c - a*b - a*c - b*c) / std::numeric_limits<int16_t>::max());
        outputSampleBuffer[chunkIndex] = tempMixSampleResult;
        continue;
    }

//     std::cout << "default" << std::endl;
    tempMixSampleResult = a + b + c + (a * b * c - a*b - a*c - b*c);
    outputSampleBuffer[chunkIndex] = tempMixSampleResult;

// //         all values are minimal
//         if (minimalCount == nrOfStreams)
//         {
// //             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::min());
//             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::min());
//             continue;
//         }
// 
//         //all values are maximal
//         if (maximalCount == nrOfStreams)
//         {
// //             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::max());
//             outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] - (sampleProductBuffer[chunkIndex] / std::numeric_limits<int16_t>::max());
//             continue;
//         }


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


    }
}

