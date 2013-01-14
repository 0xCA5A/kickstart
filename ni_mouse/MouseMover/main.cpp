#include "main.h"
#include "Debug.h"
#include "UinputCommander.h"
#include "JSONAnalyzer.h"
#include "AbstractSocketServer.h"
#include "UnixDomainSocketServer.h"
#include "HelperStuff.h"

#include <signal.h>
#include <stdlib.h>



// global stuff
bool runFlag = true;



static void signalHandler(int signalNumber)
{
    printFunctonNameMacro();

    std::cout << "\n[i] caught signal number: " << signalNumber << std::endl;
    runFlag = false;
}



void registerSignals()
{
    printFunctonNameMacro();

    if(signal(SIGINT, signalHandler) == SIG_ERR)
    {
        std::cout << "[!] error registering SIGINT signal handler";
        exit(EXIT_FAILURE);
    }
    if(signal(SIGQUIT, signalHandler) == SIG_ERR)
    {
        std::cout << "[!] error registering SIGQUIT signal handler" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(signal(SIGABRT, signalHandler) == SIG_ERR)
    {
        std::cout << "[!] error registering SIGABRT signal handler" << std::endl;
        exit(EXIT_FAILURE);
    }
//     if(signal(SIGKILL, signalHandler) == SIG_ERR)
//     {
//         std::cout << "[!] error registering SIGKILL signal handler" << std::endl;
//         exit(EXIT_FAILURE);
//     }

}



int main()
{
    printFunctonNameMacro();

    std::string uinputUNIXDeviceName(UINPUT_UNIX_DEVICE_NAME);
    std::string uinputName(UINPUT_NAME);
    UinputCommander myUinputCommander(uinputUNIXDeviceName, uinputName, MAX_SCREEN_X_RESOLUTION, MAX_SCREEN_Y_RESOLUTION);

    unsigned int selectTimeoutValueInSeconds = 1;
    UnixDomainSocketServer myUnixDomainSocketServer(selectTimeoutValueInSeconds, UNIX_DOMAIN_SOCKET_PATH);
    AbstractSocketServer* p_mySocketServer = &myUnixDomainSocketServer;

    JSONAnalyzer myJSONAnalyzer;


    char socketDataBuffer[SOCKET_DATA_BUFFER_MAX_LENGTH];
    int dxPosValue;
    int dyPosValue;
    bool key1pressedValue;
    bool key2pressedValue;
    bool lastClickLeftState = false;
    bool lastClickRightState = false;


    registerSignals();

    p_mySocketServer->openSocketNonBlocking();


    std::cout << "[i] wait for client connection, enter receive loop..." << std::endl;
    int returnValue = 0;
    while(runFlag)
    {

        //get date from socket
        returnValue = p_mySocketServer->receiveDataNonBlocking(socketDataBuffer, sizeof(socketDataBuffer));

        //something went wrong... exit gracefully...
        if (returnValue < 0)
        {
            runFlag = false;
            continue;
        }

        //select timeout
        if (returnValue == 0)
        {
            continue;
        }

        std::string jsonDataString(socketDataBuffer);
        returnValue = myJSONAnalyzer.parseJSONData(jsonDataString, dxPosValue, dyPosValue, key1pressedValue, key2pressedValue);
        if (returnValue == EXIT_FAILURE)
        {
            continue;
        }


        myUinputCommander.incrementXYPosition(dxPosValue, dyPosValue);


        //detect left click
        if ((lastClickLeftState == false) && (key1pressedValue == true))
        {
            myUinputCommander.clickLeft();
            debug() << "[i] " << __func__ << " : click left" << std::endl;
            lastClickLeftState = key1pressedValue;

        }
        else if ((lastClickLeftState == true) && (key1pressedValue == false))
        {
            myUinputCommander.releaseLeft();
            debug() << "[i] " << __func__ << " : release left" << std::endl;
            lastClickLeftState = key1pressedValue;
        }

        //detect right click
        if ((lastClickRightState == false) && (key2pressedValue == true))
        {
            myUinputCommander.clickLeft();
            debug() << "[i] " << __func__ << " : click right" << std::endl;
            lastClickRightState = key2pressedValue;

        }
        else if ((lastClickRightState == true) && (key2pressedValue == false))
        {
            myUinputCommander.releaseLeft();
            debug() << "[i] " << __func__ << " : release right" << std::endl;
            lastClickRightState = key2pressedValue;
        }


    }


    return 0;

}






