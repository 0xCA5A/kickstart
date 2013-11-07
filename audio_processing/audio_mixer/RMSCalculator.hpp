#ifndef RMSCALCULATOR_HPP
#define RMSCALCULATOR_HPP

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "PrintMacros.hpp"



/**
 * @brief class to calculate the root mean square (RMS) over a data buffer
 * note: check sample data type, int16_t, uint16_t, int32_t, uint32_t are supported
 *
 */
template<typename T, const uint32_t bufferSizeInSamples>
class RMSCalculator
{

public:
    RMSCalculator()
        : m_writeIndex(0)
    {
        PRINT_FORMATTED_INFO("create RMSCalculator, buffer size " << bufferSizeInSamples << ", " << bufferSizeInSamples / (float)(16000 / 1000) << "ms @ 16khz");
        //inti buffer with zero samples
        memset(m_sampleBuffer, 0, bufferSizeInSamples * sizeof(m_sampleBuffer[0]));
        memset(m_sampleSquareBuffer, 0, bufferSizeInSamples * sizeof(m_sampleSquareBuffer[0]));
    }

    ~RMSCalculator()
    {}

    void putSample(const T* pData)
    {
        if (++m_writeIndex >= m_bufferSizeInSamples) {
            m_writeIndex = 0;
        }
        m_sampleBuffer[m_writeIndex] = *pData;
        m_sampleSquareBuffer[m_writeIndex] = *pData * *pData;
    }

    void putSamples(const T* pData, uint32_t count)
    {
        //TODO: optimize this using memcpy
        for (uint32_t i = 0; i < count; i++) {
            putSample(pData);
        }
    }

    void printDataElements(void)
    {
        std::cout << std::endl;
        std::cout  << "################################################################################" << std::endl;
        std::cout << "# [i] RMS Calculator array elements:" << std::endl;
        for (uint32_t i = 0; i < m_bufferSizeInSamples; i++) {
            std::cout << " " << i << ": " << m_sampleBuffer[i];
            if (i == m_writeIndex) {
                std::cout << " <-- write index";
            }
            std::cout << std::endl;
        }
        std::cout << "################################################################################" << std::endl;
        std::cout << std::endl;
    }

    T getRMSValue(void) __attribute__((optimize("O3"))) 
    {
        unsigned long long superBigSmapleSquareBuffer = 0;
        for (uint32_t i = 0; i < m_bufferSizeInSamples; i++) {
            superBigSmapleSquareBuffer += m_sampleSquareBuffer[i];
        }
        superBigSmapleSquareBuffer /= m_bufferSizeInSamples;

        return round(sqrt(superBigSmapleSquareBuffer));
    }

    inline uint32_t getBufferSizeInSamples() const
    {
        return m_bufferSizeInSamples;
    }

    inline uint32_t getBufferDataSizeInByte() const
    {
        return m_bufferSizeInSamples * sizeof(T);
    }

private:
    RMSCalculator(const RMSCalculator&);
    RMSCalculator& operator=(const RMSCalculator&);

    //TODO: check this, i dont want to have this static
    static const uint32_t m_bufferSizeInSamples = bufferSizeInSamples;

    //NOTE: hope this is continuous memory
    T m_sampleBuffer[m_bufferSizeInSamples];
    //NOTE: this should be enough for samples int16_t, uint16_t, int32_t, uint32_t
    // if sizeof(m_sampleBuffer[0]) > sizeof(uint64_t) / 2, we might have a problem
    uint64_t m_sampleSquareBuffer[m_bufferSizeInSamples];
    volatile uint32_t m_writeIndex;
};

#endif
