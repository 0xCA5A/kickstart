#ifndef RIFFWAVEFILESHIFTER_HPP
#define RIFFWAVEFILESHIFTER_HPP

#include <sndfile.h>

class RIFFWaveFileShifter
{
public:
    RIFFWaveFileShifter(int argc, char** argv);
    ~RIFFWaveFileShifter();
    void createOutputFile(const char* outputFileName);
    void showHowToAndExit(const std::string& applicationName);
    void processAudioFile(void);
    
private:
    RIFFWaveFileShifter(const RIFFWaveFileShifter&);
    RIFFWaveFileShifter& operator=(const RIFFWaveFileShifter&);
    void parseCommandLineArguments(int argc, char** argv);
    void openInputFile(void);

private:
    const std::string m_applicationName;
    std::string m_inputFilePath;
    std::string m_outputFilePath;
    int m_audioZeroDelayInMilliseconds;
    SNDFILE* m_inputSoundFileHandler;
    SNDFILE* m_outputSoundFileHandler;
    SF_INFO m_inputSfInfo;
    SF_INFO m_outputSfInfo;
};

#endif