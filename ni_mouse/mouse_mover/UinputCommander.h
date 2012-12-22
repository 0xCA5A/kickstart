#ifndef UINPUTCOMMANDER_H
#define UINPUTCOMMANDER_H

#include <iostream>
#include <string>


using namespace std;


class UinputCommander
{
  public:

      ~UinputCommander();
      
    void openDevice(string& uinputDeviceName);
    void closeDevice(void);
    int setXPosition(unsigned int x);
    int setYPosition(unsigned int y);
    int incrementXPosition(unsigned int x);
    int incrementYPosition(unsigned int y);
    int decrementXPosition(unsigned int x);
    int decrementYPosition(unsigned int y);
    int incrementXPosition(void);
    int incrementYPosition(void);
    int decrementXPosition(void);
    int decrementYPosition(void);

    


protected:
private:
    unsigned int xPos;
    unsigned int yPos;
    int openedUinputDeviceFileDescriptor;

};


#endif


