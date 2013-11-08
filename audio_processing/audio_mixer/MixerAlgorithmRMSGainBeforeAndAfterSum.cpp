#include <iostream>
#include <typeinfo>

#include <limits>
#include "MixerAlgorithmRMSGainBeforeAndAfterSum.hpp"
#include "RMSCalculator.hpp"



const MixerAlgorithmDataElement MixerAlgorithmRMSGainBeforeAndAfterSum::s_mixerAlgorithmDataElement(MixerAlgorithmRMSGainBeforeAndAfterSum::s_nrOfSamplesPerChunk);



// TODO:
// make this generic (template)


//FIXME: the number of streams to mix has to be knows here to prepare the structures in a proper way...
// * define max channels and init structures for max channels (dirty)
// * init structures on first access (dirty)
MixerAlgorithmRMSGainBeforeAndAfterSum::MixerAlgorithmRMSGainBeforeAndAfterSum(std::string& algorithmName)
    : MixerAlgorithm(algorithmName)
{
}


void MixerAlgorithmRMSGainBeforeAndAfterSum::printAlgorithmConfiguration(void) const
{
    MixerAlgorithm::printAlgorithmConfiguration();
}


/**
 * @brief mix the samples depending on a calculated rms value
 * NOTE: 
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmRMSGainBeforeAndAfterSum::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int16_t sampleSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
    static uint32_t clippingCounter = 0;

    for (uint32_t chunkIndex = 0; chunkIndex < s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk(); ++chunkIndex)
    {
        uint32_t rmsSum = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];
            m_inputSignalRMSCalculatorArray[streamIndex].putSample(&originalSampleValue);
            rmsSum += m_inputSignalRMSCalculatorArray[streamIndex].getRMSValue();
        }

        sampleSumBuffer[chunkIndex] = 0;
        for (uint32_t streamIndex = 0; streamIndex < nrOfStreams; ++streamIndex)
        {
            const int16_t originalSampleValue = inputSampleBufferArray[streamIndex][chunkIndex];

            float channelGain;
            if (rmsSum == 0) {
                channelGain = 1;
            } else {
                channelGain = (float)m_inputSignalRMSCalculatorArray[streamIndex].getRMSValue() / rmsSum;
            }

            sampleSumBuffer[chunkIndex] += channelGain * originalSampleValue;
        }

        // output sample gain
        m_outputSignalRMSCalculator.putSample(&sampleSumBuffer[chunkIndex]);

        const uint32_t totalRMSValueRange = m_outputSignalRMSCalculator.getMaxRMSValue() - m_outputSignalRMSCalculator.getMinRMSValue();
        const double RMSRatio = m_outputSignalRMSCalculator.getMaxRMSValue() / m_outputSignalRMSCalculator.getRMSValue();
        double outputGain = 1.0;
        if (m_outputSignalRMSCalculator.getRMSValue() != 0) {
            outputGain = s_staticOutputSampleGain * (RMSRatio / (double)totalRMSValueRange) * (double)(std::numeric_limits<int16_t>::max());
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

