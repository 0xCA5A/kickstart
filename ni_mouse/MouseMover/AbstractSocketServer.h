
#ifndef _ABSTRACTSOCKETSERVER_H_
#define _ABSTRACTSOCKETSERVER_H_

class AbstractSocketServer
{
    public:
        virtual int setupAndOpenSocket(void) = 0;
        virtual int closeSocket(void) = 0;
        virtual int receiveData(char* dataBuffer, int dataBufferSize) = 0;
};

#endif