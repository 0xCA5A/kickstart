#ifndef _UINPUTCOMMANDER_H_
#define _UINPUTCOMMANDER_H_



#include <iostream>
#include <string>
#include <linux/uinput.h>
#include <linux/input.h>

// using namespace std;


#define MAX_X_RESOLUTION 1920
#define MAX_Y_RESOLUTION 1080


#define UINPUT_NAME "myUinput"




class UinputCommander
{

    public:

        UinputCommander(const std::string& uinputDeviceName);
        ~UinputCommander();

        int setXPosition(unsigned int x);
        int setYPosition(unsigned int y);
        int setXYPosition(unsigned int x, unsigned int y);

        int incrementXPosition(unsigned int dx);
        int incrementXPosition(void);
        int incrementYPosition(unsigned int dy);
        int incrementYPosition(void);
        int incrementXYPosition(unsigned int dx, unsigned int dy);
        int incrementXYPosition(void);

        int decrementXPosition(unsigned int dx);
        int decrementXPosition(void);
        int decrementYPosition(unsigned int dy);
        int decrementYPosition(void);
        int decrementXYPosition(unsigned int dx, unsigned int dy);
        int decrementXYPosition(void);

        int clickLeft(void);
        int releaseLeft(void);

//         int moveCursor(unsigned int xPos, unsigned int yPos);
        int moveToCenterPosition(void);



protected:
private:

    void openDevice(const std::string& uinputDeviceName);
    void configureDevice(void);
    void closeDevice(void);

    inline int updatePositionRelative(int dx, int dy);
//        inline int updatePositionAbsolute(int x, int y);

//     unsigned int xPos;
//     unsigned int yPos;
    int openedUinputDeviceFileDescriptor;
    struct uinput_user_dev uinputUserDevice;
//     struct input_event     inputEvent;

};


#endif


