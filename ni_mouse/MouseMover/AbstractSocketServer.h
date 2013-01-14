#ifndef _ABSTRACTSOCKETSERVER_H_
#define _ABSTRACTSOCKETSERVER_H_

class AbstractSocketServer
{
    public:
        virtual int openSocketNonBlocking(void) = 0;
        virtual int closeSocket(void) = 0;
        virtual int receiveDataNonBlocking(char* dataBuffer, int dataBufferSize) = 0;

    private:
        //assignment operator
        AbstractSocketServer& operator=(const AbstractSocketServer&);
};

#endif