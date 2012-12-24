
#include "main.h"
#include "UinputCommander.h"
#include "JSONAnalyzer.h"
// #include "AbstractSocketServer.h"
#include "UnixDomainSocketServer.h"


#include <signal.h>
#include <stdlib.h>



//FIXME: move this variable away from global space...
std::string uinputDeviceName(UINPUT_DEVICE_NAME);
UinputCommander myUinputCommander(uinputDeviceName);

UnixDomainSocketServer myUnixDomainSocketServer;
JSONAnalyzer myJSONAnalyzer;

static void signalHandler(int signalNumber)
{
    std::cout << "\n[i] caught signal number: " << signalNumber << std::endl;

//     not necessary if uinputcommander is defined global...
//     uinputCommander.~UinputCommander();

    exit(EXIT_SUCCESS);
}


void registerSignals()
{
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
    bool runFlag = true;

//     string uinputDeviceName(UINPUT_DEVICE_NAME);
//     UinputCommander uinputCommander(uinputDeviceName);

//     UnixDomainSocketServer myUnixDomainSocketServer;
    AbstractSocketServer* p_mySocketServer = &myUnixDomainSocketServer;
    char dataBuffer[SOCKET_DATA_BUFFER_MAX_LENGTH];

//     JSONAnalyzer myJSONAnalyzer;
    int dxPosValue;
    int dyPosValue;
    bool key1pressedValue;
    bool key2pressedValue;
    bool lastClickLeftState = false;
    bool lastClickRightState = false;


    registerSignals();
    p_mySocketServer->setupAndOpenSocket();


    while(runFlag)
    {

        p_mySocketServer->receiveData(dataBuffer, sizeof(dataBuffer));
//         try{
        std::string jsonDataString(dataBuffer);
//         std::cout << "got data:" << jsonDataString << std::endl;
//         }
//         catch
//         {
//
//         }
//         std::cout << "string size: " << jsonDataString.size() << std::endl;
        myJSONAnalyzer.parseJSONData(jsonDataString, dxPosValue, dyPosValue, key1pressedValue, key2pressedValue);


//         std::cout << "dxpos value: " << dxPosValue << std::endl;
//         std::cout << "dypos value: " << dyPosValue << std::endl;
//         std::cout << "key1pressedValue: " << key1pressedValue << std::endl;
//         std::cout << "key2pressedValue: " << key2pressedValue << std::endl;


//         dx = dxPosValue - oldXPosValue;
//         dy = dyPosValue - oldYPosValue;
//         oldXPosValue = dxPosValue;
//         oldYPosValue = dyPosValue;

        myUinputCommander.incrementXYPosition(dxPosValue, dyPosValue);

        //detect left click
        std::cout << "key1pressedValue : " << key1pressedValue << std::endl;
        std::cout << "lastClickLeftState : " << lastClickLeftState << std::endl;
//         if (lastClickLeftState != key1pressedValue)
//         {
// 
//             std::cout << "  state changed" << std::endl;
// 
//             
// 
// 
//             
//             lastClickLeftState = key1pressedValue;
//         }

        //detect left click
        if ((lastClickLeftState == false) && (key1pressedValue == true))
        {
            myUinputCommander.clickLeft();
            std::cout << "[i] " << __func__ << " : click left" << std::endl;
            lastClickLeftState = key1pressedValue;

        }
        else if ((lastClickLeftState == true) && (key1pressedValue == false))
        {
            myUinputCommander.releaseLeft();
            std::cout << "[i] " << __func__ << " : release left" << std::endl;
            lastClickLeftState = key1pressedValue;
        }

        //detect right click
        if ((lastClickRightState == false) && (key2pressedValue == true))
        {
            myUinputCommander.clickLeft();
            std::cout << "[i] " << __func__ << " : click right" << std::endl;
            lastClickRightState = key2pressedValue;

        }
        else if ((lastClickRightState == true) && (key2pressedValue == false))
        {
            myUinputCommander.releaseLeft();
            std::cout << "[i] " << __func__ << " : release right" << std::endl;
            lastClickRightState = key2pressedValue;
        }
//         


        
//         if (lastClickLeftState != key1pressedValue)
//         {
//             if (key1pressedValue == true)
//             {
//         //       clickLeft();
//                 myUinputCommander.clickLeft();
//                 std::cout << "click left" << std::endl;
//                  
//             }
//             else
//             {
//                 myUinputCommander.releaseLeft();
// 
//                 std::cout << "release left" << std::endl;
//         //       releaseLeft();
//             }
// 
//         }


        
//         if ((key1pressedValue != lastKey1pressedValue) && key1pressedValue)
//         {
//             doLeftClickFlag = true;
//         }
// //         if (!key1pressedValue && doLeftReleaseFlag == false)
// //         {
// //             doLeftClickFlag = true;
// //         }
// //         
//         if (doLeftReleaseFlag)
//         {
//             doLeftClickFlag = false;
//         }
// 
//         if (doLeftClickFlag)
//         {
//             myUinputCommander.clickLeft();
//         }
//         if (doLeftReleaseFlag)
//         {
//             myUinputCommander.clickLeft();
//         }
    }




    
//     std::string jsonDataString =  "{ \"xPos\": 123, \"yPos\": 456, \"key1pressed\": true, \"key2pressed\": false }";
//     myJSONAnalyzer.parseJSONData(jsonDataString, xPosValue, yPosValue, key1pressedValue, key2pressedValue);


//     jsonDataString =  "{ \"xPos\": 777, \"yPos\": 789, \"key1pressed\": false, \"key2pressed\": true }";
//     myJSONAnalyzer.parseJSONData(jsonDataString, xPosValue, yPosValue, key1pressedValue, key2pressedValue);
//     std::cout << "xpos value: " << xPosValue << std::endl;
//     std::cout << "ypos value: " << yPosValue << std::endl;
//     std::cout << "key1pressedValue: " << key1pressedValue << std::endl;
//     std::cout << "key2pressedValue: " << key2pressedValue << std::endl;


    return 2;



    int i = 0;

//     sleep(1);
//     cout << "move.to.center position" << endl;
//     myUinputCommander.moveToCenterPosition();
    sleep(2);
    std::cout << "XX incrment x and y, schräg" << std::endl;
    for (i = 0; i < 10; i++)
    {
//         myUinputCommander.decrementXPosition();
//         myUinputCommander.decrementYPosition();
        usleep(30000);
        myUinputCommander.incrementXPosition();
        myUinputCommander.incrementYPosition(i);
    }

    sleep(3);

    std::cout << "XX decrement x and y, schräg" << std::endl;

    for (i = 0; i < 10; i++)
    {
//         myUinputCommander.decrementXPosition();
//         myUinputCommander.decrementYPosition();
        usleep(30000);
        myUinputCommander.decrementXPosition();
        myUinputCommander.decrementYPosition(i);
    }

    sleep(3);

    std::cout << "move to center position" << std::endl;
    myUinputCommander.moveToCenterPosition();

    sleep(2);

//     cout << "move move to absolute position " << endl;
//     myUinputCommander.updatePositionAbsolute(200, 200);

    sleep(2);

    std::cout << "increment" << std::endl;
    myUinputCommander.incrementXPosition();
    sleep(1);
    myUinputCommander.incrementXPosition();
    sleep(1);
    myUinputCommander.incrementXPosition();
    sleep(1);
    myUinputCommander.incrementXPosition();
    sleep(1);
    myUinputCommander.incrementXPosition();
    sleep(1);
    myUinputCommander.incrementXPosition();


    for (i = 0; i < 10; i++)
    {
//         myUinputCommander.decrementXPosition();
//         myUinputCommander.decrementYPosition();
        usleep(30000);
        myUinputCommander.incrementXYPosition(i, i);
    }

    sleep(1);


    for (i = 0; i < 10; i++)
    {
//         myUinputCommander.decrementXPosition();
//         myUinputCommander.decrementYPosition();
        usleep(30000);
        myUinputCommander.decrementXYPosition(i, i);
    }

    sleep(1);


//     myUinputCommander.releaseLeft();

    for (i = 0; i < 10; i++)
    {
//         myUinputCommander.decrementXPosition();
//         myUinputCommander.decrementYPosition();
        usleep(30000);
//         myUinputCommander.updatePositionRelative(i, i);
    }

    return 0;


}






