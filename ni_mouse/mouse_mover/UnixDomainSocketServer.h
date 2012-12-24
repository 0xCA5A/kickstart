#ifndef _UNIXDOMAINSOCKETSERVER_H_
#define _UNIXDOMAINSOCKETSERVER_H_


#include "AbstractSocketServer.h"

#define UNIX_DOMAIN_SOCKET_PATH "/tmp/myUnixDomainSocket"


class UnixDomainSocketServer : public AbstractSocketServer
{

    public:

//         UnixDomainSocketServer(int hello)
//                 : m_crap(hello);
//         ~UnixDomainSocketServer();

        ~UnixDomainSocketServer();

        virtual int setupAndOpenSocket(void);
        virtual int closeSocket(void);
        virtual int receiveData(char* dataBuffer, int dataBufferSize);


    protected:
    private:
//         int m_crap;

//     void openDevice(const std::string& uinputDeviceName);
//     void configureDevice(void);
//     void closeDevice(void);

//     inline int updatePositionRelative(int dx, int dy);
//        inline int updatePositionAbsolute(int x, int y);

//     unsigned int xPos;
//     unsigned int yPos;
//     int openedUinputDeviceFileDescriptor;
//     struct uinput_user_dev uinputUserDevice;
//     struct input_event     inputEvent;

};


#endif