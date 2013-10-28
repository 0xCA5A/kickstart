#include <iostream>

#include "MixerApplication.hpp"
#include "MixerAlgorithmSimpleAddWithClipping.hpp"
#include "MixerAlgorithmSimpleAddWithNormalization.hpp"


int main(int argc, char* argv[])
{

    std::list<std::string> mixerInputRIFFWAVEFileList;
    mixerInputRIFFWAVEFileList.push_back("samples1.wav");
    mixerInputRIFFWAVEFileList.push_back("samples2.wav");
    mixerInputRIFFWAVEFileList.push_back("samples3.wav");

    MixerApplication mixerApplication;
    mixerApplication.tryToOpenRIFFWAVEFiles(--argc, ++argv);

    MixerAlgorithmSimpleAddWithClipping mixerAlgorithmSimpleAddWithClipping;
    MixerAlgorithmSimpleAddWithNormalization mixerAlgorithmSimpleAddWithNormalization;


    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithClipping);
    const std::string mixerAlgorithmSimpleAddWithClippingMixFileName("mixerAlgorithmSimpleAddWithClippingMixResult.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerInputRIFFWAVEFileList, mixerAlgorithmSimpleAddWithClippingMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithNormalization);
    const std::string mixerAlgorithmSimpleAddWithNormalizationMixFileName("mixerAlgorithmSimpleAddWithNormalizationMixFileName.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerInputRIFFWAVEFileList, mixerAlgorithmSimpleAddWithNormalizationMixFileName);

    return 0;
}