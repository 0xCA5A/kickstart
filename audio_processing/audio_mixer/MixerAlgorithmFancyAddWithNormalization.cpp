#include <iostream>
#include <typeinfo>

#include "MixerAlgorithmSimpleAddWithNormalization.hpp"

// with substitution, this formula explodes...
// http://www.wolframalpha.com/input/?i=+t%3Da%2Bx-%28a*x%29%2C+x%3Db%2By-%28b*y%29%2C+y%3Dc%2Bz-%28c*z%29%2C+z%3Dd%2Be-%28d*e%29
// http://www.wolframalpha.com/input/?i=+t%3Da%2Bw-%28a*w%29%2C+w%3Db%2Bx-%28b*x%29%2C+x%3Dc%2By-%28c*y%29%2C+y%3Dd%2Bz-%28d*z%29%2C+z%3De%2Bf-%28e*f%29


void __attribute__((optimize("O3")) MixerAlgorithmSimpleAddWithNormalization::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer))
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

