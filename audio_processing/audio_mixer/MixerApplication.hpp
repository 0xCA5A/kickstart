#ifndef MIXERAPPLICATION_HPP
#define MIXERAPPLICATION_HPP

#include <stdint.h>
#include <list>
#include <sndfile.h>

#include "MixerAlgorithm.hpp"


class MixerApplication
{
public:
    MixerApplication(unsigned int numberOfInputFiles, char* inputFileNameArray[]);
    ~MixerApplication(void);
    void setStrategy(MixerAlgorithm* const mixerAlgorithm);
    void mixRIFFWAVEFiles(const std::string& riffWaveMixFileName);
    const uint32_t getNrOfInputFiles(void) const {return m_nrOfInputFiles;};

private:
    MixerApplication(const MixerApplication&);
    MixerApplication& operator=(const MixerApplication&);
    void tryToOpenRIFFWAVEFiles(void);
    void tryToRewindRIFFWAVEFiles(void);
    void checkWaveFileConfiguration(void) const;
    inline uint32_t getMaximumNrOfFramesInFiles(void) const;
    inline uint32_t getMinimumNrOfFramesInFiles(void) const;

private:
    MixerAlgorithm* m_p_mixerAlgorithm;
    SF_INFO* m_p_soundFileInfoArray;
    SNDFILE** m_p_soundFileHandlerArray;
    uint32_t m_nrOfInputFiles;
    float* m_p_gainFactorArray;
    std::list<std::string> m_mixerInputRIFFWAVEFileNameList;
};

#endif