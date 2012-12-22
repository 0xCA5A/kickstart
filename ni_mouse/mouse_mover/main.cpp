
#include "main.h"
#include "UinputCommander.h"

#define UINPUT_DEVICE_NAME "/dev/uinput"
// #define UINPUT_DEVICE_NAME "/dev/input/uinput"


int main()
{

    UinputCommander uinputCommander;


    string uinputDeviceName(UINPUT_DEVICE_NAME);
    uinputCommander.openDevice(uinputDeviceName);


    return 0;

    
}






