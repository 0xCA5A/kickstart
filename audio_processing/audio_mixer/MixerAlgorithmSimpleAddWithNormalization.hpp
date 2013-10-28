#ifndef MIXERALGORITHMSIMPLEADDWITHNORMALIZATION_HPP
#define MIXERALGORITHMSIMPLEADDWITHNORMALIZATION_HPP


#include "MixerAlgorithm.hpp"


/**
 * @brief implementation of simple sample add algorithm with hard clipping
 *
 */
class MixerAlgorithmSimpleAddWithNormalization : public MixerAlgorithm
{
public:
    virtual void mixSamples();
};

#endif

