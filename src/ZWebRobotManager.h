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
 * ZWebRobotManager.h
 *
 *  Created on: 19 mai 2017
 *      Author: laurent
 */

#ifndef Z_ZWEBROBOTMANAGER_H
#define Z_ZWEBROBOTMANAGER_H

//#ifndef Z_ZWEBROBOTMANAGER_H
//#  include "ZWebRobotManager.h"
//#endif

#ifndef Z_ZOBJECT_LISTS_H
#  include "z/ZObject_Lists.h"
#endif

#ifndef Z_ZNETWORKING_TCP_H
#  include "z/ZNetworking_TCP.h"
#endif

class ZTCPNet_Socket;
class ZSimpleRequestParser;

class ZWebRobotManager
{
  protected:

    ZTCPNet_Socket * Socket;
    bool IsStarted;

    ZList RequestList;

  public:
    class Request : public ZObject
    {
      public:

        ZString Text;
        ZTCPNet_Connection Connection;
    };

  protected:
    Request * FreeRequest;
  public:

    ZWebRobotManager();
   ~ZWebRobotManager();

    bool Start(int Port);

    void Process();

    void Stop();

    Request * PopRequest();

    bool Answer_NoParameters(ZSimpleRequestParser & Parser, ZWebRobotManager::Request * Req);
    bool Answer_OneParameter(ZSimpleRequestParser & Parser, ZWebRobotManager::Request * Req, ZString & Parameter);

};



#endif /* Z_ZWEBROBOTMANAGER_H */
