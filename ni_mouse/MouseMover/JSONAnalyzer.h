#ifndef _JSONANALYZER_H_
#define _JSONANALYZER_H_

#include <iostream>

class JSONAnalyzer
{
    public:
        JSONAnalyzer();
        ~JSONAnalyzer();

        int parseJSONData(const std::string& jsonDataString, int& jsonDXPosValue, int& jsonDYPosValue, bool& jsonKey1pressedValue, bool& jsonKey2pressedValue);

    public:
    private:
};

#endif