#include <sndfile.h>
#include <cstdlib>
#include <strings.h>
#include <limits>
#include <cstring>
#include <cstdio>
#include <ctime>

#include "MixerApplication.hpp"
#include "MixerAlgorithmDataElement.hpp"
#include "PrintMacros.hpp"


MixerApplication::MixerApplication(unsigned int numberOfInputFiles, char* inputFileNameArray[])
    : m_p_mixerAlgorithm(NULL)
    , m_nrOfInputFiles(numberOfInputFiles)
{
    PRINT_FORMATTED_INFO("init internal data structures");

    if (m_nrOfInputFiles < 2) {
        PRINT_FORMATTED_ERROR("expect at least two input files...");
        exit(EXIT_FAILURE);
    }

    m_p_soundFileHandlerArray = new SNDFILE*[m_nrOfInputFiles];
    m_p_soundFileInfoArray = new SF_INFO[m_nrOfInputFiles];
    m_p_gainFactorArray = new float[m_nrOfInputFiles];

    for (unsigned int i = 0; i < m_nrOfInputFiles; i++) {
        m_mixerInputRIFFWAVEFileNameList.push_back(inputFileNameArray[i]);
        m_p_gainFactorArray[i] = 1.0;
    }

    tryToOpenRIFFWAVEFiles();
    checkWaveFileConfiguration();

    PRINT_FORMATTED_INFO("init successful");
}


MixerApplication::~MixerApplication(void)
{
    PRINT_FORMATTED_INFO("delete internal data structures and close input files");

    //close input files
    PRINT_FORMATTED_INFO("close wave files...");
    for (unsigned int index = 0; index < m_mixerInputRIFFWAVEFileNameList.size(); ++index) {
        sf_close(m_p_soundFileHandlerArray[index]);
    }
}


void MixerApplication::checkWaveFileConfiguration(void) const
{
    //check if all files have the same configuration...
    //no sampling rate conversion at the moment...

    //check if data structures are initialized
    if (m_p_soundFileInfoArray == NULL) {
        PRINT_FORMATTED_ERROR("sound file data structures not ready...");
        exit(EXIT_FAILURE);
    }

    for (int unsigned fileIndex = 1; fileIndex < m_nrOfInputFiles; fileIndex++)
    {
        if (m_p_soundFileInfoArray[fileIndex].channels != 1) {
            PRINT_FORMATTED_ERROR("at the moment only one channel files are supported " << fileIndex);
            exit(EXIT_FAILURE);
        }
        if (m_p_soundFileInfoArray[0].format != m_p_soundFileInfoArray[fileIndex].format) {
            PRINT_FORMATTED_ERROR("format differ at file index 0 and " << fileIndex);
            exit(EXIT_FAILURE);
        }
        if (m_p_soundFileInfoArray[0].samplerate != m_p_soundFileInfoArray[fileIndex].samplerate) {
            PRINT_FORMATTED_ERROR("samplerate differ at file index 0 and " << fileIndex);
            exit(EXIT_FAILURE);
        }
    }

    PRINT_FORMATTED_INFO("sound file configuration (channels, format, samplerate) ok...");
}


void MixerApplication::tryToOpenRIFFWAVEFiles(void)
{
    PRINT_FORMATTED_INFO("try to open input wave files...");
    std::list<std::string>::const_iterator listIterator = m_mixerInputRIFFWAVEFileNameList.begin();
    for (unsigned int fileIndex = 0; fileIndex < m_mixerInputRIFFWAVEFileNameList.size(); ++fileIndex) {
        PRINT_FORMATTED_LIST_ELEMENT("open file " << *listIterator);
        m_p_soundFileHandlerArray[fileIndex] = sf_open((*listIterator).c_str(), SFM_READ, &m_p_soundFileInfoArray[fileIndex]);

        if (m_p_soundFileHandlerArray[fileIndex] == NULL) {
            PRINT_FORMATTED_ERROR("unable to open the wave file " << (*listIterator).c_str() << ": " << sf_strerror(m_p_soundFileHandlerArray[fileIndex]));
            exit(EXIT_FAILURE);
        }

        ++listIterator;
    }
}


void MixerApplication::tryToRewindRIFFWAVEFiles(void)
{
    PRINT_FORMATTED_INFO("try to rewind input wave files...");
    std::list<std::string>::const_iterator listIterator = m_mixerInputRIFFWAVEFileNameList.begin();
    for (unsigned int fileIndex = 0; fileIndex < m_mixerInputRIFFWAVEFileNameList.size(); ++fileIndex) {
        if (m_p_soundFileHandlerArray[fileIndex] == NULL)
        {
            PRINT_FORMATTED_LIST_ELEMENT("file " << *listIterator << " was NULL. this is not optimal, skip this one...");
            continue;
        }

        PRINT_FORMATTED_LIST_ELEMENT("rewind file " << *listIterator);
        sf_seek(m_p_soundFileHandlerArray[fileIndex], 0, SEEK_SET);
        ++listIterator;
    }
}


void MixerApplication::setStrategy(MixerAlgorithm* const mixerAlgorithm)
{
    if (mixerAlgorithm == NULL) {
        PRINT_FORMATTED_ERROR("null pointer strategy is bad...");
        exit(EXIT_FAILURE);
    }
    m_p_mixerAlgorithm = mixerAlgorithm;
}


void MixerApplication::mixRIFFWAVEFiles(const std::string& riffWaveMixFileName)
{
    //check if mixer strategy is set
    if (m_p_mixerAlgorithm == 0) {
        PRINT_FORMATTED_ERROR("mixer strategy not set!");
        return;
    }
    PRINT_FORMATTED_INFO("mixer strategy " << m_p_mixerAlgorithm->getAlgorithmIdentifier() << " selected");
    m_p_mixerAlgorithm->printAlgorithmConfiguration();

    tryToRewindRIFFWAVEFiles();

    //init sf info structure
    SF_INFO mixSfInfo;
    bzero(&mixSfInfo, sizeof(mixSfInfo));
    mixSfInfo.channels = m_p_soundFileInfoArray[0].channels;
    mixSfInfo.format = m_p_soundFileInfoArray[0].format;
    mixSfInfo.samplerate = m_p_soundFileInfoArray[0].samplerate;
    mixSfInfo.frames = getMaximumNrOfFramesInFiles();

    PRINT_FORMATTED_INFO("minimum number of frames in input files: " << getMinimumNrOfFramesInFiles());
    PRINT_FORMATTED_INFO("maximum number of frames in input files: " << getMaximumNrOfFramesInFiles());

    //open output file
    PRINT_FORMATTED_INFO("open mixer output file " << riffWaveMixFileName);
    SNDFILE* mixFileHandler = sf_open(riffWaveMixFileName.c_str(), SFM_WRITE, &mixSfInfo);
    if (mixFileHandler == NULL) {
        PRINT_FORMATTED_ERROR("unable to open audio out file: " << riffWaveMixFileName.c_str());
        exit(EXIT_FAILURE);
    }

    //get algorithm data element prototype
    const MixerAlgorithmDataElement& mixerAlgorithmDataElementPrototype = m_p_mixerAlgorithm->getMixerAlgorithmDataElementPrototype();

    //prepare dynamic buffers, allocate memory
    PRINT_FORMATTED_INFO("prepare internal 16bit buffers...");
    int16_t** const pp_inputSampleBufferArray = new int16_t*[m_nrOfInputFiles];
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i) {
        pp_inputSampleBufferArray[i] = new int16_t[m_p_soundFileInfoArray[0].channels];
    }
    int16_t* p_outputSampleBuffer = new int16_t[mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk()];

    PRINT_FORMATTED_INFO("mix " << getMaximumNrOfFramesInFiles() << " samples ";

    std::clock_t start;
    start = std::clock();
    for (uint32_t chunkIndex = 0; chunkIndex < (getMaximumNrOfFramesInFiles() / mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk()); ++chunkIndex) {
        //read the data from file to process data structure
        uint32_t nrOfReadItems;
        for (uint32_t i = 0; i < m_nrOfInputFiles; ++i) {
            //NOTE: write zero data sample if no data was read from input file
            nrOfReadItems = sf_read_short(m_p_soundFileHandlerArray[i], pp_inputSampleBufferArray[i], mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk());
            if (nrOfReadItems == 0) {
                //write zero data, 'sample size' times 'nr of samples per chunk'
                memset(pp_inputSampleBufferArray[i], 0, sizeof(int16_t) * mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk());
            }
        }

        //execute algorithm, mix the samples chunk by chunk
        //TODO: change api to something like in mpg123, feed samples and the algorithm does something if enough samples feeded...
        m_p_mixerAlgorithm->mixSamples(pp_inputSampleBufferArray, m_nrOfInputFiles, p_outputSampleBuffer);

        if (chunkIndex % 1024 == 0) {
            std::cout << ".";
        }

        //write mixed data to file
        sf_write_short(mixFileHandler, p_outputSampleBuffer, mixerAlgorithmDataElementPrototype.getNrOfSamplesPerChunk());
    }
    double mixDuration = ( std::clock() - start);
    std::cout << " done!");
    PRINT_FORMATTED_INFO("total mix duration in clocks: " << mixDuration);
    PRINT_FORMATTED_INFO("total mix duration in seconds: " << mixDuration / (double) CLOCKS_PER_SEC);

    //delete data structures
    PRINT_FORMATTED_INFO("delete internal buffers...");
    delete [] p_outputSampleBuffer;
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i) {
        delete [] pp_inputSampleBufferArray[i];
    }
    delete [] pp_inputSampleBufferArray;

    //close output file
    PRINT_FORMATTED_INFO("close mixer output file " << riffWaveMixFileName);
    sf_close(mixFileHandler);

    PRINT_FORMATTED_INFO("mixing done using algorithm " << m_p_mixerAlgorithm->getAlgorithmIdentifier());
}


inline uint32_t MixerApplication::getMinimumNrOfFramesInFiles(void) const
{
    uint32_t minimumNrOfFrames = std::numeric_limits<uint32_t>::max();
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i) {
        if (minimumNrOfFrames > m_p_soundFileInfoArray[i].frames) {
            minimumNrOfFrames = m_p_soundFileInfoArray[i].frames;
        }
    }

    return minimumNrOfFrames;
}


inline uint32_t MixerApplication::getMaximumNrOfFramesInFiles(void) const
{
    uint32_t maximumNrOfFrames = std::numeric_limits<uint32_t>::min();
    for (uint32_t i = 0; i < m_nrOfInputFiles; ++i) {
        if (maximumNrOfFrames < m_p_soundFileInfoArray[i].frames) {
            maximumNrOfFrames = m_p_soundFileInfoArray[i].frames;
        }
    }

    return maximumNrOfFrames;
}
