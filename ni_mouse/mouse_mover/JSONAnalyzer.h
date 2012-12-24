#ifndef _JSONANALYZER_H_
#define _JSONANALYZER_H_

#include <iostream>

class JSONAnalyzer
{
    public:
//         JSONAnalyzer(const std::string& sUdnName)
//         : m_iNrOfAliveXmlRpcCallTotal(0)
//         , m_iNrOfAliveXmlRpcCallSuccessfully(0)
//         , m_iNrOfAliveXmlRpcCallFailed(0)
//         , m_iNrOfRestartedApplication(0)
//         , m_bFirstAliveCall(true)
//         , m_sUdnName(sUdnName)
//         {};

//         JSONAnalyzer();

//         AliveDeviceResult(const AliveDeviceResult& rhs);
//         AliveDeviceResult& operator=(const AliveDeviceResult& rhs);
// 
//         inline const std::string& getUdnName() const
//         { return m_sUdnName; }
// 
//         inline void clearResult()
//         {
//             m_iNrOfAliveXmlRpcCallTotal = 0;
//             m_iNrOfAliveXmlRpcCallSuccessfully = 0;
//             m_iNrOfAliveXmlRpcCallFailed = 0;
//             m_iNrOfRestartedApplication = 0;
//             m_bFirstAliveCall = true;
//         };

            int parseJSONData(const std::string& jsonDataString, int& jsonDXPosValue, int& jsonDYPosValue, bool& jsonKey1pressedValue, bool& jsonKey2pressedValue);


    public:

//         int  m_iNrOfAliveXmlRpcCallTotal;
//         int  m_iNrOfAliveXmlRpcCallSuccessfully;
//         int  m_iNrOfAliveXmlRpcCallFailed;
//         int  m_iNrOfRestartedApplication;
//         bool m_bFirstAliveCall;
    private:
//         std::string m_sUdnName;
};

#endif