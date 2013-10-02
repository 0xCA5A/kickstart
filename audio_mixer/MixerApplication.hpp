#ifndef MIXERAPPLICATION_HPP
#define MIXERAPPLICATION_HPP

#include <list>

#include "MixerAlgorithm.hpp"


class MixerApplication
{
public:
    MixerApplication(void);
    ~MixerApplication(void);
    void setStrategy(MixerAlgorithm* mixerAlgorithm);
    void mixRIFFWAVEFiles(const std::list<std::string>& riffWaveFileNameList, const std::string& riffWaveMixFileName);

private:
    MixerApplication(const MixerApplication&);
    MixerApplication& operator=(const MixerApplication&);

private:
    MixerAlgorithm* m_p_mixerAlgorithm;
};

#endif 