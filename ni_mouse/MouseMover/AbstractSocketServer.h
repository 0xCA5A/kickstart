#ifndef _ABSTRACTSOCKETSERVER_H_
#define _ABSTRACTSOCKETSERVER_H_

class AbstractSocketServer
{
    public:
        virtual int openSocket(void) = 0;
        virtual int closeSocket(void) = 0;
        virtual int receiveData(char* dataBuffer, int dataBufferSize) = 0;

        //default constructor
        AbstractSocketServer(){}

    private:
        //copy constructor
        AbstractSocketServer(const AbstractSocketServer&);

        //assignment operator
        AbstractSocketServer& operator=(const AbstractSocketServer&);
};

#endif