#ifndef MIXERAPPLICATION_HPP
#define MIXERAPPLICATION_HPP

#include <list>
#include <sndfile.h>

#include "MixerAlgorithm.hpp"


class MixerApplication
{
public:
    MixerApplication(unsigned int numberOfInputFiles, char* inputFileNameArray[]);
    ~MixerApplication(void);
    void setStrategy(MixerAlgorithm* mixerAlgorithm);
    void mixRIFFWAVEFiles(const std::list<std::string>& riffWaveFileNameList, const std::string& riffWaveMixFileName);
//     static bool checkIfFileIsReadable(const char* riffWaveMixFileName);
//     static bool checkIfFileIsRIFFWAVEFile(const char* riffWaveMixFileName);
    bool tryToOpenRIFFWAVEFiles(const unsigned int nrOfFiles, char** fileList);

private:
    MixerApplication(const MixerApplication&);
    MixerApplication& operator=(const MixerApplication&);

private:
    MixerAlgorithm* m_p_mixerAlgorithm;
    SF_INFO m_soundFileInfoArray[];
    SNDFILE* m_soundFileHandlerArray[];
    char* m_soundFileNameArray[];
};

#endif 