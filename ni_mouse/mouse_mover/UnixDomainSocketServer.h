#ifndef _UNIXDOMAINSOCKETSERVER_H_
#define _UNIXDOMAINSOCKETSERVER_H_


#include "AbstractSocketServer.h"

#define UNIX_DOMAIN_SOCKET_PATH "/tmp/myUnixDomainSocket"


class UnixDomainSocketServer : public AbstractSocketServer
{
    public:
        UnixDomainSocketServer(void);
        ~UnixDomainSocketServer(void);

        virtual int setupAndOpenSocket(void);
        virtual int closeSocket(void);
        virtual int receiveData(char* dataBuffer, int dataBufferSize);

    protected:

    private:
        int fileDescriptor;
};


#endif