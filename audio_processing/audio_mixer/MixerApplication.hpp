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
    void mixRIFFWAVEFiles(const std::string& riffWaveMixFileName);
//     static bool checkIfFileIsReadable(const char* riffWaveMixFileName);
//     static bool checkIfFileIsRIFFWAVEFile(const char* riffWaveMixFileName);


private:
    MixerApplication(const MixerApplication&);
    MixerApplication& operator=(const MixerApplication&);
    bool tryToOpenRIFFWAVEFiles(void);
    
private:
    MixerAlgorithm* m_p_mixerAlgorithm;
    SF_INFO* m_p_soundFileInfoArray;
    SNDFILE** m_p_soundFileHandlerArray;
    char* m_soundFileNameArray[];

    std::list<std::string> m_mixerInputRIFFWAVEFileList;

};

#endif 