#ifndef _UINPUTCOMMANDER_H_
#define _UINPUTCOMMANDER_H_



#include <iostream>
#include <string>
#include <linux/uinput.h>
#include <linux/input.h>

// using namespace std;







class UinputCommander
{

    public:

        UinputCommander(const std::string& uinputUNIXDeviceName, const std::string& uinputName, const unsigned int maxScreenResolutionX, const unsigned int maxScreenResolutionY);
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
        int clickRight(void);
        int releaseRight(void);

//         int moveCursor(unsigned int xPos, unsigned int yPos);
        int moveToCenterPosition(void);



protected:
private:

    void openDevice(const std::string& uinputUNIXDeviceName);
    void configureDevice(const std::string& uinputName);
    void closeDevice(void);


    inline int changeButtonState(__u16 buttonCode, __s32 buttonValue);

    inline int updatePositionRelative(int dx, int dy);
//        inline int updatePositionAbsolute(int x, int y);


    int m_openedUinputDeviceFileDescriptor;
    struct uinput_user_dev m_uinputUserDevice;
    const unsigned int m_maxScreenResolutionX;
    const unsigned int m_maxScreenResolutionY;

};


#endif


