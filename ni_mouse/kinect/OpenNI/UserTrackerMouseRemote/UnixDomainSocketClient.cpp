
#include "UnixDomainSocketClient.h"
#include "JSONGenerator.h"
#include "HelperStuff.h"
#include "Debug.h"

#include <iostream>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>


UnixDomainSocketClient::UnixDomainSocketClient(const std::string& socketPath)
    : AbstractSocketClient(),
    m_fileDescriptor(0),
    m_socketPath(socketPath)

{
//     printFunctonNameMacro();
}


UnixDomainSocketClient::~UnixDomainSocketClient(void)
{
//     printFunctonNameMacro();
    closeSocket();
}


int UnixDomainSocketClient::openSocket(void)
{
//     printFunctonNameMacro();

    struct sockaddr_un socketAddr;

    std::cout << "[i] open UNIX domain socket " << m_socketPath << "..." << std::endl;

    memset(&socketAddr, 0, sizeof(socketAddr));
    socketAddr.sun_family = AF_UNIX;
//   strncpy(socketAddr.sun_path, socket_path.c_str(), sizeof(socketAddr.sun_path)-1);
    strcpy(socketAddr.sun_path, m_socketPath.c_str());

    if ( (m_fileDescriptor = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        die("[!] socket error");
    }

    //try to unlink, remove the specified file
    unlink(m_socketPath.c_str());

    if (bind(m_fileDescriptor, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) == -1) {
        die("[!] bind error");
    }

    return m_fileDescriptor;
}


int UnixDomainSocketClient::closeSocket(void)
{
//     printFunctonNameMacro();
    std::cout << "[i] close UNIX domain socket " << m_socketPath << "..." << std::endl;
    if (close(m_fileDescriptor))
    {
        die("[i] close error");
    }

    if (unlink(m_socketPath.c_str()))
    {
        die("[i] unlink error");
    }

    return m_fileDescriptor;
}


int UnixDomainSocketClient::sendData(char* dataBuffer, int dataBufferSize)
{
//     printFunctonNameMacro();

    if (m_fileDescriptor == 0)
    {
        return -1;
    }

    memset(dataBuffer, '\0', dataBufferSize);
    int receiveCounter = recv(m_fileDescriptor, dataBuffer, dataBufferSize - 1, 0);

    if (receiveCounter == -1)
    {
        die("[!] ]receiver: recvfrom");
    }

    return receiveCounter;
}
