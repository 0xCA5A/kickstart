#include <iostream>
#include <sndfile.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "RIFFWaveFileShifter.hpp"



/**
 * @brief wave file shifter class
 * allows to prefix / shift input wave file (RIFF) data with zero samples.
 *
 * the code is tested with 16bit samples, 16khz and 32khz samplingrate
 * 
 * @param argc number of arguments from command line
 * @param argv command line argument vector
 */
RIFFWaveFileShifter::RIFFWaveFileShifter(int argc, char** argv)
    : m_applicationName(argv[0])
    , m_audioZeroDelayInMicroSeconds(0)
{
    std::cout << "[i] hello from " << m_applicationName << std::endl;
    parseCommandLineArguments(argc, argv);
}


void RIFFWaveFileShifter::shiftAudioFile(void)
{

    openInputFile();
    memcpy(&m_outputSfInfo, &m_inputSfInfo, sizeof(m_outputSfInfo));
    createOutputFile();

    //calculate how many zero samples to insert
    unsigned int numberOfBytesPerSample = 0;
    switch (m_outputSfInfo.format & 0x00000007) {
        case SF_FORMAT_PCM_S8: numberOfBytesPerSample = 1; break;
        case SF_FORMAT_PCM_16: numberOfBytesPerSample = 2; break;
        case SF_FORMAT_PCM_24: numberOfBytesPerSample = 3; break;
        case SF_FORMAT_PCM_32: numberOfBytesPerSample = 4; break;
    }
    if (!numberOfBytesPerSample) {
        std::cout << "[!] unable to determine audio format" << std::endl;
        exit(EXIT_FAILURE);
    }

    char sampleBuffer[__SIZE_OF_SAMPLE_BUFFER_IN_SAMPLES * numberOfBytesPerSample];

    unsigned int totalNumberOfWrittenSamples = 0;
    const unsigned int numberOfZeroSamplesToWrite = ((float)m_audioZeroDelayInMicroSeconds * m_outputSfInfo.samplerate * m_outputSfInfo.channels) / 1000 / 1000;
    const unsigned int numberOfZeroBytesToWrite = numberOfZeroSamplesToWrite * numberOfBytesPerSample;
    const unsigned int realAudioZeroDelayInMicroSeconds = (numberOfZeroBytesToWrite * 1000 * 1000) / (m_outputSfInfo.samplerate * m_outputSfInfo.channels * numberOfBytesPerSample);
    std::cout << "[i] audio format has " << numberOfBytesPerSample << " byte per sample" << std::endl;
    std::cout << "[i] audio format has a sampling rate of " << m_outputSfInfo.samplerate << "hz" << std::endl;
    std::cout << "[i] audio format has " << m_outputSfInfo.channels << " channels" << std::endl;
    std::cout << "[i] audio delay was defined as " << m_audioZeroDelayInMicroSeconds << "us" << std::endl;
    std::cout << "[i] write " << numberOfZeroBytesToWrite << " zero bytes to the audio file, " << realAudioZeroDelayInMicroSeconds << "us" << std::endl;

    sf_count_t nrOfWrittenSamples;
    memset(sampleBuffer, 0, __SIZE_OF_SAMPLE_BUFFER_IN_SAMPLES * numberOfBytesPerSample);

    const unsigned int nrOfFullZeroBufferWrites = numberOfZeroSamplesToWrite / __SIZE_OF_SAMPLE_BUFFER_IN_SAMPLES;
    const unsigned int nrOfIndividualZeroSampleWrites = numberOfZeroSamplesToWrite % __SIZE_OF_SAMPLE_BUFFER_IN_SAMPLES;
    for (unsigned int i = 0; i < nrOfFullZeroBufferWrites; i++) {
        nrOfWrittenSamples = sf_write_short(m_outputSoundFileHandler, (short*) sampleBuffer, __SIZE_OF_SAMPLE_BUFFER_IN_SAMPLES);
        totalNumberOfWrittenSamples += nrOfWrittenSamples;
        if (nrOfWrittenSamples != __SIZE_OF_SAMPLE_BUFFER_IN_SAMPLES) {
            std::cout << "[!] unable to write to audio output file " << m_outputFilePath << ", " << sf_strerror(m_outputSoundFileHandler) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    //write the rest
    nrOfWrittenSamples = sf_write_short(m_outputSoundFileHandler, (short*) sampleBuffer, nrOfIndividualZeroSampleWrites);
    totalNumberOfWrittenSamples += nrOfWrittenSamples;
    if (nrOfWrittenSamples != nrOfIndividualZeroSampleWrites) {
        std::cout << "[!] unable to write to audio output file " << m_outputFilePath << ", " << sf_strerror(m_outputSoundFileHandler) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "[i] copy input audio data to output file, " << m_inputSfInfo.frames << " frames, " << m_inputSfInfo.frames * 2 << " byte" << std::endl;

    sf_count_t nrOfReadSamples = !0;
    while (nrOfReadSamples != 0) {
        nrOfReadSamples = sf_read_short(m_inputSoundFileHandler, (short*) sampleBuffer, __SIZE_OF_SAMPLE_BUFFER_IN_SAMPLES);
        nrOfWrittenSamples = sf_write_short(m_outputSoundFileHandler, (short*) sampleBuffer, nrOfReadSamples);
        totalNumberOfWrittenSamples += nrOfWrittenSamples;
    }

    //check nr of samples in output file
    if (totalNumberOfWrittenSamples != m_inputSfInfo.frames + numberOfZeroSamplesToWrite) {
        std::cout << "[!] number of samples in output file not correct, have " << totalNumberOfWrittenSamples << " samples, " << m_inputSfInfo.frames + numberOfZeroSamplesToWrite << " samples expected" << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cout << "[i] " << totalNumberOfWrittenSamples << " samples written" << std::endl;
    }
}


RIFFWaveFileShifter::~RIFFWaveFileShifter()
{
    sf_close(m_inputSoundFileHandler);
    sf_close(m_outputSoundFileHandler);
}


void RIFFWaveFileShifter::createOutputFile(void)
{
    m_outputSoundFileHandler = sf_open(m_outputFilePath.c_str(), SFM_WRITE, &m_outputSfInfo);
    if (!m_outputSoundFileHandler) {
        std::cout << "[!] unable to create output sound file " << m_outputFilePath << ", " << sf_strerror(m_outputSoundFileHandler) << std::endl;
        exit(EXIT_FAILURE);
    }

}


void RIFFWaveFileShifter::showHowToAndExit(const std::string& applicationName) const
{
    std::cout << "[!] bad arguments, expect exactlty three arguments:" << std::endl;
    std::cout << applicationName << " -i <INPUT FILE PATH>" << " -z <INPUT ZERO DELAY IN US>" << " -o <OUTPUT FILE PATH>" << std::endl;
    exit(EXIT_SUCCESS);
}


void RIFFWaveFileShifter::parseCommandLineArguments(int argc, char** argv)
{
    //check number of arguments
    if (argc != 7) {
        showHowToAndExit(argv[0]);
    }

    //check command line arguments using getopt
    int c;
    while ((c = getopt(argc, argv, "i:z:o:")) != -1) {
        switch (c) {
            case 'i':
                m_inputFilePath.assign(optarg, strlen(optarg));
                break;
            case 'z':
                m_audioZeroDelayInMicroSeconds = atoi(optarg);
                if (m_audioZeroDelayInMicroSeconds <= 0)
                {
                    showHowToAndExit(m_applicationName);
                }
                break;
            case 'o':
                m_outputFilePath.assign(optarg, strlen(optarg));
                break;
            default:
                showHowToAndExit(m_applicationName);
        }
    }

    if (m_inputFilePath.size() == 0 || m_outputFilePath.size() == 0 || m_audioZeroDelayInMicroSeconds == 0) {
        showHowToAndExit(argv[0]);
    }
}


void RIFFWaveFileShifter::openInputFile(void)
{
    m_inputSoundFileHandler = sf_open(m_inputFilePath.c_str(), SFM_READ, &m_inputSfInfo);
    if (!m_inputSoundFileHandler) {
        std::cout << "[!] unable to open input sound file " << m_inputFilePath << ", " << sf_strerror(m_outputSoundFileHandler) << std::endl;
        exit(EXIT_FAILURE);
    }
}

