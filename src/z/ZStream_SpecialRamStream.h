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
 * ZStream_SpecialRamStream.h
 *
 *  Created on: 1 nov. 2011
 *      Author: laurent
 */

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#define Z_ZSTREAM_SPECIALRAMSTREAM_H

//#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
//#  include "ZStream_SpecialRamStream.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_STREAMS_H
#  include "ZStreams.h"
#endif

class ZStream_SpecialRamStream
{
  static ZMonoSizeMemoryPool MemPool;

  ZStream * Stream;

  char * Buffer;
  ZMemSize  BufferSize;
  ZMemSize  ReadCount;
  ZMemSize  Pointer;

  public:

  void Debug_MemoryOut(Long Offset = 0);



  ZStream_SpecialRamStream()
  {
    Stream = 0;
    BufferSize = 1024 * 1024 * 5;
    ReadCount = 0;
    Buffer = (char *)MemPool.AllocMem(BufferSize);
    Pointer = 0;
  }

  ~ZStream_SpecialRamStream()
  {
    if (Buffer) MemPool.FreeMem(Buffer);
    Buffer = 0;
  }

  void SetStream( ZStream * Stream ) { this->Stream = Stream; }

  bool OpenWrite()
  {
    Pointer = 0;
    return(true);
  }
  bool OpenRead()
  {
    Pointer = 0;
    ReadCount = Stream->ReadObj(Buffer, 1, BufferSize);
    return( (ReadCount) ? true : false );
  }

  inline ZMemSize GetOffset() { return(Pointer); }

  bool inline FlushBuffer()
  {
    bool Result;

    Result = Stream->WriteObj(Buffer, Pointer, 1);
    Pointer = 0;

    return(Result);
  }

  inline void Put(UByte const Data)
  {
    //if (Pointer >= FlushLimit) FlushBuffer();
    Buffer[Pointer++] = (char)Data;
  }

  inline void Put(ULong const Data)
  {
    Buffer[Pointer++]=(char)Data;
    Buffer[Pointer++]=(char)(Data >> 8);
    Buffer[Pointer++]=(char)(Data >> 16);
    Buffer[Pointer++]=(char)(Data >> 24);
  }

  inline void Put(Long const Data)
  {
    Buffer[Pointer++]=(char)Data;
    Buffer[Pointer++]=(char)(Data >> 8);
    Buffer[Pointer++]=(char)(Data >> 16);
    Buffer[Pointer++]=(char)(Data >> 24);
  }

  inline void Put(UELong const Data)
  {
    *(UELong *)(&Buffer[Pointer]) = Data;
    Pointer += sizeof(UELong);
  }

  inline void Put(ELong const Data)
  {
    *(ELong *)(&Buffer[Pointer]) = Data;
    Pointer += sizeof(ELong);
  }

  inline void Put(double const & Data)
  {
    ULong i;

    for (i=0;i<sizeof(Data);i++) Buffer[Pointer++] = ((char *)(&Data))[i];
  }

  inline void Put(UShort const Data)
  {
    Buffer[Pointer++]=(char)Data;
    Buffer[Pointer++]=(char)(Data >> 8);
  }

  inline void Put(Short const Data)
  {
    Buffer[Pointer++]=(char)Data;
    Buffer[Pointer++]=(char)(Data >> 8);
  }

  inline void Put(bool const Data)
  {
    if (Data) Buffer[Pointer++] = 0x01;
    else      Buffer[Pointer++] = 0x00;
  }

  inline void Put (ZVector3L const & Data)
  {
    Put(Data.x);
    Put(Data.y);
    Put(Data.z);
  }

  inline void PutZone(void const * Zone, ZMemSize ZoneLen)
  {
    ZMemSize i;

    for (i=0;i<ZoneLen;i++) Buffer[Pointer++]= ((char*)Zone)[i];
  }

  inline void PutString(char const * String)
  {
    char c;
    while ( (c = *String++) ) Buffer[Pointer++]=c;
  }

  inline void PutStringWithZero(char const * String)
  {
    while ( (Buffer[Pointer++] = *(String++)) );
  }

  inline ULong * GetPointer_ULong()
  {
    return((ULong *)&Buffer[Pointer]);
  }

  inline UShort * GetPointer_UShort()
  {
    return((UShort *)&Buffer[Pointer]);
  }

  inline ULong GetActualBufferLen()
  {
    return(Pointer);
  }

  inline ULong GetRemainBytesToRead()
  {
    return( ReadCount - Pointer);
  }

  inline bool Get(UByte & Data)
  {
    if (Pointer >= ReadCount) return(false);
    Data = Buffer[Pointer++];
    return(true);
  }
  inline bool Get(UShort & Data)
  {
    if ( (Pointer + 2) > (ReadCount) ) return(false);
    Data = *((UShort*)(Buffer+Pointer));
    Pointer += 2;
    return(true);
  }

  inline bool Get(Short & Data)
  {
    if ( (Pointer + 2) > (ReadCount) ) return(false);
    Data = *((Short*)(Buffer+Pointer));
    Pointer += 2;
    return(true);
  }

  inline bool Get(ULong & Data)
  {
    if ( (Pointer + 4) > (ReadCount) ) return(false);
    Data = *((ULong*)(Buffer+Pointer));
    Pointer+=4;
    return(true);
  }

  inline bool Get(Long & Data)
  {
    if ( (Pointer + 4) > (ReadCount) ) return(false);
    Data = *((Long*)(Buffer+Pointer));
    Pointer+=4;
    return(true);
  }

  inline bool Get(UELong & Data)
  {
    if ( (Pointer + 8) > (ReadCount) ) return(false);
    Data = *((UELong*)(Buffer+Pointer));
    Pointer+=8;
    return(true);
  }

  inline bool Get(ELong & Data)
  {
    if ( (Pointer + 8) > (ReadCount) ) return(false);
    Data = *((ELong*)(Buffer+Pointer));
    Pointer+=8;
    return(true);
  }

  inline bool Get(ZVector3L & Data)
  {
    bool Ok;

    Ok   = Get(Data.x);
    Ok   = Ok && Get(Data.y);
    Ok   = Ok && Get(Data.z);

    return(Ok);
  }

  inline bool Get(double & Data)
  {
    if ( (Pointer +6) > (ReadCount) ) return(false);
    Data = *((double*)(Buffer+Pointer));
    Pointer+=sizeof(double);
    return(true);
  }

  inline bool Get(bool & Data)
  {
    if (Pointer >= ReadCount) return(false);
    if (Buffer[Pointer++]>0) Data = true;
    else                     Data = false;
    return(true);
  }

  inline bool GetZone(void * Zone, ZMemSize ZoneLen)
  {
    ZMemSize i;

    if ( (Pointer + ZoneLen) > (ReadCount) ) return(false);
    for (i=0;i<ZoneLen;i++) ((char *)Zone)[i] = Buffer[Pointer++];

    return(true);
  }

  inline bool GetStringFixedLen(char * String, ULong Len)
  {
    ULong NewPointer;

    NewPointer = Pointer + Len;
    if ( NewPointer > ReadCount ) return(false);

    while(Len--) *(String++) = Buffer[Pointer++];

    return(true);
  }

  inline void Close()
  {
    ReadCount = 0;
    Pointer = 0;
  }

};



#endif /* Z_ZSTREAM_SPECIALRAMSTREAM_H */
