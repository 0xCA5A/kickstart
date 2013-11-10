#include <iostream>
#include <typeinfo>
#include <limits>
#include <strings.h>

#include "MixerAlgorithmFancyAddWithNormalization.hpp"


const MixerAlgorithmDataElement MixerAlgorithmFancyAddWithNormalization::s_mixerAlgorithmDataElement(MixerAlgorithmFancyAddWithNormalization::s_nrOfSamplesPerChunk);


void MixerAlgorithmFancyAddWithNormalization::printAlgorithmConfiguration(void) const
{
    MixerAlgorithm::printAlgorithmConfiguration();
    PRINT_FORMATTED_INFO("number of samples per data chunk: " << s_nrOfSamplesPerChunk);
}


/**
 * @brief use smoothing mix algorithm formula from viktor toth
 * S(t) = A(t) + B(t) - A(t) * B(t)
 * http://club15cc.com/code/ios/mixing-audio-without-clipping-limiters-etc
 * http://atastypixel.com/blog/how-to-mix-audio-samples-properly-on-ios/
 *
 * formula works fine for two signals.
 * if i try to include more signals using substitution, the formula explodes to something in this form:
 * http://www.wolframalpha.com/input/?i=+t%3Da%2Bx-%28a*x%29%2C+x%3Db%2By-%28b*y%29%2C+y%3Dc%2Bz-%28c*z%29%2C+z%3Dd%2Be-%28d*e%29
 * http://www.wolframalpha.com/input/?i=+t%3Da%2Bw-%28a*w%29%2C+w%3Db%2Bx-%28b*x%29%2C+x%3Dc%2By-%28c*y%29%2C+y%3Dd%2Bz-%28d*z%29%2C+z%3De%2Bf-%28e*f%29
 *
 * it was not possible for me to find a closed formula. the permutations for each case (different numbers of input signals) can be prepared once,
 * the per output sample calculation effort seems to be too high in the end (in my point of view). there are better solutions...
 *
 * a cascading of multiple two channel mixers seems as well not to be the right way to do this. the resulting 'wave' looks strange...
 * this strategy delivers no proper result...
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmFancyAddWithNormalization::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{
    for (uint32_t chunkIndex = 0; chunkIndex < s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex) {
        outputSampleBuffer[chunkIndex] = 0;
        int16_t cascadingMixResultSample = 0;

        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex) {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];

            addSamplesWithNoClipping(originalSampleValue, cascadingMixResultSample, cascadingMixResultSample);
        }
        outputSampleBuffer[chunkIndex] = cascadingMixResultSample;
    }
}


inline void MixerAlgorithmFancyAddWithNormalization::addSamplesWithNoClipping(const int16_t&  sampleA, const int16_t&  sampleB, int16_t&  outputSample)
{
    //small values
    if (sampleA < 0 && sampleB < 0) {
        outputSample = sampleA + sampleB - ((sampleA * sampleB) / std::numeric_limits<int16_t>::min());
        return;
    }

    //big values
    if (sampleA > 0 && sampleB > 0) {
        outputSample = sampleA + sampleB - ((sampleA * sampleB) / std::numeric_limits<int16_t>::max());
        return;
    }

    //default case
    outputSample = sampleA + sampleB;
}
