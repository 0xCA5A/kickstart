#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <stdint.h>


//ringbuffer rules
//* read pointer can never overtake write pointer
//* unread data can never be overwritten
template<typename T, const uint32_t ringBufferSizeInElements>
class RingBuffer
{

public:
    RingBuffer()
        : m_readIndex(0)
        , m_writeIndex(0)
    {}

    ~RingBuffer()
    {}

    uint32_t put(const T* pData, uint32_t count)
    {
        uint32_t writeIndex = m_writeIndex;
        uint32_t readIndex = m_readIndex;
        uint32_t free;

        //get number of free elements
        if (readIndex > writeIndex) {
            free = readIndex - writeIndex - 1;
        } else {
            free = (m_numberOfDataArrayElements - 1) - (writeIndex - readIndex);
        }

        //do not overwrite data
        if (count > free) {
            count = free;
        }

        for (uint32_t i = 0; i < count; i++) {
            m_elements[writeIndex] = *pData++;
            //catch overrun
            if (++writeIndex >= m_numberOfDataArrayElements) {
                writeIndex = 0;
            }
        }

        m_writeIndex = writeIndex;

        return count;
    }

    uint32_t get(T * pData, uint32_t count)
    {
        uint32_t writeIndex = m_writeIndex;
        uint32_t readIndex = m_readIndex;
        uint32_t used;

        //get number of used elements
        if (writeIndex >= readIndex) {
            used = writeIndex - readIndex;
        } else {
            used = m_numberOfDataArrayElements - (readIndex - writeIndex);
        }

        //do not overtake write pointer
        if (count > used) {
            count = used;
        }

        for (uint32_t i = 0; i < count; i++) {
            *pData++ = m_elements[readIndex];
            //catch overrun
            if (++readIndex >= m_numberOfDataArrayElements) {
                readIndex = 0;
            }
        }

        m_readIndex = readIndex;

        return count;
    }

    uint32_t getNumberOfUsedElements() const
    {
        uint32_t writeIndex = m_writeIndex;
        uint32_t readIndex = m_readIndex;
        uint32_t used;
        if (writeIndex >= readIndex) {
            used = writeIndex - readIndex;
        } else {
            used = m_numberOfDataArrayElements - (readIndex - writeIndex);
        }
        return used;
    }

    inline uint32_t getRingBufferSizeInElements() const
    {
        return m_numberOfDataArrayElements;
    }

    inline uint32_t getRingBufferDataSize() const
    {
        return m_numberOfDataArrayElements * sizeof(T);
    }

private:
    RingBuffer(const RingBuffer&);
    RingBuffer& operator=(const RingBuffer&);

    static const uint32_t m_numberOfDataArrayElements = ringBufferSizeInElements + 1;

    //NOTE: hope this is continuous memory
    T m_elements[m_numberOfDataArrayElements];
    volatile uint32_t m_readIndex;
    volatile uint32_t m_writeIndex;
};

#endif
