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
 * ZNetworking_TCP.h
 *
 *  Created on: 16 mai 2017
 *      Author: laurent
 */

#ifndef Z_ZNETWORKING_TCP_H
#define Z_ZNETWORKING_TCP_H

//#ifndef Z_ZNETWORKING_TCP_H
//#  include "ZNetworking_TCP.h"
//#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif


class ZTCPNet_Connection
{
    friend class ZTCPNet_Socket;

    sockaddr_in Address;
    int         ConnectionFileDescriptor;
    bool        ValidConnection;

  public:

    ZTCPNet_Connection()
    {
      memset(&Address, 0, sizeof(sockaddr_in));
      ConnectionFileDescriptor = 0;
      ValidConnection = false;
    }

    ~ZTCPNet_Connection()
    {
      if (ValidConnection) Close();
    }

    bool ReadChar(char &c)
    {
      return(read(ConnectionFileDescriptor,&c,1)==1);
    }

    bool Write(char * Line, int Len)
    {
      return(Len == write(ConnectionFileDescriptor, Line, Len));
    }

    void Close()
    {
      if (ValidConnection)
      {
        close( ConnectionFileDescriptor);
        ConnectionFileDescriptor = 0;
        ValidConnection = false;
        memset(&Address, 0, sizeof(sockaddr_in));
      }
    }

    bool Set_NonBlockingMode(bool Enabled=true)
    {
      int Actual, Status;

      Actual = fcntl(ConnectionFileDescriptor, F_GETFL, 0);

      if (Enabled) Actual |= O_NONBLOCK;
      else         Actual &= -1 ^ O_NONBLOCK;

      Status = fcntl(ConnectionFileDescriptor, F_SETFL, Actual);

      if (Status == -1) return(false);

      return(true);
    }



};

class ZTCPNet_Socket
{
  protected:

    sockaddr_in Address;
    int         SocketFileDescriptor;
    bool        Initialized_Socket;
    bool        Bound_Socket;

  public:

    ZTCPNet_Socket()
    {
      SocketFileDescriptor = 0;
      memset(&Address, 0, sizeof(sockaddr_in));
      Initialized_Socket = false;
      Bound_Socket = false;
    }

    ~ZTCPNet_Socket()
    {
      if (Initialized_Socket) Close();
    }

    bool Create()
    {
      // If already initialized, return error

      if (Initialized_Socket) return(false);

      // Create a socket

      SocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
      if (SocketFileDescriptor == -1) return(false);

      Initialized_Socket = true;
      return(true);
    }

    void Close()
    {
      if (Initialized_Socket)
      {
        close( SocketFileDescriptor);
        SocketFileDescriptor = 0;
        Initialized_Socket = false;
        memset(&Address, 0, sizeof(sockaddr_in));
      }
    }

    bool Bind(int Port)
    {
      // Server address

      if (Bound_Socket) return(false);

      Address.sin_family = AF_INET;
      Address.sin_port = htons(Port);
      Address.sin_addr.s_addr = INADDR_ANY;

      if (bind(SocketFileDescriptor, (struct sockaddr *) &Address,sizeof(Address)) == -1) return(false);

      Bound_Socket = true;

      return(true);
    }

    bool Listen()
    {
      if (listen(SocketFileDescriptor, 5) == -1) return(false);

      return(true);
    }

    bool Accept(ZTCPNet_Connection * Connection)
    {
      if (!Initialized_Socket || !Bound_Socket) return(false);


      unsigned int Len = sizeof(Connection->Address);
      Connection->ConnectionFileDescriptor = accept(SocketFileDescriptor, (struct sockaddr *) &Connection->Address, &Len);
      if (Connection->ConnectionFileDescriptor == -1 ) return(false);

      Connection->ValidConnection = true;

      return(true);
    }

    bool Set_NonBlockingMode(bool Enabled=true)
    {
      int Actual, Status;

      Actual = fcntl(SocketFileDescriptor, F_GETFL, 0);

      if (Enabled) Actual |= O_NONBLOCK;
      else         Actual &= -1 ^ O_NONBLOCK;

      Status = fcntl(SocketFileDescriptor, F_SETFL, Actual);

      if (Status == -1) return(false);

      return(true);
    }

    bool Set_AbortiveClose(bool Enabled=true)
    {
      struct linger Ln;
      int Res;

      Ln.l_onoff = 1;
      Ln.l_linger = 30;
      Res = setsockopt(SocketFileDescriptor,
                       SOL_SOCKET,
                       SO_LINGER,
                       &Ln,
                       sizeof Ln);

      return(Res ? false:true);
    }

    bool Set_ReceiveTimeOut(int Sec, int USec)
    {
      struct timeval Time;

      Time.tv_sec  = Sec;
      Time.tv_usec = USec;

      if (setsockopt (SocketFileDescriptor, SOL_SOCKET, SO_RCVTIMEO, (char *)&Time, sizeof(Time)) < 0) return(false);

      return(true);
    }

    bool Set_SendTimeOut(int Sec, int USec)
    {
      struct timeval Time;

      Time.tv_sec  = Sec;
      Time.tv_usec = USec;

      if (setsockopt (SocketFileDescriptor, SOL_SOCKET, SO_SNDTIMEO, (char *)&Time, sizeof(Time)) < 0) return(false);

      return(true);
    }


    static void Test()
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
            usleep(1000000);
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
};



#endif /* Z_ZNETWORKING_TCP_H */
