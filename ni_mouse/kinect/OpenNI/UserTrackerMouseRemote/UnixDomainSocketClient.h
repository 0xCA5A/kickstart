#ifndef _UNIXDOMAINSOCKETCLIENT_H_
#define _UNIXDOMAINSOCKETCLIENT_H_


#include "AbstractSocketClient.h"



#include <iostream>

class UnixDomainSocketClient : public AbstractSocketClient
{
    public:
        UnixDomainSocketClient(const std::string& socketPath);
        ~UnixDomainSocketClient(void);

        virtual int openSocket(void);
        virtual int closeSocket(void);
        virtual int sendData(char* dataBuffer, int dataBufferSize);

    protected:

    private:
        int m_fileDescriptor;
        std::string m_socketPath;
};


#endif