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
 * ZStream.h
 *
 *  Created on: 26 sept. 2011
 *      Author: laurent
 */

#ifndef Z_STREAMS_H
#define Z_STREAMS_H

//#ifndef Z_STREAMS_H
//#  include "ZStreams.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

class ZStream
{
  public:
    virtual ~ZStream() {}

    virtual bool  OpenRead()=0;
    virtual bool  OpenWrite()=0;
    virtual bool  OpenReadWrite()=0;
    virtual bool  OpenApend()=0;
    virtual bool  Close()=0;

    virtual bool  WriteObj(void * Object,ULong ObjectLen, ULong nObjects)=0;
    virtual ULong ReadObj(void * Object,ULong ObjectLen , ULong nObjects)=0;
    virtual bool  Move(long Offset)=0;
    virtual ULong Note()=0;
    virtual bool  Point(ULong NVal)=0;
    virtual void  Flush()=0;
    virtual bool  IsError()=0;
    virtual bool  LookError()=0;

    virtual ZStream & operator << (ULong const Data)=0;
    virtual ZStream & operator << (Long  const Data)=0;
    virtual ZStream & operator << (UShort const Data)=0;
    virtual ZStream & operator << (Short  const Data)=0;
    virtual ZStream & operator << (UByte  const Data)=0;
    virtual ZStream & operator << (Byte   const Data)=0;
    virtual ZStream & operator << (char const * const Data)=0;
    virtual ZStream & operator << (ZString const * const Data)=0;
    virtual ZStream & operator << (ZString const & Data)=0;

    virtual ZStream & operator >> (ULong  &Data)=0;
    virtual ZStream & operator >> (Long   &Data)=0;
    virtual ZStream & operator >> (UShort &Data)=0;
    virtual ZStream & operator >> (Short  &Data)=0;
    virtual ZStream & operator >> (UByte  &Data)=0;
    virtual ZStream & operator >> (Byte   &Data)=0;
    virtual ZStream & operator >> (char * Data)=0;
    virtual ZStream & operator >> (ZString * Data)=0;
    virtual ZStream & operator >> (ZString & Data)=0;

    virtual bool CopyFileFrom(ZStream &SrcStream)= 0;
    virtual bool AppendToOpenedFileFrom(ZStream &SrcStream) = 0;
    virtual bool CompareFileFrom(ZStream &SrcStream)= 0;

    virtual void AddToSavedLen(ULong &Len, ULong x)=0;
    virtual void AddToSavedLen(ULong &Len, Long x)=0;
    virtual void AddToSavedLen(ULong &Len, UShort x)=0;
    virtual void AddToSavedLen(ULong &Len, Short x)=0;
    virtual void AddToSavedLen(ULong &Len, UByte x)=0;
    virtual void AddToSavedLen(ULong &Len, Byte x)=0;
    virtual void AddToSavedLen(ULong &Len, char * String)=0;
    virtual void AddToSavedLen(ULong &Len, ZString * String)=0;
    virtual void AddToSavedLen(ULong &Len, ZString & String)=0;

    ZStream & NewLine() { (*this)<<"\r\n";return (*this);}
};

#endif /* Z_STREAM_H */
