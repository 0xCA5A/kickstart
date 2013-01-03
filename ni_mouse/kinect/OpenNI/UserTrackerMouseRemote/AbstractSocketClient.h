#ifndef _ABSTRACTSOCKETCLIENT_H_
#define _ABSTRACTSOCKETCLIENT_H_


class AbstractSocketClient
{
    public:
        virtual int openSocket(void) = 0;
        virtual int closeSocket(void) = 0;
        virtual int sendData(char* dataBuffer, int dataBufferSize) = 0;
};


#endif