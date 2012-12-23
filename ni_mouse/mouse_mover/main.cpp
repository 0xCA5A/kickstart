
#include "main.h"
#include "UinputCommander.h"
#include "JSONAnalyzer.h"

#include <signal.h>
#include <stdlib.h>



//FIXME: move this variable away from global space...
std::string uinputDeviceName(UINPUT_DEVICE_NAME);
UinputCommander myUinputCommander(uinputDeviceName);




static void signalHandler(int signalNumber)
{
    std::cout << "\n[i] caught signal number: " << signalNumber << std::endl;

//     not necessary if uinputcommander is defined global...
//     uinputCommander.~UinputCommander();

    exit(EXIT_SUCCESS);
}

int main()
{


//     string uinputDeviceName(UINPUT_DEVICE_NAME);
//     UinputCommander uinputCommander(uinputDeviceName);


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


    unsigned int xPosValue;
    unsigned int yPosValue;

    bool key1pressedValue;
    bool key2pressedValue;

    JSONAnalyzer myJSONAnalyzer;

    std::string jsonDataString =  "{ \"xPos\": 123, \"yPos\": 456, \"key1pressed\": true, \"key2pressed\": false }";
    myJSONAnalyzer.parseJSONData(jsonDataString, xPosValue, yPosValue, key1pressedValue, key2pressedValue);
    std::cout << "xpos value: " << xPosValue << std::endl;
    std::cout << "ypos value: " << yPosValue << std::endl;
    std::cout << "key1pressedValue: " << key1pressedValue << std::endl;
    std::cout << "key2pressedValue: " << key2pressedValue << std::endl;

    jsonDataString =  "{ \"xPos\": 777, \"yPos\": 789, \"key1pressed\": false, \"key2pressed\": true }";
    myJSONAnalyzer.parseJSONData(jsonDataString, xPosValue, yPosValue, key1pressedValue, key2pressedValue);
    std::cout << "xpos value: " << xPosValue << std::endl;
    std::cout << "ypos value: " << yPosValue << std::endl;
    std::cout << "key1pressedValue: " << key1pressedValue << std::endl;
    std::cout << "key2pressedValue: " << key2pressedValue << std::endl;


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






