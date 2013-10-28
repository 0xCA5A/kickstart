#include <iostream>

#include "MixerApplication.hpp"
#include "MixerAlgorithmSimpleAddWithClipping.hpp"
#include "MixerAlgorithmSimpleAddWithNormalization.hpp"


int main(int argc, char* argv[])
{
    MixerApplication mixerApplication(--argc, ++argv);

    MixerAlgorithmSimpleAddWithClipping mixerAlgorithmSimpleAddWithClipping;
    MixerAlgorithmSimpleAddWithNormalization mixerAlgorithmSimpleAddWithNormalization;


    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithClipping);
    const std::string mixerAlgorithmSimpleAddWithClippingMixFileName("mixerAlgorithmSimpleAddWithClippingMixResult.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmSimpleAddWithClippingMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithNormalization);
    const std::string mixerAlgorithmSimpleAddWithNormalizationMixFileName("mixerAlgorithmSimpleAddWithNormalizationMixFileName.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmSimpleAddWithNormalizationMixFileName);

    return 0;
}