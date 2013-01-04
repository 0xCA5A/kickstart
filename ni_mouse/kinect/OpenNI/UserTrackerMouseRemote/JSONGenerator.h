#ifndef _JSONGENERATOR_H_
#define _JSONGENERATOR_H_

#include <iostream>

class JSONGenerator
{
    public:
        JSONGenerator();
        ~JSONGenerator();

        int generateJSONData(std::string& jsonDataString, int& jsonDXPosValue, int& jsonDYPosValue, bool& jsonKey1pressedValue, bool& jsonKey2pressedValue);

    public:
    private:
        unsigned int m_lastPlayerXCoord;
        unsigned int m_lastPlayerYCoord;
};

#endif