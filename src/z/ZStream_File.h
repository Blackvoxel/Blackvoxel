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
 * ZStreams_File.h
 *
 *  Created on: 26 sept. 2011
 *      Author: laurent
 */

#ifndef Z_ZSTREAMS_FILE_H
#define Z_ZSTREAMS_FILE_H

// #ifndef Z_ZSTREAMS_FILE_H
// #  include "ZStream_File.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_STREAMS_H
#  include "ZStreams.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

class ZStream_File : public ZStream
{
  public:
    ZString FileName;
  protected:
    void * Fl;
    bool  ReadOk;
    bool  WriteOk;
    bool  Error;
  public:
    /*Constr*/ ZStream_File();
    bool  SetFileName(char const * const FileName);
    bool  OpenRead();
    bool  OpenWrite();
    bool  OpenReadWrite() {return(false);} // Not implemented yet.
    bool  OpenApend()     {return(false);} // Not implemented yet.
    bool  Close();
    bool  WriteObj(void * Object,ULong ObjectLen, ULong nObjects);
    ULong  ReadObj(void * Object,ULong ObjectLen , ULong nObjects);
    bool  Move(long Offset);
    ULong Note();
    bool  Point(ULong NVal);
    void  Flush();

    bool  IsError();
    bool  LookError();

    ZStream & operator << (ULong const Data);
    ZStream & operator << (Long  const Data);
    ZStream & operator << (UShort const Data);
    ZStream & operator << (Short const Data);
    ZStream & operator << (UByte const Data);
    ZStream & operator << (Byte const Data);
    ZStream & operator << (char const * const Data);
    ZStream & operator << (ZString const * const Data);
    ZStream & operator << (ZString const & Data);

    ZStream & operator >> (ULong  &Data);
    ZStream & operator >> (Long   &Data);
    ZStream & operator >> (UShort &Data);
    ZStream & operator >> (Short  &Data);
    ZStream & operator >> (UByte  &Data);
    ZStream & operator >> (Byte   &Data);
    ZStream & operator >> (char * Data);
    ZStream & operator >> (ZString * Data);
    ZStream & operator >> (ZString & Data);


    bool CopyFileFrom(ZStream &SrcStream);
    bool AppendToOpenedFileFrom(ZStream &SrcStream);
    bool CompareFileFrom(ZStream &SrcStream);
    bool GetFileContent(ZString & Result);
    bool PutFileContent(ZString & Content);


    static bool Set_CurrentDirectory(char const * const Directory);
    static ZString Get_CurrentDirectory();
    static ZString Get_Directory_UserData();
    static bool Destroy_File(char * FileToDestroy);
    static bool Copy_File(char * ToCopy, char * NewFile, bool FailIfExists);
    static bool Compare_Files(char * File1, char * File2);

    // Directory Creation and management
    static bool Directory_CreateIfNotExists(const char * NewDir);
    static bool Directory_Create(const char * NewDir);
    static bool Directory_Destroy(const char * DirectoryToRemove);
    static bool Directory_IsExists(const char * DirectoryName);

    void AddToSavedLen(ULong &Len, ULong x);
    void AddToSavedLen(ULong &Len, Long x);
    void AddToSavedLen(ULong &Len, UShort x);
    void AddToSavedLen(ULong &Len, Short x);
    void AddToSavedLen(ULong &Len, UByte x);
    void AddToSavedLen(ULong &Len, Byte x);
    void AddToSavedLen(ULong &Len, char * String);
    void AddToSavedLen(ULong &Len, ZString * String);
    void AddToSavedLen(ULong &Len, ZString & String);
};



#endif /* Z_ZSTREAMS_FILE_H */
