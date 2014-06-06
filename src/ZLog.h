/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2014 Laurent Thiebaut & Olivia Merle
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
 * ZLog.h
 *
 *  Created on: 19 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZLOG_H
#define Z_ZLOG_H

//#ifndef Z_ZLOG_H
//#  include "ZLog.h"
//#endif

#ifndef Z_STREAMS_H
#  include "z/ZStreams.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

class ZLog
{
    ULong Section;
    ZString LogString;

    ZString OutputFilespec;
    ZStream * Stream;
  public:
    enum TInfoType {FAIL, INFO, IMINFO}; // IMINFO = Important info.

    void Log(ULong MessageID, TInfoType InfoType, const char * String);
    void Log(ULong MessageID, TInfoType InfoType, ZString & String);

    void SetFileSpec(const char * Filespec)
    {
      OutputFilespec = Filespec;
    }

    ZLog * Sec(ULong Section) {this->Section = Section; return(this);}

    ZLog()
    {
      Section = 0;
      OutputFilespec = "Log.txt";
      Stream = 0;
    }

    ~ZLog()
    {
      if (Stream)
      {
        Stream->Close();
        delete Stream;
      }
    }
};


#endif /* Z_LOG_H */
