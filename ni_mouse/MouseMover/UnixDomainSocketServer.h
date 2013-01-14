#ifndef _UNIXDOMAINSOCKETSERVER_H_
#define _UNIXDOMAINSOCKETSERVER_H_


#include "AbstractSocketServer.h"
       #include <sys/select.h>

#include <iostream>


class UnixDomainSocketServer : public AbstractSocketServer
{
    public:
        UnixDomainSocketServer(const int unsigned selectTimeoutValueInSeconds, const std::string& socketPath);
        ~UnixDomainSocketServer(void);

        virtual int openSocketNonBlocking(void);
        virtual int closeSocket(void);
        virtual int receiveDataNonBlocking(char* dataBuffer, int dataBufferSize);

    private:
        //default constructor
        UnixDomainSocketServer();

        //copy constructor
        UnixDomainSocketServer(const UnixDomainSocketServer& other);

        //assignment operator
        UnixDomainSocketServer&  operator = (const UnixDomainSocketServer& other);

    private:
        int m_fileDescriptor;
        unsigned int m_selectTimeoutValueInSeconds;
        struct timeval m_timeout;
        fd_set m_master_set;
        fd_set m_working_set;

        std::string m_socketPath;
};


#endif