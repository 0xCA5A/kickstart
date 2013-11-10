#include <iostream>
#include <typeinfo>


#include <limits>
#include "MixerAlgorithmRMSGainBeforeAndAfterSum.hpp"
#include "RMSCalculator.hpp"


const MixerAlgorithmDataElement MixerAlgorithmRMSGainBeforeAndAfterSum::s_mixerAlgorithmDataElement(MixerAlgorithmRMSGainBeforeAndAfterSum::s_nrOfSamplesPerChunk);


MixerAlgorithmRMSGainBeforeAndAfterSum::MixerAlgorithmRMSGainBeforeAndAfterSum(std::string& algorithmName, const uint32_t nrOfStreams)
    : MixerAlgorithm(algorithmName)
{
    m_inputSignalRMSCalculatorArray = new RMSCalculator<int16_t, s_inputSignalRMSCalculatorBufferSizeInSample>[nrOfStreams];
}


MixerAlgorithmRMSGainBeforeAndAfterSum::~MixerAlgorithmRMSGainBeforeAndAfterSum(void)
{
    delete[] m_inputSignalRMSCalculatorArray;
}


void MixerAlgorithmRMSGainBeforeAndAfterSum::printAlgorithmConfiguration(void) const
{
    MixerAlgorithm::printAlgorithmConfiguration();
    PRINT_FORMATTED_INFO("number of samples per data chunk: " << s_nrOfSamplesPerChunk);
    PRINT_FORMATTED_INFO("static output sample gain factor: " << s_staticOutputSampleGainFactor);
    PRINT_FORMATTED_INFO("nr of elements in output signal RMS calculator buffer: " << s_outputSignalRMSCalculatorBufferSizeInSample);
    PRINT_FORMATTED_INFO("nr of elements in input signal RMS calculator buffer: " << s_inputSignalRMSCalculatorBufferSizeInSample);
}


/**
 * @brief mix the samples depending on a calculated rms value
 * rms value is calculated for each input stream, stream gain depends on the stream rms value
 * the output gain depends on the output sample rms value
 *
 * @param inputSampleBufferArray input data structure, pointer to 2d array holding 'no of streams' x 'chunk size' data samples
 * @param nrOfStreams number of mixer input streams
 * @param outputSampleBuffer buffer to store the result, 'chunk size' samples
 * @return void
 */
void __attribute__((optimize("O3"))) MixerAlgorithmRMSGainBeforeAndAfterSum::mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer)
{

    int16_t sampleSumBuffer[s_mixerAlgorithmDataElement.getNrOfSamplesPerChunk()];
    static uint64_t clippingCounter = 0;

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

        double outputGain = 1.0;
        if (m_outputSignalRMSCalculator.getRMSValue() != 0) {
            const uint32_t totalRMSValueRange = m_outputSignalRMSCalculator.getMaxRMSValue() - m_outputSignalRMSCalculator.getMinRMSValue();
            const double RMSRatio = m_outputSignalRMSCalculator.getMaxRMSValue() / m_outputSignalRMSCalculator.getRMSValue();
            outputGain = (RMSRatio / (double)totalRMSValueRange) * (std::numeric_limits<int16_t>::max());
        }

        //store
        const int32_t outputSampleValue = s_staticOutputSampleGainFactor * outputGain * sampleSumBuffer[chunkIndex];

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

