#include <iostream>
#include <typeinfo>

#include "MixerAlgorithmSimpleAddWithClipping.hpp"



void MixerAlgorithmSimpleAddWithClipping::mixSamples()
{
    std::cout << typeid(*this).name() << std::endl;


// short sample1 = ...;
// short sample2 = ...;
// float samplef1 = sample1 / 32768.0f;
// float samplef2 = sample2 / 32768.0f;
// float mixed = samplef1 + sample2f;
// // reduce the volume a bit:
// mixed *= 0.8;
// // hard clipping
// if (mixed > 1.0f) mixed = 1.0f;
// if (mixed < -1.0f) mixed = -1.0f;
// short outputSample = (short)(mixed * 32768.0f)

    
}
