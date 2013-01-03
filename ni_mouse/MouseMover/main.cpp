
#include "main.h"
#include "Debug.h"
#include "UinputCommander.h"
#include "JSONAnalyzer.h"
// #include "AbstractSocketServer.h"
#include "UnixDomainSocketServer.h"
#include "HelperStuff.h"
#include "MouseMover.h"

#include <signal.h>
#include <stdlib.h>





//FIXME: move this variable away from global space...
// i do not understand why the destructor is called automatically if the objects are
// defined here...
std::string uinputUNIXDeviceName(UINPUT_UNIX_DEVICE_NAME);
std::string uinputName(UINPUT_NAME);
UinputCommander myUinputCommander(uinputUNIXDeviceName, uinputName, MAX_SCREEN_X_RESOLUTION, MAX_SCREEN_Y_RESOLUTION);

UnixDomainSocketServer myUnixDomainSocketServer(UNIX_DOMAIN_SOCKET_PATH);
// MouseMover myMouseMover();






static void signalHandler(int signalNumber)
{
    printFunctonNameMacro();
    std::cout << "\n[i] caught signal number: " << signalNumber << std::endl;

//     not necessary if uinputcommander is defined global...
//     uinputCommander.~UinputCommander();

//     myMouseMover.~MouseMover();

    exit(EXIT_SUCCESS);
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

    bool runFlag = true;

//     std::string uinputDeviceName(UINPUT_DEVICE_NAME);
//     UinputCommander uinputCommander(uinputDeviceName);

    //FIXME: can not call my own constructor...
    AbstractSocketServer* p_mySocketServer = &myUnixDomainSocketServer;

    char dataBuffer[SOCKET_DATA_BUFFER_MAX_LENGTH];

//     JSONAnalyzer myJSONAnalyzer;
    int dxPosValue;
    int dyPosValue;
    bool key1pressedValue;
    bool key2pressedValue;
    bool lastClickLeftState = false;
    bool lastClickRightState = false;

    JSONAnalyzer myJSONAnalyzer;


    registerSignals();
    p_mySocketServer->openSocket();

    int returnValue = 0;


    std::cout << "[i] wait for client connection..." << std::endl;
    while(runFlag)
    {

        p_mySocketServer->receiveData(dataBuffer, sizeof(dataBuffer));
        std::string jsonDataString(dataBuffer);
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






