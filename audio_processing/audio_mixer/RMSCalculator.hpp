#ifndef RMSCALCULATOR_HPP
#define RMSCALCULATOR_HPP

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "PrintMacros.hpp"


template<typename T, const uint32_t bufferSizeInSamples>
class RMSCalculator
{

public:
    RMSCalculator()
        : m_writeIndex(0)
    {
        PRINT_FORMATTED_INFO("create RMSCalculator, buffer size " << bufferSizeInSamples << ", " << bufferSizeInSamples / (float)(16000 / 1000) << "ms @ 16khz");
        //inti buffer with zero samples
        memset(m_sampleBuffer, 0, bufferSizeInSamples * sizeof(T));
    }

    ~RMSCalculator()
    {}

    void putSample(const T* pData)
    {
        if (++m_writeIndex >= m_bufferSizeInSamples) {
            m_writeIndex = 0;
        }
        m_sampleBuffer[m_writeIndex] = *pData;
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
        int64_t superBigSmapleBuffer = 0;
        for (uint32_t i = 0; i < m_bufferSizeInSamples; i++) {
            superBigSmapleBuffer += (m_sampleBuffer[i] * m_sampleBuffer[i]);
        }
        superBigSmapleBuffer /= m_bufferSizeInSamples;

        return round(sqrt(superBigSmapleBuffer));
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
    volatile uint32_t m_writeIndex;
};

#endif
