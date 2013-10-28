#include <iostream>
#include <typeinfo>
#include <fstream>
#include <sndfile.h>

#include "MixerApplication.hpp"


MixerApplication::MixerApplication(unsigned int numberOfInputFiles, char* inputFileNameArray[])
    : m_p_mixerAlgorithm(0)
    , m_soundFileNameArray(inputFileNameArray)

{
    m_soundFileInfoArray = new SF_INFO[123];
}


MixerApplication::~MixerApplication(void )
{
}


bool MixerApplication::tryToOpenRIFFWAVEFiles(const unsigned int nrOfFiles, char** fileList)
{

    for (unsigned int fileIndex = 0; fileIndex < nrOfFiles; ++fileIndex)
    {

        sf_open();
//         if (!MixerApplication::checkIfFileIsReadable(argv[fileIndex]))
//         {
//             std::cout << __FUNCTION__ << " [!] file " << argv[fileIndex] << "is not readable" << std::endl;
//             return -1;
//         }
// 
//         if (!MixerApplication::checkIfFileIsRIFFWAVEFile(argv[fileIndex]))
//         {
//             std::cout << __FUNCTION__ << " [!] file " << argv[fileIndex] << "is not a valid RIFF wave file" << std::endl;
//             return -1;
//         }

    }
}

// bool MixerApplication::openRIFFWAVEFileForReading(const char* riffWaveMixFileName)
// {
//     sf_open(riffWaveMixFileName, );
// }
// 
// bool MixerApplication::checkIfFileIsRIFFWAVEFile(const char* riffWaveMixFileName)
// {
//     sf_open();
//     sf_open_virtual();
//     return true;
// }


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

    //check if the files do exist or not...
//     for (std::list<std::string>::const_iterator iterator = riffWaveFileNameList.begin(); iterator != riffWaveFileNameList.end(); ++iterator)
//     {
//         if (!isFileInputFile(iterator->c_str()))
//         {
//             std::cout << __FUNCTION__ << " [!] file " << *iterator << " does not exist" << std::endl;
//             return;
//         }
// //         std::cout << "file: " << *iterator << std::endl;
// //         std::cout << typeid(*this).name() << std::endl;
//     }

    //find shortest file to determine the maximum mix duration

    //mix the samples
    m_p_mixerAlgorithm->mixSamples();

}
