/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
 *
 * Blackvoxel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Blackvoxel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 * ZNetworking_TCP.cpp
 *
 *  Created on: 14 juin 2017
 *      Author: laurent
 */

#ifndef Z_ZNETWORKING_TCP_H
#  include "ZNetworking_TCP.h"
#endif

#ifdef ZENV_OS_WINDOWS
  #include <windows.h>
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <stdlib.h>
  #include <stdio.h>
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
#endif

ULong ZTCPNet_Socket::NetworkInitCounter=0;


ZTCPNet_Connection::ZTCPNet_Connection()
{
  Address = new sockaddr_in; if (!Address) throw;
  memset(Address, 0, sizeof(sockaddr_in));
  ConnectionFileDescriptor = 0;
  ValidConnection = false;
}


ZTCPNet_Connection::~ZTCPNet_Connection()
{
  if (ValidConnection) Close();
  if (Address) {delete Address; Address=0;}
}


bool ZTCPNet_Connection::ReadChar(char &c)
{
#ifdef ZENV_OS_WINDOWS
  return(1==recv(ConnectionFileDescriptor,&c,1,0 ));
#else
  return(1==read(ConnectionFileDescriptor,&c,1));
#endif
}


bool ZTCPNet_Connection::Write(char * Line, int Len)
{
#ifdef ZENV_OS_WINDOWS
	return(Len == send(ConnectionFileDescriptor, Line, Len,0));
#else
  return(Len == write(ConnectionFileDescriptor, Line, Len));
#endif
}

bool ZTCPNet_Connection::Shutdown(ShutdownMode Mode)
{
#ifdef ZENV_OS_WINDOWS

  int How;

  switch(Mode)
  {
    case SHD_ALL:     How = SD_BOTH;    break;
    case SHD_RECEIVE: How = SD_RECEIVE; break;
    case SHD_SEND:    How = SD_SEND;    break;
  }
  return( 0==shutdown(ConnectionFileDescriptor, How));

#else

  int How;

  switch(Mode)
  {
    case SHD_ALL:     How = SHUT_RDWR; break;
    case SHD_RECEIVE: How = SHUT_RD;   break;
    case SHD_SEND:    How = SHUT_WR;   break;
  }
  return( 0==shutdown(ConnectionFileDescriptor, How));

#endif
}


void ZTCPNet_Connection::Close()
{
  if (ValidConnection)
  {
    close( ConnectionFileDescriptor);
    ConnectionFileDescriptor = 0;
    ValidConnection = false;
    memset(Address, 0, sizeof(sockaddr_in));
  }
}


bool ZTCPNet_Connection::Set_NonBlockingMode(bool Enabled)
{
#ifdef ZENV_OS_WINDOWS

  // Windows version

  unsigned long state = Enabled ? 1 : 0;
  if (0 != ioctlsocket(ConnectionFileDescriptor, FIONBIO, &state))
  {
      return(false);
  }

#else

  int Actual, Status;

  Actual = fcntl(ConnectionFileDescriptor, F_GETFL, 0);

  if (Enabled) Actual |= O_NONBLOCK;
  else         Actual &= -1 ^ O_NONBLOCK;

  Status = fcntl(ConnectionFileDescriptor, F_SETFL, Actual);

  if (Status == -1) return(false);

#endif

  return(true);
}


ZTCPNet_Socket::ZTCPNet_Socket()
{
  Address = new sockaddr_in; if (!Address) throw;
  SocketFileDescriptor = 0;
  memset(Address, 0, sizeof(sockaddr_in));
  Initialized_Socket = false;
  Bound_Socket = false;
}


ZTCPNet_Socket::~ZTCPNet_Socket()
{
  if (Initialized_Socket) Close();
  if (Address) {delete Address; Address=0;}
}

bool ZTCPNet_Socket::Init()
{
  if (NetworkInitCounter == 0)
  {
#ifdef ZENV_OS_WINDOWS
    WSADATA wsaData;
    if(WSAStartup(0x202, &wsaData) != 0) return(false);
#endif
    NetworkInitCounter++;
  }

  return(true);
}

void ZTCPNet_Socket::End()
{
  if (NetworkInitCounter==1)
  {
#ifdef ZENV_OS_WINDOWS
    WSACleanup();
#endif
  }
  if (NetworkInitCounter>0) NetworkInitCounter--;

}


bool ZTCPNet_Socket::Create()
{
  // If already initialized, return error

  if (Initialized_Socket) return(false);

  // Create a socket

  SocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (SocketFileDescriptor == -1) return(false);

  Initialized_Socket = true;
  return(true);
}


void ZTCPNet_Socket::Close()
{
  if (Initialized_Socket)
  {
    close( SocketFileDescriptor);
    SocketFileDescriptor = 0;
    Initialized_Socket = false;
    memset(Address, 0, sizeof(sockaddr_in));
  }
}


bool ZTCPNet_Socket::Bind(int Port)
{
  // Server address

  if (Bound_Socket) return(false);

  Address->sin_family = AF_INET;
  Address->sin_port = htons(Port);
  Address->sin_addr.s_addr = INADDR_ANY;

  if (bind(SocketFileDescriptor, (struct sockaddr *) Address,sizeof(sockaddr_in)) == -1) return(false);

  Bound_Socket = true;

  return(true);
}


bool ZTCPNet_Socket::Listen()
{
  if (listen(SocketFileDescriptor, 5) == -1) return(false);

  return(true);
}


bool ZTCPNet_Socket::Accept(ZTCPNet_Connection * Connection)
{
  if (!Initialized_Socket || !Bound_Socket) return(false);


  socklen_t Len = sizeof(sockaddr_in);
  Connection->ConnectionFileDescriptor = accept(SocketFileDescriptor, (struct sockaddr *)Connection->Address, &Len);
  if (Connection->ConnectionFileDescriptor == -1 ) return(false);

  Connection->ValidConnection = true;

  return(true);
}


bool ZTCPNet_Socket::Set_NonBlockingMode(bool Enabled)
{
#ifdef ZENV_OS_WINDOWS

  // Windows version

  unsigned long state = Enabled ? 1 : 0;
  if (0 != ioctlsocket(SocketFileDescriptor, FIONBIO, &state))
  {
      return(false);
  }

#else

  // Linux, OSX and other unix.

  int Actual, Status;

  Actual = fcntl(SocketFileDescriptor, F_GETFL, 0);

  if (Enabled) Actual |= O_NONBLOCK;
  else         Actual &= -1 ^ O_NONBLOCK;

  Status = fcntl(SocketFileDescriptor, F_SETFL, Actual);

  if (Status == -1) return(false);

#endif

  return(true);
}


bool ZTCPNet_Socket::Set_AbortiveClose(bool Enabled)
{
  struct linger Ln;
  int Res;

  Ln.l_onoff = 1;
  Ln.l_linger = 30;
  Res = setsockopt(SocketFileDescriptor,
                   SOL_SOCKET,
                   SO_LINGER,
                   (char *)&Ln,
                   sizeof Ln);

  return(Res ? false:true);
}


bool ZTCPNet_Socket::Set_ReceiveTimeOut(int Sec, int USec)
{
  struct timeval Time;

  Time.tv_sec  = Sec;
  Time.tv_usec = USec;

  if (setsockopt (SocketFileDescriptor, SOL_SOCKET, SO_RCVTIMEO, (char *)&Time, sizeof(Time)) < 0) return(false);

  return(true);
}


bool ZTCPNet_Socket::Set_SendTimeOut(int Sec, int USec)
{
  struct timeval Time;

  Time.tv_sec  = Sec;
  Time.tv_usec = USec;

  if (setsockopt (SocketFileDescriptor, SOL_SOCKET, SO_SNDTIMEO, (char *)&Time, sizeof(Time)) < 0) return(false);

  return(true);
}


void ZTCPNet_Socket::Test()
{
  ZTCPNet_Socket Socket;
  ZTCPNet_Connection Connection;
  char c;

  if (Socket.Create())
  {
    if (Socket.Bind(5003))
    {
      Socket.Set_NonBlockingMode();
      Socket.Listen();
      while(true)
      {
        printf("Waiting connection\n");
        //usleep(1000000);
        if(Socket.Accept(&Connection))
        {
          Connection.Set_NonBlockingMode();
          printf("Accept new incoming connection...\n");

          while(Connection.ReadChar(c)) putchar(c);

          char * Line = (char *) "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello, World!";

          Connection.Write(Line, strlen((const char *)Line));

          Connection.Close();
        }
      }
    }


  }
}
