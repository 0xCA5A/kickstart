#include <iostream>
#include <sndfile.h>
#include <cstdlib>
#include <strings.h>
#include <limits>
#include <cstring>

#include "MixerApplication.hpp"
#include "MixerAlgorithmDataElement.hpp"


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
    std::cout << "[i] delete internal data structures and close input files" << std::endl;

    //close input files
    std::cout << "[i] close wave files..." << std::endl;
    for (unsigned int index = 0; index < m_mixerInputRIFFWAVEFileNameList.size(); ++index)
    {
        sf_close(m_p_soundFileHandlerArray[index]);
    }
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
    std::cout << "[i] try to open input wave files..." << std::endl;
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


void MixerApplication::tryToRewindRIFFWAVEFiles(void)
{
    std::cout << "[i] try to rewind input wave files..." << std::endl;
    std::list<std::string>::const_iterator listIterator = m_mixerInputRIFFWAVEFileNameList.begin();
    for (unsigned int fileIndex = 0; fileIndex < m_mixerInputRIFFWAVEFileNameList.size(); ++fileIndex)
    {
        if (m_p_soundFileHandlerArray[fileIndex] == NULL)
        {
            std::cout << " * file " << *listIterator << " was NULL. this is not optimal, skip this one..." << std::endl;
            continue;
        }

        std::cout << " * rewind file " << *listIterator << std::endl;
        sf_seek(m_p_soundFileHandlerArray[fileIndex], 0, SEEK_SET);
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


void MixerApplication::mixRIFFWAVEFiles(const std::string& riffWaveMixFileName)
{
    tryToRewindRIFFWAVEFiles();

    //check if mixer strategy is set
    if (m_p_mixerAlgorithm == 0)
    {
        std::cout << " [!] mixer strategy not set!" << std::endl;
        return;
    }
    std::cout <<"\n[i] mixer strategy " << m_p_mixerAlgorithm->getAlgorithmIdentifier() << " selected" << std::endl;

    //init sf info structure
    SF_INFO mixSfInfo;
    bzero(&mixSfInfo, sizeof(mixSfInfo));
    mixSfInfo.channels = m_p_soundFileInfoArray[0].channels;
    mixSfInfo.format = m_p_soundFileInfoArray[0].format;
    mixSfInfo.samplerate = m_p_soundFileInfoArray[0].samplerate;
    mixSfInfo.frames = getMaximumNrOfFramesInFiles();

    std::cout << "[i] minimum number of frames in input files: " << getMinimumNrOfFramesInFiles() << std::endl;
    std::cout << "[i] maximum number of frames in input files: " << getMaximumNrOfFramesInFiles() << std::endl;

    //open output file
    std::cout << "[i] open mixer output file " << riffWaveMixFileName << std::endl;
    SNDFILE* mixFileHandler = sf_open(riffWaveMixFileName.c_str(), SFM_WRITE, &mixSfInfo);
    if (mixFileHandler == NULL)
    {
        std::cout << "[!] unable to open audio out file: " << riffWaveMixFileName.c_str() << std::endl;
        exit(EXIT_FAILURE);
    }

    //get algorithm data element prototype
    MixerAlgorithmDataElement& mixerAlgorithmDataElementPrototype = m_p_mixerAlgorithm->getMixerAlgorithmDataElementPrototype();

    //prepare dynamic buffers, allocate memory
    std::cout << "[i] prepare internal 16bit buffers..." << std::endl;
    int16_t** const pp_inputSampleBufferArray = new int16_t*[m_nrOfInputFiles];
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
    {
        pp_inputSampleBufferArray[i] = new int16_t[m_p_soundFileInfoArray[0].channels];
    }
    int16_t* p_outputSampleBuffer = new int16_t[mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk()];

    std::cout << "[i] mix " << getMaximumNrOfFramesInFiles() << " samples ";
    for (uint32_t chunkIndex = 0; chunkIndex < (getMaximumNrOfFramesInFiles() / mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk()); ++chunkIndex)
    {
        //read the data from file to process data structure
        uint32_t nrOfReadItems;
        for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
        {
            //NOTE: write zero data sample if no data was read from input file
            nrOfReadItems = sf_read_short(m_p_soundFileHandlerArray[i], pp_inputSampleBufferArray[i], mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk());
            if (nrOfReadItems == 0)
            {
                //write zero data, 'sample size' times 'nr of samples per chunk'
                memset(pp_inputSampleBufferArray[i], 0, sizeof(int16_t) * mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk());
            }
        }

        //execute algorithm, mix the samples chunk by chunk
        //TODO: change api to something like in mpg123, feed samples and the algorithm does something if enough samples feeded...
        m_p_mixerAlgorithm->mixSamples(pp_inputSampleBufferArray, m_nrOfInputFiles, p_outputSampleBuffer);

        if (chunkIndex % 256 == 0)
        {
            std::cout << ".";
        }

        //write mixed data to file
        sf_write_short(mixFileHandler, p_outputSampleBuffer, mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk());
    }
    std::cout << " done!" << std::endl;

    //delete data structures
    std::cout << "[i] delete internal buffers..." << std::endl;
    delete [] p_outputSampleBuffer;
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
    {
        delete [] pp_inputSampleBufferArray[i];
    }
    delete [] pp_inputSampleBufferArray;

    //close output file
    std::cout << "[i] close mixer output file " << riffWaveMixFileName << std::endl;
    sf_close(mixFileHandler);

    std::cout << "[i] mixing done using algorithm " << m_p_mixerAlgorithm->getAlgorithmIdentifier() << std::endl << std::endl;
}


inline uint32_t MixerApplication::getMinimumNrOfFramesInFiles(void) const
{
    uint32_t minimumNrOfFrames = std::numeric_limits<uint32_t>::max();
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
    {
        if (minimumNrOfFrames > m_p_soundFileInfoArray[i].frames)
        {
            minimumNrOfFrames = m_p_soundFileInfoArray[i].frames;
        }
    }

    return minimumNrOfFrames;
}


inline uint32_t MixerApplication::getMaximumNrOfFramesInFiles(void) const
{
    uint32_t maximumNrOfFrames = std::numeric_limits<uint32_t>::min();
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i)
    {
        if (maximumNrOfFrames < m_p_soundFileInfoArray[i].frames)
        {
            maximumNrOfFrames = m_p_soundFileInfoArray[i].frames;
        }
    }

    return maximumNrOfFrames;
}