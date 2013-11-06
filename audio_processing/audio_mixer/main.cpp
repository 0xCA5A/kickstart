#include <iostream>

#include "MixerApplication.hpp"
#include "MixerAlgorithmSimpleAddWithClipping.hpp"
#include "MixerAlgorithmSimpleAddWithNormalization.hpp"
#include "MixerAlgorithmSimpleAddWithSmoothNormalization.hpp"
#include "MixerAlgorithmRMSGainBeforeSum.hpp"
#include "MixerAlgorithmRMSGainBeforeAndAfterSum.hpp"


int main(int argc, char* argv[])
{
    MixerApplication mixerApplication(--argc, ++argv);

    std::string algorithmName;

    //get the algorithms
    algorithmName = "mixerAlgorithmSimpleAddWithClipping";
    MixerAlgorithmSimpleAddWithClipping mixerAlgorithmSimpleAddWithClipping(algorithmName);

    algorithmName = "mixerAlgorithmSimpleAddWithNormalization";
    MixerAlgorithmSimpleAddWithNormalization mixerAlgorithmSimpleAddWithNormalization(algorithmName);

    algorithmName = "mixerAlgorithmSimpleAddWithSmoothNormalization";
    MixerAlgorithmSimpleAddWithSmoothNormalization mixerAlgorithmSimpleAddWithSmoothNormalization(algorithmName);

    algorithmName = "mixerAlgorithmRMSGainBeforeSum";
    MixerAlgorithmRMSGainBeforeSum mixerAlgorithmRMSGainBeforeSum(algorithmName);

    algorithmName = "mixerAlgorithmRMSGainBeforeAndAfterSum";
    MixerAlgorithmRMSGainBeforeAndAfterSum mixerAlgorithmRMSGainBeforeAndAfterSum(algorithmName);

    //start mixing
    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithClipping);
    const std::string mixerAlgorithmSimpleAddWithClippingMixOutputFileName(mixerAlgorithmSimpleAddWithClipping.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmSimpleAddWithClippingMixOutputFileName);

    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithNormalization);
    const std::string mixerAlgorithmSimpleAddWithNormalizationMixFileName(mixerAlgorithmSimpleAddWithNormalization.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmSimpleAddWithNormalizationMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithSmoothNormalization);
    const std::string mixerAlgorithmSimpleAddWithSmoothNormalizationMixFileName(mixerAlgorithmSimpleAddWithSmoothNormalization.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmSimpleAddWithSmoothNormalizationMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmRMSGainBeforeSum);
    const std::string mixerAlgorithmRMSGainBeforeSumMixFileName(mixerAlgorithmRMSGainBeforeSum.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmRMSGainBeforeSumMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmRMSGainBeforeAndAfterSum);
    const std::string mixerAlgorithmRMSGainBeforeAndAfterSumMixFileName(mixerAlgorithmRMSGainBeforeAndAfterSum.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmRMSGainBeforeAndAfterSumMixFileName);

    return 0;
}