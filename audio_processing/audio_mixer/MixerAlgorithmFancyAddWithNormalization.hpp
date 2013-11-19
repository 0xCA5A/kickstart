#ifndef MIXERALGORITHMFANCYADDWITHNORMALIZATION_HPP
#define MIXERALGORITHMFANCYADDWITHNORMALIZATION_HPP


#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"


class MixerAlgorithmFancyAddWithNormalization : public MixerAlgorithm
{
public:
    MixerAlgorithmFancyAddWithNormalization(std::string& algorithmName)
        : MixerAlgorithm(algorithmName) {}
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual void printAlgorithmConfiguration(void) const;
    virtual inline const MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) {return s_mixerAlgorithmDataElement;};
    inline void addSamplesWithNoClipping(const int16_t& sampleA, const int16_t& sampleB, int16_t& outputSample);

private:
    MixerAlgorithmFancyAddWithNormalization(const MixerAlgorithmFancyAddWithNormalization&);
    MixerAlgorithmFancyAddWithNormalization& operator=(const MixerAlgorithmFancyAddWithNormalization&);

private:
    static const MixerAlgorithmDataElement s_mixerAlgorithmDataElement;
    static const uint32_t s_nrOfSamplesPerChunk = 1;
};

#endif