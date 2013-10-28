#include <iostream>
#include <typeinfo>
#include <fstream>
#include <sndfile.h>

#include "MixerApplication.hpp"


MixerApplication::MixerApplication(unsigned int numberOfInputFiles, char* inputFileNameArray[])
    : m_p_mixerAlgorithm(0)
{
    m_p_soundFileHandlerArray = new SNDFILE*[numberOfInputFiles];
    m_p_soundFileInfoArray = new SF_INFO[numberOfInputFiles];

    for (unsigned int i = 0; i < numberOfInputFiles; i++)
    {
        m_mixerInputRIFFWAVEFileList.push_back(inputFileNameArray[i]);
    }

    tryToOpenRIFFWAVEFiles();
}


MixerApplication::~MixerApplication(void)
{
    //close files
    for (unsigned int index = 0; index < m_mixerInputRIFFWAVEFileList.size(); ++index)
    {

        sf_close(m_p_soundFileHandlerArray[index]);
    }

    //delete structures
    delete [] m_p_soundFileHandlerArray;
    delete [] m_p_soundFileInfoArray;
}


bool MixerApplication::tryToOpenRIFFWAVEFiles(void)
{
    std::list<std::string>::const_iterator listIterator = m_mixerInputRIFFWAVEFileList.begin();
    for (unsigned int fileIndex = 0; fileIndex < m_mixerInputRIFFWAVEFileList.size(); ++fileIndex)
    {

        m_p_soundFileHandlerArray[fileIndex] = sf_open((*listIterator).c_str(), SFM_READ, &m_p_soundFileInfoArray[fileIndex]);

        if (m_p_soundFileHandlerArray[fileIndex] == NULL)
        {
            std::cout << "[!] unable to open the wave file " << (*listIterator).c_str() << ": " << sf_strerror(m_p_soundFileHandlerArray[fileIndex]) << std::endl;
            return false;
        }

        ++listIterator;

    }

    return true;
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


void MixerApplication::mixRIFFWAVEFiles(const std::string& riffWaveMixFileName)
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
