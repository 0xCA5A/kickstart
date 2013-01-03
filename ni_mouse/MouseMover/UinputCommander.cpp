
#include "HelperStuff.h"
#include "Debug.h"

#include "UinputCommander.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <linux/input.h>
#include <linux/uinput.h>


// TODO
// * check if usleep in constructor is necessary
// * do you want to move cursor to center position in constructor






UinputCommander::UinputCommander(const std::string& uinputUNIXDeviceName, const std::string& uinputName, const unsigned int maxScreenResolutionX, const unsigned int maxScreenResolutionY) :
m_maxScreenResolutionX(maxScreenResolutionX),
m_maxScreenResolutionY(maxScreenResolutionY)
{
    printFunctonNameMacro();
    openDevice(uinputUNIXDeviceName);
    configureDevice(uinputName);

    usleep(5000);
    moveToCenterPosition();
}


UinputCommander::~UinputCommander()
{
    printFunctonNameMacro();

    std::cout << "[i] release mouse buttons" << std::endl;
    releaseLeft();
    releaseRight();

    moveToCenterPosition();

    closeDevice();
}


void UinputCommander::openDevice(const std::string& uinputUNIXDeviceName)
{
    printFunctonNameMacro();
    std::cout << "[i] open uinput device " << uinputUNIXDeviceName << std::endl;

    m_openedUinputDeviceFileDescriptor = open(uinputUNIXDeviceName.c_str(), O_WRONLY | O_NONBLOCK);
    if(m_openedUinputDeviceFileDescriptor < 0)
    {
        die("[!] error in open");
    }
}


void UinputCommander::configureDevice(const std::string& uinputName)
{
    printFunctonNameMacro();
    std::cout << "[i] configure uinput device " << std::endl;

    if (m_openedUinputDeviceFileDescriptor == 0)
    {
        die("[!] error, uinput device not opened (fd is 0)");
    }

    //key stuff
    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_SET_EVBIT, EV_KEY) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_SET_KEYBIT, BTN_LEFT) < 0)
    {
        die("[i] error in ioctl");
    }
    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_SET_EVBIT, EV_KEY) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_SET_KEYBIT, BTN_RIGHT) < 0)
    {
        die("[i] error in ioctl");
    }

    //relative movement stuff
    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_SET_EVBIT, EV_REL) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_SET_RELBIT, REL_X) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_SET_RELBIT, REL_Y) < 0)
    {
        die("[i] error in ioctl");
    }

    //absolute movement stuff, didnt get it to work...
//     if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_EVBIT, EV_ABS) < 0)
//     {
//         die("[i] error in ioctl");
//     }
// 
//     if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_ABSBIT, ABS_X) < 0)
//     {
//         die("[i] error in ioctl");
//     }
// 
//     if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_ABSBIT, ABS_Y) < 0)
//     {
//         die("[i] error in ioctl");
//     }


    // check string size to avoid char array overflow
    if (uinputName.size() >= UINPUT_MAX_NAME_SIZE)
    {
        die("[i] check length of uinput name string");
    }

//     snprintf(m_uinputUserDevice.name, uinputName.size(), uinputName.c_str());
    strncpy(m_uinputUserDevice.name, uinputName.c_str(), uinputName.size());
    m_uinputUserDevice.id.bustype = BUS_USB;
    m_uinputUserDevice.id.vendor  = 0x1;
    m_uinputUserDevice.id.product = 0x1;
    m_uinputUserDevice.id.version = 777;


//     uinputUserDevice.absmin[ABS_X]=0;
//     uinputUserDevice.absmax[ABS_X]=1023;
//     uinputUserDevice.absfuzz[ABS_X]=0;
//     uinputUserDevice.absflat[ABS_X ]=0;
//     uinputUserDevice.absmin[ABS_Y]=0;
//     uinputUserDevice.absmax[ABS_Y]=767;
//     uinputUserDevice.absfuzz[ABS_Y]=0;
//     uinputUserDevice.absflat[ABS_Y ]=0;


    if(write(m_openedUinputDeviceFileDescriptor, &m_uinputUserDevice, sizeof(m_uinputUserDevice)) < 0)
    {
        die("error: write");
    }

    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_DEV_CREATE) < 0)
    {
        die("error: ioctl");
    }


}


void UinputCommander::closeDevice(void)
{
    printFunctonNameMacro();

//     std::cout << "openedUinputDeviceFileDescriptor: " << openedUinputDeviceFileDescriptor << std::endl;

    if(ioctl(m_openedUinputDeviceFileDescriptor, UI_DEV_DESTROY) < 0)
    {
           die("[!] error, cannot destroy uinput device");
    }
    else
    {
        std::cout << "[i] successfully destroyed uinput device" << std::endl;
    }

    std::cout << "[i] close uinput device" << std::endl;
    close(m_openedUinputDeviceFileDescriptor);

}


// FIXME: use better function to set absolute cursor position
int UinputCommander::setXPosition(unsigned int x)
{
    printFunctonNameMacro();
    decrementXPosition(-m_maxScreenResolutionX);
    incrementXPosition(x);

    return 0;
}


// FIXME: use better function to set absolute cursor position
int UinputCommander::setYPosition(unsigned int y)
{
    printFunctonNameMacro();
    decrementYPosition(-m_maxScreenResolutionY);
    incrementYPosition(y);

    return 0;
}


// FIXME: use better function to set absolute cursor position
int UinputCommander::setXYPosition(unsigned int x, unsigned int y)
{
//     std::cout << "[i] " << __func__ << std::endl;
//     std::cout << "decrement x: " << m_maxScreenResolutionX << std::endl;
//     std::cout << "decrement y: " << m_maxScreenResolutionY << std::endl;
//     std::cout << "set x: " << x << std::endl;
//     std::cout << "set y: " << y << std::endl;

    decrementXPosition(m_maxScreenResolutionX);
    decrementYPosition(m_maxScreenResolutionY);
    incrementXPosition(x);
    incrementYPosition(y);

    return 0;
}


int UinputCommander::incrementXPosition(unsigned int dx)
{
    printFunctonNameMacro();
    updatePositionRelative(dx, 0);
    return 0;
}


int UinputCommander::incrementXPosition(void)
{
    printFunctonNameMacro();
    updatePositionRelative(1, 0);
    return 0;
}


int UinputCommander::incrementYPosition(unsigned int dy)
{
    printFunctonNameMacro();
    updatePositionRelative(0, dy);
    return 0;
}


int UinputCommander::incrementYPosition(void)
{
    printFunctonNameMacro();
    updatePositionRelative(0, 1);
    return 0;
}


int UinputCommander::incrementXYPosition(unsigned int dx, unsigned int dy)
{
    printFunctonNameMacro();
    updatePositionRelative(dx, dy);
    return 0;
}


int UinputCommander::incrementXYPosition(void)
{
    printFunctonNameMacro();
    updatePositionRelative(1, 1);
    return 0;
}


int UinputCommander::decrementXPosition(unsigned int dx)
{
    printFunctonNameMacro();
    updatePositionRelative(-dx, 0);
    return 0;
}


int UinputCommander::decrementXPosition(void)
{
    printFunctonNameMacro();
    updatePositionRelative(-1, 0);
    return 0;
}


int UinputCommander::decrementYPosition(unsigned int dy)
{
    printFunctonNameMacro();
    updatePositionRelative(0, -dy);
    return 0;
}


int UinputCommander::decrementYPosition(void)
{
    printFunctonNameMacro();
    updatePositionRelative(0, -1);
    return 0;
}


int UinputCommander::decrementXYPosition(unsigned int dx , unsigned int dy)
{
    printFunctonNameMacro();
    updatePositionRelative(-dx, -dy);
    return 0;
}


int UinputCommander::decrementXYPosition(void)
{
    printFunctonNameMacro();
    updatePositionRelative(-1, -1);
    return 0;
}


int UinputCommander::moveToCenterPosition(void)
{
    printFunctonNameMacro();
    std::cout << "[i] move mouse cursor to center position ("
        << m_maxScreenResolutionX / 2
        << "/"
        << m_maxScreenResolutionY / 2
        << ")" << std::endl;
    setXYPosition(m_maxScreenResolutionX / 2, m_maxScreenResolutionY / 2);
    return 0;
}



inline int UinputCommander::updatePositionRelative(int dx, int dy)
{
    printFunctonNameMacro();

    debug() << "> dx: " << dx << std::endl;
    debug() << "> dy: " << dy << std::endl;

    struct input_event inputEvent;


    if (dx != 0)
    {
        memset(&inputEvent, 0, sizeof(struct input_event));
        gettimeofday(&inputEvent.time, NULL);
        inputEvent.type = EV_REL;
        inputEvent.code = REL_X;
        inputEvent.value = dx;
        if(write(m_openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
        {
            die("[!] error write");
        }
    }

    if (dy != 0)
    {
        memset(&inputEvent, 0, sizeof(struct input_event));
        gettimeofday(&inputEvent.time, NULL);
        inputEvent.type = EV_REL;
        inputEvent.code = REL_Y;
        inputEvent.value = dy;
        if(write(m_openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
        {
            die("[!] error write");
        }
    }

    if ((dx != 0) || (dy != 0) )
        {
        memset(&inputEvent, 0, sizeof(struct input_event));
        gettimeofday(&inputEvent.time, NULL);
        inputEvent.type = EV_SYN;
        inputEvent.code = SYN_REPORT;
        inputEvent.value = 0;
        if(write(m_openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
        {
            die("[!] error write");
        }
    }

    return 0;
}


// int UinputCommander::updatePositionAbsolute(int x, int y)
// {
//     cout << __func__ << endl;
//
//     struct input_event inputEvent;
//
//     memset(&inputEvent, 0, sizeof(struct input_event));
//     inputEvent.type = EV_ABS;
//     inputEvent.code = ABS_X;
//     inputEvent.value = x;
//     if(write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
//     {
//         die("[!] error write");
//     }
// 
//     memset(&inputEvent, 0, sizeof(struct input_event));
//     inputEvent.type = EV_ABS;
//     inputEvent.code = ABS_Y;
//     inputEvent.value = y;
//     if(write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
//     {
//         die("[!] error write");
//     }
// 
//     memset(&inputEvent, 0, sizeof(struct input_event));
//     inputEvent.type = EV_SYN;
//     inputEvent.code = SYN_REPORT;
//     inputEvent.value = 0;
//     if(write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
//     {
//         die("[!] error write");
//     }
//
//     return 0;
// }


int UinputCommander::clickLeft()
{
    printFunctonNameMacro();
    return changeButtonState(BTN_LEFT, 1);
}


int UinputCommander::releaseLeft()
{
    printFunctonNameMacro();
    return changeButtonState(BTN_LEFT, 0);
}


int UinputCommander::clickRight()
{
    printFunctonNameMacro();
    return changeButtonState(BTN_LEFT, 1);
}


int UinputCommander::releaseRight()
{
    printFunctonNameMacro();
    return changeButtonState(BTN_LEFT, 0);
}


inline int UinputCommander::changeButtonState(__u16 buttonCode, __s32 buttonValue)
{
    printFunctonNameMacro();

    struct input_event inputEvent;

    // Report BUTTON CLICK - RELEASE event
    memset(&inputEvent, 0, sizeof(inputEvent));
    gettimeofday(&inputEvent.time, NULL);

    inputEvent.type = EV_KEY;
    inputEvent.code = buttonCode;
    inputEvent.value = buttonValue;
    if (write(m_openedUinputDeviceFileDescriptor, &inputEvent, sizeof(inputEvent)) < 0)
    {
        die("[!] error write");
    }

    inputEvent.type = EV_SYN;
    inputEvent.code = SYN_REPORT;
    inputEvent.value = 0;
    if (write(m_openedUinputDeviceFileDescriptor, &inputEvent, sizeof(inputEvent)) < 0)
    {
        die("[!] error write");
    }
    return 0;
}

