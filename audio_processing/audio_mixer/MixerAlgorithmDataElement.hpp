#ifndef MIXERALGORITHMDATAELEMENT_HPP
#define MIXERALGORITHMDATAELEMENT_HPP

#include <stdint.h>



/**
 * @brief date element to store algorithm specific configuration stuff
 *
 */
class MixerAlgorithmDataElement
{

public:
    MixerAlgorithmDataElement(const uint32_t nrOfSamplesPerChunk) : m_nrOfSamplesPerChunk(nrOfSamplesPerChunk) {};
    inline const uint32_t getNrOfSamplesPerChunk() const {return m_nrOfSamplesPerChunk;} ;

private:
    MixerAlgorithmDataElement(const MixerAlgorithmDataElement&);
    MixerAlgorithmDataElement& operator=(const MixerAlgorithmDataElement&);

private:
    const uint32_t m_nrOfSamplesPerChunk;

};

#endif


