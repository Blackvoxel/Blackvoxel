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


#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

struct sockaddr_in;


class ZTCPNet_Connection
{
    friend class ZTCPNet_Socket;

    sockaddr_in * Address;
    int           ConnectionFileDescriptor;
    bool          ValidConnection;

  public:
    enum ShutdownMode  {SHD_ALL,SHD_RECEIVE, SHD_SEND};

    ZTCPNet_Connection();
    ~ZTCPNet_Connection();

    bool ReadChar(char &c);
    bool Write(char * Line, int Len);
    bool Shutdown(ShutdownMode Mode = SHD_ALL);
    void Close();
    bool Set_NonBlockingMode(bool Enabled=true);

};

class ZTCPNet_Socket
{
  protected:
    static ULong NetworkInitCounter;

    sockaddr_in * Address;
    int           SocketFileDescriptor;
    bool          Initialized_Socket;
    bool          Bound_Socket;

  public:

    ZTCPNet_Socket();
    ~ZTCPNet_Socket();

    bool Init();
    void End();

    bool Create();
    void Close();
    bool Bind(int Port);
    bool Listen();
    bool Accept(ZTCPNet_Connection * Connection);
    bool Set_NonBlockingMode(bool Enabled=true);
    bool Set_AbortiveClose(bool Enabled=true);
    bool Set_ReceiveTimeOut(int Sec, int USec);
    bool Set_SendTimeOut(int Sec, int USec);

    static void Test();

};



#endif /* Z_ZNETWORKING_TCP_H */
