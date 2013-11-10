#include <iostream>
#include <typeinfo>


#include <limits>
#include "MixerAlgorithmRMSGainBeforeSum.hpp"
#include "RMSCalculator.hpp"


const MixerAlgorithmDataElement MixerAlgorithmRMSGainBeforeSum::s_mixerAlgorithmDataElement(MixerAlgorithmRMSGainBeforeSum::s_nrOfSamplesPerChunk);



MixerAlgorithmRMSGainBeforeSum::MixerAlgorithmRMSGainBeforeSum(std::string& algorithmName, const uint32_t nrOfStreams)
    : MixerAlgorithm(algorithmName)
{
    m_inputSignalRMSCalculatorArray = new RMSCalculator<int16_t, s_inputSignalRMSCalculatorBufferSizeInSample>[nrOfStreams];
}


MixerAlgorithmRMSGainBeforeSum::~MixerAlgorithmRMSGainBeforeSum(void)
{
    delete[] m_inputSignalRMSCalculatorArray;
}


void MixerAlgorithmRMSGainBeforeSum::printAlgorithmConfiguration(void) const
{
    MixerAlgorithm::printAlgorithmConfiguration();
    PRINT_FORMATTED_INFO("number of samples per data chunk: " << s_nrOfSamplesPerChunk);
    PRINT_FORMATTED_INFO("static output sample gain factor: " << s_staticOutputSampleGainFactor);
    PRINT_FORMATTED_INFO("nr of elements in input signal RMS calculator buffer: " << s_inputSignalRMSCalculatorBufferSizeInSample);
}


/**
 * @brief mix the samples depending on a calculated rms value
 * rms value is calculated for each input stream, stream gain depends on the stream rms value
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmRMSGainBeforeSum::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{
    int16_t sampleSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];

    for (uint32_t chunkIndex = 0; chunkIndex < s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex) {
        uint32_t rmsSum = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex) {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];
            m_inputSignalRMSCalculatorArray[streamIndex].putSample(&originalSampleValue);
            rmsSum += m_inputSignalRMSCalculatorArray[streamIndex].getRMSValue();
        }

        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex) {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];

            float channelGain;
            if (rmsSum == 0) {
                channelGain = 1;
            } else {
                channelGain = (float)m_inputSignalRMSCalculatorArray[streamIndex].getRMSValue() / rmsSum;
            }

            sampleSumBuffer[chunkIndex] += channelGain * originalSampleValue;
        }

        //store
        outputSampleBuffer[chunkIndex] = sampleSumBuffer[chunkIndex];
    }
}

