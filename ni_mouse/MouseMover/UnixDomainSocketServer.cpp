
#include "UnixDomainSocketServer.h"
#include "HelperStuff.h"
#include "Debug.h"

#include <iostream>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <fcntl.h>



UnixDomainSocketServer::UnixDomainSocketServer(const int unsigned selectTimeoutValueInSeconds, const std::string& socketPath)
    : AbstractSocketServer(),
    m_fileDescriptor(0),
    m_selectTimeoutValueInSeconds(selectTimeoutValueInSeconds),
    m_socketPath(socketPath)

{
    printFunctonNameMacro();

    m_timeout.tv_sec  = selectTimeoutValueInSeconds;
    m_timeout.tv_usec = 0;
}


UnixDomainSocketServer::~UnixDomainSocketServer(void)
{
    printFunctonNameMacro();
    closeSocket();
}


int UnixDomainSocketServer::openSocketNonBlocking(void)
{
    printFunctonNameMacro();

    //check the select example here for further socket / select information
    //http://publib.boulder.ibm.com/infocenter/iseries/v5r3/index.jsp?topic=%2Frzab6%2Frzab6xnonblock.htm

    struct sockaddr_un socketAddr;

    std::cout << "[i] open UNIX domain socket " << m_socketPath << "..." << std::endl;

    memset(&socketAddr, 0, sizeof(socketAddr));
    socketAddr.sun_family = AF_UNIX;
//   strncpy(socketAddr.sun_path, socket_path.c_str(), sizeof(socketAddr.sun_path)-1);
    strcpy(socketAddr.sun_path, m_socketPath.c_str());

    if ( (m_fileDescriptor = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        closeSocket();
        die("[!] socket error");
    }

    //set the socket to non-blocking
    int socketFlags = fcntl(m_fileDescriptor, F_GETFL, 0);
    if (fcntl(m_fileDescriptor, F_SETFL, socketFlags | O_NONBLOCK))
    {
        closeSocket();
        die("[!] fcntl error");
    }

    //try to unlink, remove the specified file
    unlink(m_socketPath.c_str());

    if (bind(m_fileDescriptor, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) == -1)
    {
        closeSocket();
        die("[!] bind error");
    }

    //initialize the master fd_set
    FD_ZERO(&m_master_set);
//     m_max_sd = m_fileDescriptor;
    FD_SET(m_fileDescriptor, &m_master_set);

    return m_fileDescriptor;
}


int UnixDomainSocketServer::closeSocket(void)
{
    printFunctonNameMacro();
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


/**
 * @brief function tries to read from a socket (m_fileDescriptor).
 * the function is non blocking, select handles the timeout (m_selectTimeoutValueInSeconds).
 *
 * the function returns a
 *  - negative value if an error occur
 *  - 0 if select timed out
 *  - positive value if data was received from the socket (nr of received bytes)
 *
 * @param dataBuffer data area to write the received data to
 * @param dataBufferSize size of the data buffer
 * @return int
 **/
int UnixDomainSocketServer::receiveDataNonBlocking(char* dataBuffer, int dataBufferSize)
{
//     printFunctonNameMacro();

    if (m_fileDescriptor == 0)
    {
        std::cout << "[!] bad file descriptor" << std::endl;
        return -1;
    }

    //FIXME: why do i have to re-set this value here every time???
    m_timeout.tv_sec = m_selectTimeoutValueInSeconds;
    m_timeout.tv_usec = 0;


    //flatten buffer
    memset(dataBuffer, '\0', dataBufferSize);

    //copy the master fd_set over to the working fd_set
    memcpy(&m_working_set, &m_master_set, sizeof(m_master_set));

    int returnValue = 0;

    //call select() and wait 5 minutes for it to complete
    debug() << "[i] waiting on select...";
    returnValue = select(m_fileDescriptor + 1, &m_working_set, NULL, NULL, &m_timeout);


    //check to see if the select call failed
    if (returnValue < 0)
    {
         debug() << "[i] select() failed";
         return -2;
    }

    //check to see if the timeout expired
    if (returnValue == 0)
    {
        debug() << "[i] select() timed out";
        return returnValue;
    }


    returnValue = recv(m_fileDescriptor, dataBuffer, dataBufferSize - 1, 0);

    if (returnValue == -1)
    {
        std::cout << "[!] unable to receive from socket" << std::endl;
        return returnValue;
    }

    return returnValue;
}
