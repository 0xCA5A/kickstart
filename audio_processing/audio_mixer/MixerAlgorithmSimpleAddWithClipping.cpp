#include <iostream>
#include <typeinfo>

#include "MixerAlgorithmSimpleAddWithClipping.hpp"
#include "../../../maglev/hwpacks/ESU-II/linux-3.2.0-psp04.06.00.08.sdk/scripts/kconfig/expr.h"

void MixerAlgorithmSimpleAddWithClipping::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, const uint32_t nrOfSamplesPerChunk, int16_t* const outputSampleBuffer)
{
    int64_t sampleSumBuffer[nrOfSamplesPerChunk];

    for (uint32_t chunkIndex = 0; chunkIndex < nrOfSamplesPerChunk; ++chunkIndex)
    {
        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            sampleSumBuffer[chunkIndex] += inputSampleBufferArray[streamIndex][chunkIndex];
        }

        //normalize and store
        outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex] / nrOfStreams;
    }

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


}

uint32_t MixerAlgorithmSimpleAddWithClipping::getNrOfSamplesPerChunk(void)
{
    //algorithm specific stuff, how to design this in the right way?
    return 1;
}