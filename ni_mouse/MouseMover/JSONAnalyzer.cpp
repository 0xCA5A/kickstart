#include "JSONAnalyzer.h"
#include "HelperStuff.h"
#include "Debug.h"

#include <stdlib.h>
#include <jsoncpp/json/reader.h>



JSONAnalyzer::JSONAnalyzer()
{
    printFunctonNameMacro();
}


JSONAnalyzer::~JSONAnalyzer()
{
    printFunctonNameMacro();
}


int JSONAnalyzer::parseJSONData(const std::string& jsonDataString, int& jsonDXPosValue, int& jsonDYPosValue, bool& jsonKey1PressedValue, bool& jsonKey2PressedValue)
{
    printFunctonNameMacro();


    //http://jsoncpp.sourceforge.net/
    //check here for further json details...

    // expected data
    // {
    //   "dxPos": 123,
    //   "dyPos": 456,
    //   "key1pressed": true,
    //   "key2pressed": false
    // }


    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonDataString, root);
    if (!parsingSuccessful)
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "[!] failed to parse JSON data, drop complete data...\n" << reader.getFormattedErrorMessages() << std::endl;
//         jsonDXPosValue = 0;
//         jsonDYPosValue = 0;
//         jsonKey1PressedValue = false;
//         jsonKey2PressedValue = false;

        return EXIT_FAILURE;
    }

    //get the value of the member of root named 'xPos', return '-1' if there is no such member
    jsonDXPosValue = root.get("dxPos", "0").asInt();
    jsonDYPosValue = root.get("dyPos", "0").asInt();
    jsonKey1PressedValue = root.get("key1pressed", false).asBool();
    jsonKey2PressedValue = root.get("key2pressed", false).asBool();

#ifdef DEBUG
    std::cout << "jsonDXPosValue value: " << jsonDXPosValue << std::endl;
    std::cout << "jsonDYPosValue value: " << jsonDYPosValue << std::endl;
    std::cout << "jsonKey1PressedValue: " << jsonKey1PressedValue << std::endl;
    std::cout << "jsonKey2PressedValue: " << jsonKey2PressedValue << std::endl;
#endif

    return EXIT_SUCCESS;
}

