#include <iostream>
#include <sndfile.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "RIFFWaveFileShifter.hpp"



void RIFFWaveFileShifter::showHowToAndExit(const std::string& applicationName)
{
    std::cout << "[!] bad arguments, expect exactlty three arguments:" << std::endl;
    std::cout << applicationName << " -i <INPUT FILE PATH>" << " -z <INPUT ZERO DELAY>" << " -o <OUTPUT FILE PATH>" << std::endl;
    exit(EXIT_SUCCESS);
}


void RIFFWaveFileShifter::parseCommandLineArguments(int argc, char** argv)
{
    //check command line arguments using getopt
    int c;
    while ((c = getopt(argc, argv, "i:z:o:")) != -1)
    switch (c)
    {
        case 'i':
            m_inputFilePath.assign(optarg, strlen(optarg));
            break;
        case 'z':
            m_audioZeroDelayInMilliseconds = atoi(optarg);
            if (m_audioZeroDelayInMilliseconds <= 0)
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

void RIFFWaveFileShifter::openInputFile(void)
{
    m_inputSoundFileHandler = sf_open(m_inputFilePath.c_str(), SFM_READ, &m_inputSfInfo);
    if (!m_inputSoundFileHandler)
    {
        std::cout << "[!] unable to open sound file " << m_inputFilePath << std::endl;
        exit(EXIT_FAILURE);
    }
}



RIFFWaveFileShifter::RIFFWaveFileShifter(int argc, char** argv)
    : m_applicationName(argv[0])
{
    parseCommandLineArguments(argc, argv);
}

void RIFFWaveFileShifter::processAudioFile(void)
{

    openInputFile();
    createOutputFile("shifted.wav");

    memcpy(&m_outputSfInfo, &m_inputSfInfo, sizeof(m_outputSfInfo));

}

RIFFWaveFileShifter::~RIFFWaveFileShifter()
{
    sf_close(m_inputSoundFileHandler);
    sf_close(m_outputSoundFileHandler);
}

void RIFFWaveFileShifter::createOutputFile(const char* outputFileName)
{
    m_outputSoundFileHandler = sf_open(outputFileName, SFM_WRITE, &m_outputSfInfo);
    if (!m_outputSoundFileHandler)
    {
        std::cout << "[!] unable to open sound file " << outputFileName << std::endl;
        exit(EXIT_FAILURE);
    }


    
//     {
//         printf ("Not able to open output file %s.\n", outfilename) ;
//         puts (sf_strerror (NULL)) ;
//         return 1 ;
}

