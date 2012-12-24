
#include "UnixDomainSocketServer.h"
#include "HelperStuff.h"

#include <iostream>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>



UnixDomainSocketServer::~UnixDomainSocketServer()
{
    std::cout << __func__ << std::endl;
    closeSocket();
}


std::string  socket_path = UNIX_DOMAIN_SOCKET_PATH;
struct sockaddr_un addr;
// char buf[1024];
int fd;
int cl;
int rc;


int UnixDomainSocketServer::setupAndOpenSocket(void)
{

    std::cout << __func__ << std::endl;

//   if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
//     perror("socket error");
//     exit(-1);
//   }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
//   strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path)-1);
    strcpy(addr.sun_path, socket_path.c_str());

  if ( (fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
    die("socket error");
  }

  unlink(socket_path.c_str());



  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    die("bind error");
  }

//     if (listen(fd, 5) == -1) {
//     perror("listen error");
//     exit(-1);
//   }
// 
//   std::cout << "wait for client..." << std::endl; 
//     if ( (cl = accept(fd, NULL, NULL)) == -1) {
//       perror("accept error");
// //       continue;
// //         TODO
// 
//     }

  
    int i = 132;
    return i;
}

int UnixDomainSocketServer::closeSocket(void)
{
    std::cout << __func__ << std::endl;
    close(fd);
    int c = 123;
    return c;
}

//client as argument!
int UnixDomainSocketServer::receiveData(char* dataBuffer, int dataBufferSize)
{
//         std::cout << __func__ << std::endl;
 static     int c = 0;
 c++;
 
//   while (1) {
//     if ( (cl = accept(fd, NULL, NULL)) == -1) {
//       perror("accept error");
// //       continue;
// //         TODO
//       
//     }

//     while ( (rc=read(cl,buf,sizeof(buf))) > 0) {
//       printf("read %u bytes: %.*s\n", rc, rc, buf);
//     }
//     if (rc == -1) {
//       perror("read");
//       exit(-1);
//     }
//     else if (rc == 0) {
//       printf("EOF\n");
//       close(cl);
//     }
//   }
        memset(dataBuffer, '\0', dataBufferSize);
        int receiveCounter = recvfrom(fd, dataBuffer, dataBufferSize - 1, 0, 0, 0);
        if ( receiveCounter == -1)
        {
            die("receiver: recvfrom");
        }
//         std::cout << "> " << receiveCounter << std::endl;
 
//     int c = 123;
    return receiveCounter ;
}

// 
// 
// UnixDomainSocketServer::receiveData()
// {
// 
// }