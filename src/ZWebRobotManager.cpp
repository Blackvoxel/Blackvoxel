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
 * ZWebRobotManager.cpp
 *
 *  Created on: 19 mai 2017
 *      Author: laurent
 */

#include "ZWebRobotManager.h"


#ifndef Z_ZNETWORKING_TCP_H
#  include "z/ZNetworking_TCP.h"
#endif

#ifndef Z_ZSIMPLEREQUESTPARSER_H
#  include "z/ZSimpleRequestParser.h"
#endif




ZWebRobotManager::ZWebRobotManager()
{
  Socket = new ZTCPNet_Socket();
  IsStarted = false;
  FreeRequest = 0;
}

ZWebRobotManager::~ZWebRobotManager()
{
  if (Socket)
  {
    delete Socket;
    Socket = 0;
  }
  if (FreeRequest)
  {
    delete FreeRequest;
    FreeRequest = 0;
  }

  IsStarted = false;

}

bool ZWebRobotManager::Start(int Port)
{
  bool Res;

  // Create socket

  Res = Socket->Create();    if (!Res) return(false);

  // Set the socket to non blocking mode

  Socket->Set_NonBlockingMode();
  Socket->Set_AbortiveClose();

  // Bind socket with port 5000
  if (!Port) Port = 48559;

  Res = Socket->Bind(Port);  if (!Res) { Socket->Close(); return(false); }

  // Listen

  Res = Socket->Listen();    if (!Res) { Socket->Close(); return(false); }

  Socket->Set_ReceiveTimeOut(5,0);
  Socket->Set_SendTimeOut(5,0);

  printf("Web Robot Extension server waiting on port %d\n", Port);
  return(true);
}

void ZWebRobotManager::Process()
{
  char c;

  Request * Req;

  if (!FreeRequest)
  {
    FreeRequest = new Request;
  }

  // Reply to unprocessed connections (connections for which no robot answered).

  while ((Req = (Request *)RequestList.RemoveTail()))
  {
    // printf("Answering connection\n");
    char * Line = (char *) "HTTP/1.0 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello, World!";
    Req->Connection.Write(Line,strlen(Line));
    Req->Connection.Close();
    delete Req;
  }

  // Accept new requests

  if (Socket->Accept(&FreeRequest->Connection))
  {
    // printf("Accept connection\n");
    FreeRequest->Connection.Set_NonBlockingMode();
    while( FreeRequest->Connection.ReadChar(c)) FreeRequest->Text.Append_char(c);
    //printf("-*-*-*-*-*-*-*-*-*\n%s\n-*-*-*-*-*-*-*-*-*-\n",FreeRequest->Text.String);
    RequestList.AddToHead(FreeRequest);
    FreeRequest = 0;
  }
}

void ZWebRobotManager::Stop()
{
  Socket->Close();
  printf("Closing Socket\n");
}

ZWebRobotManager::Request * ZWebRobotManager::PopRequest()
{
  return((Request *)RequestList.RemoveTail());
}

/*
bool ZWebRobotManager::ReplyOK(Request * Request)
{

  Request->Connection.Write()

  return(true);

}
*/

bool ZWebRobotManager::Answer_NoParameters(ZSimpleRequestParser & Parser, ZWebRobotManager::Request * Req)
{
  ZString Out, Callback;
  bool Cb, Result;


  if (Parser.FindEntryText((char *)"callback", Callback)) {Cb = true;}

  // Header

  Out << "HTTP/1.0 200 OK\r\n"
         "Content-Type: text/javascript; charset=utf8\r\n"
         "\r\n";

  // If there is jsonp callback, put it.

  if (Cb) Out << Callback << "(";

  // Dummy Payload

  Out << "{\n\"main\": { \"dummy\":0 }\n}\n";

  // Callback closing form

  if (Cb) Out << ");";

  // Write result on opened connection.

  Result = Req->Connection.Write(Out.String,Out.Len);

  // Close connection after reply

  Req->Connection.Close();

  // Result

  return(Result);
}

bool ZWebRobotManager::Answer_OneParameter(ZSimpleRequestParser & Parser, ZWebRobotManager::Request * Req, ZString & Parameter)
{
  ZString Out, Callback;
  bool Cb, Result;


  if (Parser.FindEntryText((char *)"callback", Callback)) {Cb = true;}

  // Header

  Out << "HTTP/1.0 200 OK\r\n"
         "Content-Type: text/javascript; charset=utf8\r\n"
         "\r\n";

  // If there is jsonp callback, put it.

  if (Cb) Out << Callback << "(";

  // Dummy Payload

  Out << "{\n\"main\": { \"data\":" << Parameter << " }\n}\n";

  // Callback closing form

  if (Cb) Out << ");";

  // Write result on opened connection.

  Result = Req->Connection.Write(Out.String,Out.Len);

  // Close connection after reply

  Req->Connection.Close();

  // Result

  return(Result);
}


