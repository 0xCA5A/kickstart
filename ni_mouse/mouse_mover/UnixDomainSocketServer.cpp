
#include "UnixDomainSocketServer.h"
#include "HelperStuff.h"

#include <iostream>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>


UnixDomainSocketServer::UnixDomainSocketServer(void)
    : AbstractSocketServer()
{
    fileDescriptor = 0;
}


UnixDomainSocketServer::~UnixDomainSocketServer(void)
{
    printFunctonNameMacro();
    closeSocket();
}


int UnixDomainSocketServer::setupAndOpenSocket(void)
{
    printFunctonNameMacro();

    std::string  socketPath = UNIX_DOMAIN_SOCKET_PATH;
    struct sockaddr_un socketAddr;

    memset(&socketAddr, 0, sizeof(socketAddr));
    socketAddr.sun_family = AF_UNIX;
//   strncpy(socketAddr.sun_path, socket_path.c_str(), sizeof(socketAddr.sun_path)-1);
    strcpy(socketAddr.sun_path, socketPath.c_str());

    if ( (fileDescriptor = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        die("[!] socket error");
    }

    unlink(socketPath.c_str());

    if (bind(fileDescriptor, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) == -1) {
        die("[!] bind error");
    }

    return fileDescriptor;
}


int UnixDomainSocketServer::closeSocket(void)
{
    printFunctonNameMacro();
    close(fileDescriptor);
    return fileDescriptor;
}


int UnixDomainSocketServer::receiveData(char* dataBuffer, int dataBufferSize)
{
//     printFunctonNameMacro();

    if (fileDescriptor == 0)
    {
        return -1;
    }

    memset(dataBuffer, '\0', dataBufferSize);
    int receiveCounter = recvfrom(fileDescriptor, dataBuffer, dataBufferSize - 1, 0, 0, 0);
    if ( receiveCounter == -1)
    {
        die("[!] ]receiver: recvfrom");
    }

    return receiveCounter ;
}
