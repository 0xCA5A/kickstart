#include <iostream>
#include <typeinfo>

#include <limits>
#include "MixerAlgorithmRMSGainAfterSum.hpp"
#include "RMSCalculator.hpp"
#include "PrintMacros.hpp"





#define FIXME_FIXME_FIXME_MAX_NR_OF_CHANNELS 24



const MixerAlgorithmDataElement MixerAlgorithmRMSGainAfterSum::s_mixerAlgorithmDataElement(MixerAlgorithmRMSGainAfterSum::s_nrOfSamplesPerChunk);


MixerAlgorithmRMSGainAfterSum::MixerAlgorithmRMSGainAfterSum(std::string& algorithmName)
    : MixerAlgorithm(algorithmName)
{
}


void MixerAlgorithmRMSGainAfterSum::printAlgorithmConfiguration(void)
{
    PRINT_FORMATTED_INFO(getAlgorithmIdentifier() << " configuration:");
    PRINT_FORMATTED_INFO("number of samples per data chunk: " << s_nrOfSamplesPerChunk);
    PRINT_FORMATTED_INFO("static output sample gain: " << s_staticOutputSampleGain);
    PRINT_FORMATTED_INFO("nr of elements in output signal RMS calculator buffer: " << s_outputSignalRMSCalculatorBufferSizeInSample);
}


/**
 * @brief mix the samples depending on a calculated RMS value after the sum
 * TODO: make this generic (template)
 * TODO: the number of streams to mix has to be knows here to prepare the structures in a proper way...
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmRMSGainAfterSum::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{
    //32bit are enough
    int32_t sampleSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
    static uint32_t clippingCounter = 0;

    for (uint32_t chunkIndex = 0; chunkIndex < s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex) {
        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex) {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];
            sampleSumBuffer[chunkIndex] += originalSampleValue;
        }

        // output sample gain
        m_outputSignalRMSCalculator.putSample(&sampleSumBuffer[chunkIndex]);

        const uint32_t totalRMSValueRange = m_outputSignalRMSCalculator.getMaxRMSValue() - m_outputSignalRMSCalculator.getMinRMSValue();
        const double RMSRatio = m_outputSignalRMSCalculator.getMaxRMSValue() / m_outputSignalRMSCalculator.getRMSValue();
        double outputGain = 1.0;
        if (m_outputSignalRMSCalculator.getRMSValue() != 0) {
            outputGain = s_staticOutputSampleGain * (RMSRatio / (double)totalRMSValueRange) * (std::numeric_limits<int16_t>::max());
        }

        //store
        const int32_t outputSampleValue = outputGain * sampleSumBuffer[chunkIndex];

        //normalize if value does not fit into 16bit, upper limit
        if (outputSampleValue > std::numeric_limits<int16_t>::max()) {
            ++clippingCounter;
            PRINT_FORMATTED_LIST_ELEMENT(" !!! clipping, upper bound (clipping counter: " << clippingCounter << ") !!! ");
            outputSampleBuffer[chunkIndex] = std::numeric_limits<int16_t>::max();
            continue;
        }

        //normalize if value does not fit into 16bit, lower limit
        if (outputSampleValue < std::numeric_limits<int16_t>::min()) {
            ++clippingCounter;
            PRINT_FORMATTED_LIST_ELEMENT(" !!! clipping, lower bound (clipping counter: " << clippingCounter << ") !!! ");
            outputSampleBuffer[chunkIndex] = std::numeric_limits<int16_t>::min();
            continue;
        }

        outputSampleBuffer[chunkIndex] = (int16_t)outputSampleValue;
    }
}

