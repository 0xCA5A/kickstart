#ifndef _UNIXDOMAINSOCKETSERVER_H_
#define _UNIXDOMAINSOCKETSERVER_H_


#include "AbstractSocketServer.h"



#include <iostream>

class UnixDomainSocketServer : public AbstractSocketServer
{
    public:
        UnixDomainSocketServer(const std::string& socketPath);
        ~UnixDomainSocketServer(void);

        virtual int openSocket(void);
        virtual int closeSocket(void);
        virtual int receiveData(char* dataBuffer, int dataBufferSize);

    protected:

    private:
        UnixDomainSocketServer();

    private:
        int m_fileDescriptor;
        std::string m_socketPath;
};


#endif