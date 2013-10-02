#include <iostream>
#include <typeinfo>

#include "MixerApplication.hpp"


MixerApplication::MixerApplication()
    : m_p_mixerAlgorithm(0)
{
}


MixerApplication::~MixerApplication(void )
{
}


void MixerApplication::setStrategy(MixerAlgorithm* mixerAlgorithm)
{
    m_p_mixerAlgorithm = mixerAlgorithm;
}


void MixerApplication::mixRIFFWAVEFiles(const std::list<std::string>& riffWaveFileNameList, const std::string& riffWaveMixFileName)
{
    //check if mixer strategy is set
    if (m_p_mixerAlgorithm == 0)
    {
        std::cout << __FUNCTION__ << " [!] mixer strategy not set!" << std::endl;
        return;
    }
    std::cout << __FUNCTION__ << " [i] mixer strategy ok!" << std::endl;

    //try to open RIFF WAVE files
    for (std::list<std::string>::const_iterator iterator = riffWaveFileNameList.begin(); iterator != riffWaveFileNameList.end(); ++iterator)
    {
        std::cout << "file: " << *iterator << std::endl;
        std::cout << typeid(*this).name() << std::endl;
    }

    //find shortest file to determine the maximum mix duration

    //mix the samples
    m_p_mixerAlgorithm->algorithm();

}
