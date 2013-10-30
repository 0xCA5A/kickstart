#include <iostream>
#include <sndfile.h>
#include <cstdlib>
#include <strings.h>
#include <limits>

#include "MixerApplication.hpp"


MixerApplication::MixerApplication(unsigned int numberOfInputFiles, char* inputFileNameArray[])
    : m_p_mixerAlgorithm(NULL)
    , m_nrOfInputFiles(numberOfInputFiles)
{
    std::cout << "[i] init internal data structures" << std::endl;

    if (m_nrOfInputFiles < 2)
    {
        std::cout << "[!] expect at least two input files..." << std::endl;
        exit(EXIT_FAILURE);
    }

    m_p_soundFileHandlerArray = new SNDFILE*[m_nrOfInputFiles];
    m_p_soundFileInfoArray = new SF_INFO[m_nrOfInputFiles];
    m_p_gainFactorArray = new float[m_nrOfInputFiles];

    for (unsigned int i = 0; i < m_nrOfInputFiles; i++)
    {
        m_mixerInputRIFFWAVEFileNameList.push_back(inputFileNameArray[i]);
        m_p_gainFactorArray[i] = 1.0;
    }

    tryToOpenRIFFWAVEFiles();
    checkWaveFileConfiguration();

    std::cout << "[i] init successful" << std::endl;
}


MixerApplication::~MixerApplication(void)
{
    std::cout << "[i] delete internal data structures" << std::endl;

    //close files
    std::cout << "[i] close wave files..." << std::endl;
    for (unsigned int index = 0; index < m_mixerInputRIFFWAVEFileNameList.size(); ++index)
    {
        sf_close(m_p_soundFileHandlerArray[index]);
    }

    //delete structures
    delete [] m_p_soundFileHandlerArray;
    delete [] m_p_soundFileInfoArray;
}


void MixerApplication::checkWaveFileConfiguration(void) const
{
    //check if all files have the same configuration...
    //no sampling rate conversion at the moment...

    //check if data structures are initialized
    if (m_p_soundFileInfoArray == NULL)
    {
        std::cout << "[!] sound file data structures not ready..." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int unsigned fileIndex = 1; fileIndex < m_nrOfInputFiles; fileIndex++)
    {
        if (m_p_soundFileInfoArray[fileIndex].channels != 1)
        {
            std::cout << "[!] at the moment only one channel files are supported " << fileIndex << std::endl;
            exit(EXIT_FAILURE);
        }
        if (m_p_soundFileInfoArray[0].format != m_p_soundFileInfoArray[fileIndex].format)
        {
            std::cout << "[!] format differ at file index 0 and " << fileIndex << std::endl;
            exit(EXIT_FAILURE);
        }
        if (m_p_soundFileInfoArray[0].samplerate != m_p_soundFileInfoArray[fileIndex].samplerate)
        {
            std::cout << "[!] samplerate differ at file index 0 and " << fileIndex << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "[i] sound file configuration (channels, format, samplerate) ok..." << std::endl;
}


void MixerApplication::tryToOpenRIFFWAVEFiles(void)
{
    std::cout << "[i] try to open wave files..." << std::endl;
    std::list<std::string>::const_iterator listIterator = m_mixerInputRIFFWAVEFileNameList.begin();
    for (unsigned int fileIndex = 0; fileIndex < m_mixerInputRIFFWAVEFileNameList.size(); ++fileIndex)
    {
        std::cout << " * open file " << *listIterator << std::endl;
        m_p_soundFileHandlerArray[fileIndex] = sf_open((*listIterator).c_str(), SFM_READ, &m_p_soundFileInfoArray[fileIndex]);

        if (m_p_soundFileHandlerArray[fileIndex] == NULL)
        {
            std::cout << "[!] unable to open the wave file " << (*listIterator).c_str() << ": " << sf_strerror(m_p_soundFileHandlerArray[fileIndex]) << std::endl;
            exit(EXIT_FAILURE);
        }

        ++listIterator;
    }
}


void MixerApplication::setStrategy(MixerAlgorithm* const mixerAlgorithm)
{
    if (mixerAlgorithm == NULL)
    {
        std::cout << "[!] null pointer strategy is bad..." << std::endl;
        exit(EXIT_FAILURE);
    }
    m_p_mixerAlgorithm = mixerAlgorithm;
}


void MixerApplication::mixRIFFWAVEFiles(const std::string& riffWaveMixFileName) const
{
    //check if mixer strategy is set
    if (m_p_mixerAlgorithm == 0)
    {
        std::cout << " [!] mixer strategy not set!" << std::endl;
        return;
    }
    std::cout <<"[i] mixer strategy " << m_p_mixerAlgorithm->getAlgorithmName() << " selected" << std::endl;

    //init sf info structure
    SF_INFO mixSfInfo;
    bzero(&mixSfInfo, sizeof(mixSfInfo));
    mixSfInfo.channels = m_p_soundFileInfoArray[0].channels;
    mixSfInfo.format = m_p_soundFileInfoArray[0].format;
    mixSfInfo.samplerate = m_p_soundFileInfoArray[0].samplerate;
    mixSfInfo.frames = getMinimalNrOfFramesInFiles();

    std::cout << "[i] minimal number of frames in input files: " << mixSfInfo.frames << std::endl;

    //open output file
    std::cout << "[i] open mixer output file " << riffWaveMixFileName << std::endl;
    SNDFILE* mixFileHandler = sf_open(riffWaveMixFileName.c_str(), SFM_WRITE, &mixSfInfo);
    if (mixFileHandler == NULL)
    {
        std::cout << "[!] unable to open audio out file: " << riffWaveMixFileName.c_str() << std::endl;
        exit(EXIT_FAILURE);
    }

    //prepare dynamic buffers, allocate memory
    std::cout << "[i] prepare internal 16bit buffers..." << std::endl;
    int16_t** const pp_inputSampleBufferArray = new int16_t*[m_nrOfInputFiles];
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
    {
        pp_inputSampleBufferArray[i] = new int16_t[m_p_soundFileInfoArray[0].channels];
    }
    int16_t* p_outputSampleBuffer = new int16_t[m_p_mixerAlgorithm->getNrOfSamplesPerChunk()];

    std::cout << "[i] process frames..." << std::endl;
    for (uint32_t chunkIndex = 0; chunkIndex < (getMinimalNrOfFramesInFiles() / m_p_mixerAlgorithm->getNrOfSamplesPerChunk()); ++chunkIndex)
    {
        //read the data from file to process data structure
        for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
        {
            sf_read_short(m_p_soundFileHandlerArray[i], pp_inputSampleBufferArray[i], m_p_mixerAlgorithm->getNrOfSamplesPerChunk());
        }

        //execute algorithm, mix the samples chunk by chunk
        m_p_mixerAlgorithm->mixSamples(pp_inputSampleBufferArray, m_nrOfInputFiles, m_p_mixerAlgorithm->getNrOfSamplesPerChunk(), p_outputSampleBuffer);

        sf_write_short(mixFileHandler, p_outputSampleBuffer, m_p_mixerAlgorithm->getNrOfSamplesPerChunk());
    }
    std::cout << "[i] process done..." << std::endl;

    //free data structures
    delete [] p_outputSampleBuffer;
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
    {
        delete [] pp_inputSampleBufferArray[i];
    }
    delete [] pp_inputSampleBufferArray;

    //close output file
    sf_close(mixFileHandler);
}


uint32_t MixerApplication::getMinimalNrOfFramesInFiles(void) const
{
    uint32_t minimalNrOfFrames = std::numeric_limits<uint32_t>::max();
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
    {
        if (minimalNrOfFrames > m_p_soundFileInfoArray[i].frames)
        {
            minimalNrOfFrames = m_p_soundFileInfoArray[i].frames;
        }
    }

    return minimalNrOfFrames;
}
