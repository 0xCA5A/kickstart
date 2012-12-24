
#include "HelperStuff.h"

#include "UinputCommander.h"


#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <linux/input.h>
#include <linux/uinput.h>


// TODO
// * check if usleep in constructor is necessary
// * do you want to move cursor to center position in constructor






UinputCommander::UinputCommander(const std::string& uinputDeviceName)
{
    std::cout << "constructor" << std::endl;
    openDevice(uinputDeviceName);
    configureDevice();

    usleep(5000);
    moveToCenterPosition();
}


UinputCommander::~UinputCommander()
{
    std::cout << "destructor" << std::endl;
    releaseLeft();
    closeDevice();
}


void UinputCommander::openDevice(const std::string& uinputDeviceName)
{
    std::cout << "[i] try to open uinput device " << uinputDeviceName << std::endl;

    openedUinputDeviceFileDescriptor = open(uinputDeviceName.c_str(), O_WRONLY | O_NONBLOCK);
    if(openedUinputDeviceFileDescriptor < 0)
    {
        die("[!] error in open");
    }
}


void UinputCommander::configureDevice()
{
    if (openedUinputDeviceFileDescriptor == 0)
    {
        die("[!] error, uinput device not opened (fd is 0)...");
    }

    //key stuff
    if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_EVBIT, EV_KEY) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_KEYBIT, BTN_LEFT) < 0)
    {
        die("[i] error in ioctl");
    }
    if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_EVBIT, EV_KEY) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_KEYBIT, BTN_RIGHT) < 0)
    {
        die("[i] error in ioctl");
    }

    //relative movement stuff
    if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_EVBIT, EV_REL) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_RELBIT, REL_X) < 0)
    {
        die("[i] error in ioctl");
    }

    if(ioctl(openedUinputDeviceFileDescriptor, UI_SET_RELBIT, REL_Y) < 0)
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


    memset(&uinputUserDevice, 0, sizeof(uinputUserDevice));
    snprintf(uinputUserDevice.name, UINPUT_MAX_NAME_SIZE, UINPUT_NAME);
    uinputUserDevice.id.bustype = BUS_USB;
    uinputUserDevice.id.vendor  = 0x1;
    uinputUserDevice.id.product = 0x1;
    uinputUserDevice.id.version = 777;


//     uinputUserDevice.absmin[ABS_X]=0;
//     uinputUserDevice.absmax[ABS_X]=1023;
//     uinputUserDevice.absfuzz[ABS_X]=0;
//     uinputUserDevice.absflat[ABS_X ]=0;
//     uinputUserDevice.absmin[ABS_Y]=0;
//     uinputUserDevice.absmax[ABS_Y]=767;
//     uinputUserDevice.absfuzz[ABS_Y]=0;
//     uinputUserDevice.absflat[ABS_Y ]=0;


    if(write(openedUinputDeviceFileDescriptor, &uinputUserDevice, sizeof(uinputUserDevice)) < 0)
    {
        die("error: write");
    }

    if(ioctl(openedUinputDeviceFileDescriptor, UI_DEV_CREATE) < 0)
    {
        die("error: ioctl");
    }


}


void UinputCommander::closeDevice(void)
{


//     std::cout << "openedUinputDeviceFileDescriptor: " << openedUinputDeviceFileDescriptor << std::endl;

    if(ioctl(openedUinputDeviceFileDescriptor, UI_DEV_DESTROY) < 0)
    {
           die("[!] error, cannot destroy uinput device");
    }
    else
    {
        std::cout << "[i] successfully destroyed uinput device..." << std::endl;
    }

    std::cout << "[i] close uinput device..." << std::endl;
    close(openedUinputDeviceFileDescriptor);

}


// FIXME: use better function to set absolute cursor position
int UinputCommander::setXPosition(unsigned int x)
{
    decrementXPosition(-MAX_X_RESOLUTION);
    incrementXPosition(x);

    return 0;
}


// FIXME: use better function to set absolute cursor position
int UinputCommander::setYPosition(unsigned int y)
{
    decrementYPosition(-MAX_Y_RESOLUTION);
    incrementYPosition(y);

    return 0;
}


// FIXME: use better function to set absolute cursor position
int UinputCommander::setXYPosition(unsigned int x, unsigned int y)
{
//     std::cout << "decrement x: " << MAX_X_RESOLUTION << std::endl;
//     std::cout << "decrement y: " << MAX_Y_RESOLUTION << std::endl;
//     std::cout << "set x: " << x << std::endl;
//     std::cout << "set y: " << y << std::endl;

    decrementXPosition(MAX_X_RESOLUTION);
    decrementYPosition(MAX_Y_RESOLUTION);
    incrementXPosition(x);
    incrementYPosition(y);

    return 0;
}


int UinputCommander::incrementXPosition(unsigned int dx)
{
    updatePositionRelative(dx, 0);
    return 0;
}


int UinputCommander::incrementXPosition(void)
{
    updatePositionRelative(1, 0);
    return 0;
}


int UinputCommander::incrementYPosition(unsigned int dy)
{
    updatePositionRelative(0, dy);
    return 0;
}


int UinputCommander::incrementYPosition(void)
{
    updatePositionRelative(0, 1);
    return 0;
}


int UinputCommander::incrementXYPosition(unsigned int dx, unsigned int dy)
{
    updatePositionRelative(dx, dy);
    return 0;
}


int UinputCommander::incrementXYPosition(void)
{
    updatePositionRelative(1, 1);
    return 0;
}


int UinputCommander::decrementXPosition(unsigned int dx)
{
    updatePositionRelative(-dx, 0);
    return 0;
}


int UinputCommander::decrementXPosition(void)
{
    updatePositionRelative(-1, 0);
    return 0;
}


int UinputCommander::decrementYPosition(unsigned int dy)
{
    updatePositionRelative(0, -dy);
    return 0;
}


int UinputCommander::decrementYPosition(void)
{
    updatePositionRelative(0, -1);
    return 0;
}


int UinputCommander::decrementXYPosition(unsigned int dx , unsigned int dy)
{
    updatePositionRelative(-dx, -dy);
    return 0;
}


int UinputCommander::decrementXYPosition(void)
{
    updatePositionRelative(-1, -1);
    return 0;
}


int UinputCommander::moveToCenterPosition(void)
{
    std::cout << "[i] " << __func__ << std::endl;
    setXYPosition(MAX_X_RESOLUTION / 2, MAX_Y_RESOLUTION / 2);
    return 0;
}



inline int UinputCommander::updatePositionRelative(int dx, int dy)
{
//     cout << __func__ << endl;
// 
//     cout << "> x: " << dx << endl;
//     cout << "> y: " << dy << endl;

    
    struct input_event inputEvent;

    memset(&inputEvent, 0, sizeof(struct input_event));

    gettimeofday(&inputEvent.time, NULL);
    inputEvent.type = EV_REL;
    inputEvent.code = REL_X;
    inputEvent.value = dx;
    if(write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
    {
        die("[!] error write");
    }

    memset(&inputEvent, 0, sizeof(struct input_event));
    gettimeofday(&inputEvent.time, NULL);
    inputEvent.type = EV_REL;
    inputEvent.code = REL_Y;
    inputEvent.value = dy;
    if(write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
    {
        die("[!] error write");
    }

    memset(&inputEvent, 0, sizeof(struct input_event));
    gettimeofday(&inputEvent.time, NULL);
    inputEvent.type = EV_SYN;
    inputEvent.code = SYN_REPORT;
    inputEvent.value = 0;
    if(write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(struct input_event)) < 0)
    {
        die("[!] error write");
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
    return changeButtonState(BTN_LEFT, 1);
}

int UinputCommander::releaseLeft()
{
    return changeButtonState(BTN_LEFT, 0);
}

int UinputCommander::clickRight()
{
    return changeButtonState(BTN_LEFT, 1);
}

int UinputCommander::releaseRight()
{
    return changeButtonState(BTN_LEFT, 0);
}

// inline int UinputCommander::clickButton(__u16 buttonCode)
// {
// //     std::cout << "[" << __func__ << "] clicked!\n" << std::endl;
// 
//     struct input_event inputEvent;
//     
// //     memset(&inputEvent, 0, sizeof(inputEvent));
// //     gettimeofday(&inputEvent.time, NULL);
// 
// 
// 
//     // Report BUTTON CLICK - PRESS event
//     memset(&inputEvent, 0, sizeof(inputEvent));
//     gettimeofday(&inputEvent.time, NULL);
// 
//     inputEvent.type = EV_KEY;
//     inputEvent.code = buttonCode;
//     inputEvent.value = 1;
//     if (write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(inputEvent)) < 0)
//     {
//         die("[!] error write");
//     }
// 
//     inputEvent.type = EV_SYN;
//     inputEvent.code = SYN_REPORT;
//     inputEvent.value = 0;
//     if (write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(inputEvent)) < 0)
//     {
//         die("[!] error write");
//     }
// 
//     return 0;
// }


inline int UinputCommander::changeButtonState(__u16 buttonCode, __s32 buttonValue)
{
//     std::cout << "[" << __func__ << "] released!\n" << std::endl;

    struct input_event inputEvent;

    // Report BUTTON CLICK - RELEASE event
    memset(&inputEvent, 0, sizeof(inputEvent));
    gettimeofday(&inputEvent.time, NULL);

    inputEvent.type = EV_KEY;
    inputEvent.code = buttonCode;
    inputEvent.value = buttonValue;
    if (write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(inputEvent)) < 0)
    {
        die("[!] error write");
    }

    inputEvent.type = EV_SYN;
    inputEvent.code = SYN_REPORT;
    inputEvent.value = 0;
    if (write(openedUinputDeviceFileDescriptor, &inputEvent, sizeof(inputEvent)) < 0)
    {
        die("[!] error write");
    }
    return 0;
}



/*
int
main2(void)
{
    int                    fd;
    struct uinput_user_dev uidev;
    struct input_event     ev;
    int                    dx = 0;
    int                     dy = 0;
    int                    i;

    fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("error: open");

    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        die("error: ioctl");

    if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
        die("error: ioctl");

    memset(&uidev, 0, sizeof(uidev));
        gettimeofday(&inputEvent.time, NULL);
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        die("error: write");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: ioctl");

    sleep(2);

//     srand(time(NULL));

    while(1) {
        switch(rand() % 4) {
        case 0:
            dx = -10;
            dy = -1;
            break;
        case 1:
            dx = 10;
            dy = 1;
            break;
        case 2:
            dx = -1;
            dy = 10;
            break;
        case 3:
            dx = 1;
            dy = -10;
            break;
        }

        for(i = 0; i < 20; i++) {
            memset(&ev, 0, sizeof(struct input_event));
            ev.type = EV_REL;
            ev.code = REL_X;
            ev.value = dx;
            if(write(fd, &ev, sizeof(struct input_event)) < 0)
                die("error: write");

            memset(&ev, 0, sizeof(struct input_event));
            ev.type = EV_REL;
            ev.code = REL_Y;
            ev.value = dy;
            if(write(fd, &ev, sizeof(struct input_event)) < 0)
                die("error: write");

            memset(&ev, 0, sizeof(struct input_event));
            ev.type = EV_SYN;
            ev.code = SYN_REPORT;
            ev.value = 0;
            if(write(fd, &ev, sizeof(struct input_event)) < 0)
                die("error: write");

            usleep(15000);
        }

        sleep(5);
    }

    sleep(2);

    if(ioctl(fd, UI_DEV_DESTROY) < 0)
        die("error: ioctl");

    close(fd);

    return 0;
}*/