#include "JSONAnalyzer.h"

#include <stdlib.h>
#include <jsoncpp/json/reader.h>





int JSONAnalyzer::parseJSONData(const std::string& jsonDataString, unsigned int& jsonXPosValue, unsigned int& jsonYPosValue, bool& jsonKey1pressedValue, bool& jsonKey2pressedValue)
{

    //http://jsoncpp.sourceforge.net/
    //check here for further json details...

    // expected data
    // {
    //   "xPos": 123,
    //   "yPos": 456,
    //   "key1pressed": true,
    //   "key2pressed": false
    // }

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonDataString, root);
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "[!] failed to parse JSON data, drop data..." << reader.getFormattedErrorMessages() << std::endl;
        return EXIT_FAILURE;
    }

    //get the value of the member of root named 'xPos', return '-1' if there is no such member
    jsonXPosValue = root.get("xPos", "-1").asInt();
    jsonYPosValue = root.get("yPos", "-1").asInt();
    jsonKey1pressedValue = root.get("key1pressed", false).asBool();
    jsonKey2pressedValue = root.get("key2pressed", false).asBool();

//     cout << "xpos value: " << xPosValue << endl;
//     cout << "ypos value: " << yPosValue << endl;
//     cout << "key1pressedValue: " << key1pressedValue << endl;
//     cout << "key2pressedValue: " << key2pressedValue << endl;

    return EXIT_SUCCESS;
}





