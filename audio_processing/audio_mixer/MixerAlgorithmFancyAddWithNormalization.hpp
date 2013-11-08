#ifndef MIXERALGORITHMSIMPLEADDWITHNORMALIZATION_HPP
#define MIXERALGORITHMSIMPLEADDWITHNORMALIZATION_HPP

#include "MixerAlgorithm.hpp"
#include "MixerAlgorithmDataElement.hpp"



/**
 * @brief implementation of a more or less fancy algorithm with dynamic normalization
 * with substitution, this formula explodes...
 * http://www.wolframalpha.com/input/?i=+t%3Da%2Bx-%28a*x%29%2C+x%3Db%2By-%28b*y%29%2C+y%3Dc%2Bz-%28c*z%29%2C+z%3Dd%2Be-%28d*e%29
 * http://www.wolframalpha.com/input/?i=+t%3Da%2Bw-%28a*w%29%2C+w%3Db%2Bx-%28b*x%29%2C+x%3Dc%2By-%28c*y%29%2C+y%3Dd%2Bz-%28d*z%29%2C+z%3De%2Bf-%28e*f%29
 *
 */
class MixerAlgorithmSimpleAddWithNormalization : public MixerAlgorithm
{
public:
    MixerAlgorithmSimpleAddWithNormalization(std::string& algorithmName)
    : MixerAlgorithm(algorithmName) {}
    virtual void mixSamples(int16_t** const inputSampleBufferArray, const uint32_t nrOfStreams, int16_t* const outputSampleBuffer);
    virtual inline const MixerAlgorithmDataElement& getMixerAlgorithmDataElementPrototype(void) const {return s_mixerAlgorithmDataElement;};

private:
    MixerAlgorithmSimpleAddWithNormalization(const MixerAlgorithmSimpleAddWithNormalization&);
    MixerAlgorithmSimpleAddWithNormalization& operator=(const MixerAlgorithmSimpleAddWithNormalization&);

private:
    static const uint32_t s_nrOfSamplesPerChunk = 1;
    static const MixerAlgorithmDataElement s_mixerAlgorithmDataElement;
};

#endif