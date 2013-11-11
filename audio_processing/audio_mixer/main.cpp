#include <iostream>

#include "MixerApplication.hpp"
#include "MixerAlgorithmSimpleAddWithClipping.hpp"
#include "MixerAlgorithmSimpleAddWithNormalization.hpp"
#include "MixerAlgorithmFancyAddWithNormalization.hpp"
#include "MixerAlgorithmRMSGainBeforeSum.hpp"
#include "MixerAlgorithmRMSGainBeforeAndAfterSum.hpp"
#include "MixerAlgorithmRMSGainAfterSum.hpp"


int main(int argc, char* argv[])
{
    MixerApplication mixerApplication(--argc, ++argv);

    std::string algorithmName;


    //get the algorithms
    algorithmName = "mixerAlgorithmSimpleAddWithClipping";
    MixerAlgorithmSimpleAddWithClipping mixerAlgorithmSimpleAddWithClipping(algorithmName);

    algorithmName = "mixerAlgorithmSimpleAddWithNormalization";
    MixerAlgorithmSimpleAddWithNormalization mixerAlgorithmSimpleAddWithNormalization(algorithmName);

    algorithmName = "mixerAlgorithmFancyAddWithNormalization";
    MixerAlgorithmFancyAddWithNormalization mixerAlgorithmFancyAddWithNormalization(algorithmName);

    algorithmName = "mixerAlgorithmRMSGainBeforeSum";
    MixerAlgorithmRMSGainBeforeSum mixerAlgorithmRMSGainBeforeSum(algorithmName, mixerApplication.getNrOfInputFiles());

    algorithmName = "mixerAlgorithmRMSGainBeforeAndAfterSum";
    MixerAlgorithmRMSGainBeforeAndAfterSum mixerAlgorithmRMSGainBeforeAndAfterSum(algorithmName, mixerApplication.getNrOfInputFiles());

    algorithmName = "mixerAlgorithmRMSGainAfterSum";
    MixerAlgorithmRMSGainAfterSum mixerAlgorithmRMSGainAfterSum(algorithmName);


    //start mixing
    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithClipping);
    const std::string mixerAlgorithmSimpleAddWithClippingMixOutputFileName(mixerAlgorithmSimpleAddWithClipping.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmSimpleAddWithClippingMixOutputFileName);

    mixerApplication.setStrategy(&mixerAlgorithmSimpleAddWithNormalization);
    const std::string mixerAlgorithmSimpleAddWithNormalizationMixFileName(mixerAlgorithmSimpleAddWithNormalization.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmSimpleAddWithNormalizationMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmFancyAddWithNormalization);
    const std::string mixerAlgorithmFancyAddWithNormalizationMixFileName(mixerAlgorithmFancyAddWithNormalization.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmFancyAddWithNormalizationMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmRMSGainBeforeSum);
    const std::string mixerAlgorithmRMSGainBeforeSumMixFileName(mixerAlgorithmRMSGainBeforeSum.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmRMSGainBeforeSumMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmRMSGainBeforeAndAfterSum);
    const std::string mixerAlgorithmRMSGainBeforeAndAfterSumMixFileName(mixerAlgorithmRMSGainBeforeAndAfterSum.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmRMSGainBeforeAndAfterSumMixFileName);

    mixerApplication.setStrategy(&mixerAlgorithmRMSGainAfterSum);
    const std::string mixerAlgorithmRMSGainAfterSumMixFileName(mixerAlgorithmRMSGainAfterSum.getAlgorithmIdentifier() + "MixResultData.wav");
    mixerApplication.mixRIFFWAVEFiles(mixerAlgorithmRMSGainAfterSumMixFileName);

    return 0;
}