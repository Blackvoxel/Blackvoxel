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
 * ZStream_File.cpp
 *
 *  Created on: 26 sept. 2011
 *      Author: laurent
 */

#include "ZStream_File.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef ZENV_OS_LINUX
#  include "stdlib.h"
#  include "linux/limits.h"
#  include "sys/stat.h"
#endif

#ifdef ZENV_OS_WINDOWS
#  include "windows.h"
#  include "Shlobj.h"
#endif

#ifdef ZENV_OS_OSX
#  include "stdlib.h"
#  include "limits.h"
#  include "sys/stat.h"
#endif


void ZStream_File::AddToSavedLen(ULong &Len, ULong x) {Len += sizeof(ULong);}
void ZStream_File::AddToSavedLen(ULong &Len, Long x)  {Len += sizeof(Long);}
void ZStream_File::AddToSavedLen(ULong &Len, UShort x){Len += sizeof(UShort);}
void ZStream_File::AddToSavedLen(ULong &Len, Short x) {Len += sizeof(Short);}
void ZStream_File::AddToSavedLen(ULong &Len, UByte x) {Len += sizeof(UByte);}
void ZStream_File::AddToSavedLen(ULong &Len, Byte x)  {Len += sizeof(Byte);}
void ZStream_File::AddToSavedLen(ULong &Len, char * String) {ULong i;for (i=0;String[i]!=0;i++);Len+=i+sizeof(ULong);}
void ZStream_File::AddToSavedLen(ULong &Len, ZString * String) {Len += String->Len + sizeof(ULong);}
void ZStream_File::AddToSavedLen(ULong &Len, ZString & String) {Len += String.Len + sizeof(ULong);}


bool ZStream_File::IsError()
{
  bool Flag;

  Flag = Error;
  Error= false;
  return(Flag);
}

bool ZStream_File::LookError()
{
  return(Error);
}

bool ZStream_File::WriteObj(void * Object,ULong ObjectLen, ULong nObjects)
{
  if (Fl && WriteOk)
  {
    if (nObjects==fwrite(Object,ObjectLen,nObjects,(FILE *)Fl))
    {
      return(true);
    }
  }
  Error = true;
  return(false);
}

ULong ZStream_File::ReadObj(void * Object,ULong ObjectLen , ULong nObjects)
{
  if (Fl && ReadOk)
  {
    if ((nObjects=fread(Object,ObjectLen,nObjects,(FILE *)Fl)))
    {
      return(nObjects);
    }
  }
  Error = true;
  return(0UL);
}

bool ZStream_File::Move(long Offset)
{
  if (!fseek((FILE *)Fl, Offset,SEEK_CUR)) return(true);
  Error = true;
  return(false);
}

ULong ZStream_File::Note()
{
  return((ULong)ftell((FILE *)Fl));
}

bool  ZStream_File::Point(ULong NoteVal)
{
  if (!fseek((FILE *)Fl, NoteVal,SEEK_SET)) return(true);
  Error = true;
  return(false);
}

void ZStream_File::Flush()
{
  if (Fl && (WriteOk || ReadOk)) fflush((FILE *)Fl);
}

ZStream & ZStream_File::operator << (ULong const Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data,4,1,(FILE *)Fl))
    {
      Error = true;
    }
  }
  else Error = true;
  return(*this);
}



ZStream & ZStream_File::operator << (Long const Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data,4,1,(FILE *)Fl))
    {
      Error = true;
    }
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator << (UShort const Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data,2,1,(FILE *)Fl))
    {
      Error = true;
    }
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator << (Short const Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data,2,1,(FILE *)Fl))
    {
      Error = true;
    }
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator << (UByte const Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data,1,1,(FILE *)Fl))
    {
      Error = true;
    }
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator << (Byte const Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data,1,1,(FILE *)Fl))
    {
      Error = true;
    }
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator << (char const * const Data)
{
  ULong Len;

  Len = strlen(Data);
  if (Fl && WriteOk)
  {
    if (Len)
      if (Len!=fwrite(Data,1,Len,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator << (ZString const * const Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data->Len,4,1,(FILE *)Fl))     Error = true;
    if (Data->Len)
      if (1!=fwrite(Data->String,Data->Len,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator << (ZString const & Data)
{
  if (Fl && WriteOk)
  {
    if (1!=fwrite(&Data.Len,4,1,(FILE *)Fl))     Error = true;
    if (Data.Len)
      if (1!=fwrite(Data.String,Data.Len,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}



ZStream & ZStream_File::operator >> (ULong  &Data)
{
  if (Fl && ReadOk)
  {
    if (1!=fread(&Data,4,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator >> (Long   &Data)
{
  if (Fl && ReadOk)
  {
    if (1!=fread(&Data,4,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator >> (UShort &Data)
{
  if (Fl && ReadOk)
  {
    if (1!=fread(&Data,2,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}


ZStream & ZStream_File::operator >> (Short  &Data)
{
  if (Fl && ReadOk)
  {
    if (1!=fread(&Data,2,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator >> (UByte  &Data)
{
  if (Fl && ReadOk)
  {
    if (1!=fread(&Data,1,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator >> (Byte   &Data)
{
  if (Fl && ReadOk)
  {
    if (1!=fread(&Data,1,1,(FILE *)Fl)) Error = true;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator >> (char * Data)
{
  ULong Len;
  if (Fl && ReadOk)
  {
    if (1!=fread(&Len,4,1,(FILE *)Fl)) {Error = true;return(*this);}
    if (Len)
      if (1!=fread(Data,Len,1,(FILE *)Fl)) {Error = true;return(*this);}
    Data[Len] = 0;
  }
  else Error = true;
  return(*this);
}


ZStream & ZStream_File::operator >> (ZString * Data)
{
  ULong Len;
  if (Fl && ReadOk)
  {
    if (1!=fread(&Len,4,1,(FILE *)Fl)) {Error = true;return(*this);}
    if (!Data->MemLen)              {Error = true;return(*this);}
    if (Len)
      if (1!=fread(Data->String,Len,1,(FILE *)Fl)) {Error = true;return(*this);}
    Data->String[Len] = 0;
  }
  else Error = true;
  return(*this);
}

ZStream & ZStream_File::operator >> (ZString & Data)
{
  ULong Len;
  if (Fl && ReadOk)
  {
    if (1!=fread(&Len,4,1,(FILE *)Fl)) {Error = true;return(*this);}
    if (!Data.MemLen)              {Error = true;return(*this);}
    if (Len)
      if (1!=fread(Data.String,Len,1,(FILE *)Fl)) {Error = true;return(*this);}
    Data.String[Len] = 0;
  }
  else Error = true;
  return(*this);
}

bool ZStream_File::CopyFileFrom(ZStream &SrcStream)
{
  char * Buffer;
  ULong i;
  bool ReturnValue = false;
  if ((Buffer = new char[10240]))
  {
    if (SrcStream.OpenRead())
    {
      if (OpenWrite())
      {
        do
        {
          i = SrcStream.ReadObj(Buffer,1,10240);
          if (i) WriteObj(Buffer,1,i);
        } while(i);
        ReturnValue = true;
        Close();
      }
      SrcStream.Close();
    }
    delete Buffer;
  }
  return(ReturnValue);
}

bool ZStream_File::AppendToOpenedFileFrom(ZStream &SrcStream)
{
  char * Buffer;
  ULong i;
  bool ReturnValue = false;
  if ((Buffer = new char[10240]))
  {
    if (SrcStream.OpenRead())
    {
      if (WriteOk)
      {
        do
        {
          i = SrcStream.ReadObj(Buffer,1,10240);
          if (i) WriteObj(Buffer,1,i);
        } while(i);
        ReturnValue = true;
      }
      SrcStream.Close();
    }
    delete Buffer;
  }
  return(ReturnValue);
}


bool ZStream_File::CompareFileFrom(ZStream &SrcStream)
{
  char * Buffer1;
  char * Buffer2;
  ULong i1,i2;
  bool ReturnValue = false;
  if ((Buffer1 = new char[10240]))
  {
    if ((Buffer2 = new char[10240]))
    {
      if (SrcStream.OpenRead())
      {
        if(OpenRead())
        {
          ReturnValue = true;
          do
          {
            i1 = SrcStream.ReadObj(Buffer1,1,10240);
            i2 = ReadObj(Buffer2,1,10240);
            if (i1 !=i2) ReturnValue = false;
            else
            {
              if (i2)
                while(--i2) if (Buffer1[i2] != Buffer2[i2]) {ReturnValue = false;continue;}
            }
          } while(ReturnValue && i1);
          Close();
        }
        SrcStream.Close();
      }
      delete Buffer2;
    }
    delete Buffer1;
  }
  return(ReturnValue);
}



bool ZStream_File::Close()
{
  ReadOk = false;
  WriteOk = false;
  if (Fl)
  {
    fclose((FILE *)Fl);
    Fl = 0;
    return(true);
  }
  Error = true;
  return(false);
}

bool ZStream_File::OpenWrite()
{
  if (!Fl)
  {
    if ((Fl = (void *)fopen(FileName.String,"wb")))
    {
      WriteOk = true;
      Error  = false;
      return(true);
    }
  }
  Error = true;
  return(false);
}

bool ZStream_File::OpenRead()
{
  if (!Fl)
  {
    if ((Fl = (void *)fopen(FileName.String,"rb")))
    {
      ReadOk = true;
      Error = false;
      return(true);
    }
  }
  Error = true;
  return(false);
}



ZStream_File::ZStream_File()
{
  Fl = 0;
  ReadOk =  false;
  WriteOk = false;
  Error   = false;
}

bool ZStream_File::SetFileName(char const * const FileName)
{
  this->FileName = FileName;
  return(true);
}

bool ZStream_File::Set_CurrentDirectory(char const * const Directory)
{
#ifdef ZENV_OS_WINDOWS
  return(::SetCurrentDirectory(Directory));
#endif
#ifdef ZENV_OS_LINUX
  return( chdir(Directory) ? true: false);
#endif
#ifdef ZENV_OS_OSX
  return( chdir(Directory) ? true: false);
#endif
}

ZString ZStream_File::Get_CurrentDirectory()
{
#ifdef ZENV_OS_LINUX
  ZString Buffer;
  Buffer.RaiseMem_DiscardContent(PATH_MAX);
  if (0 == getcwd(Buffer.String, PATH_MAX)) Buffer.Clear();
  return(Buffer);
#endif
#ifdef ZENV_OS_WINDOWS
  ULong Writen;
  ZString Directory;
  Directory.RaiseMem_DiscardContent(4096);
  Writen = ::GetCurrentDirectory(4096UL,Directory.String);
  Directory.SetLen(Writen);
  return(Directory);
#endif
#ifdef ZENV_OS_OSX
  ZString Buffer;
  Buffer.RaiseMem_DiscardContent(PATH_MAX);
  if (0 == getcwd(Buffer.String, PATH_MAX)) Buffer.Clear();
  return(Buffer);
#endif
}

ZString ZStream_File::Get_Directory_UserData()
{
#ifdef ZENV_OS_LINUX
  ZString Buffer;
  char * Str;

  Str = getenv("HOME");
  if (Str) Buffer = Str;
  else     Str = (char * )"/home/laurent";

  return(Buffer);
#endif
#ifdef ZENV_OS_WINDOWS
  HRESULT Result;
  ZString Directory, Buffer;
  Buffer.RaiseMem_DiscardContent(4096);
  Result = ::SHGetFolderPath(0, CSIDL_PERSONAL , 0, 0,  Buffer.String);
  // Result = ::SHGetFolderPath(0, CSIDL_APPDATA, 0, 0,  Buffer.String);
  if (Result == S_OK) Directory = Buffer.String;
  return(Directory);
#endif
#ifdef ZENV_OS_OSX
  ZString Buffer;
  char * Str;

  Str = getenv("HOME");
  if (Str) Buffer = Str;
  else     Str = (char * )"/home/laurent";

  return(Buffer);
#endif
}

bool ZStream_File::Copy_File(char * ToCopy, char * NewFile, bool FailIfExists)
{
#ifdef ZENV_OS_LINUX
  // To do
  return(false);
#endif

#ifdef ZENV_OS_WINDOWS
  return(::CopyFile(ToCopy, NewFile, FailIfExists));
#endif

#ifdef ZENV_OS_OSX
  // To do
  return(false);
#endif
}

bool ZStream_File::Compare_Files(char * File1, char * File2)
{
  char Buf1[4096],Buf2[4096];
  FILE * handle1, * handle2;
  ULong F1Len,F2Len,i;

  handle1 = fopen(File1,"rb"); if (!handle1) return(false);
  handle2 = fopen(File2,"rb"); if (!handle2) {fclose(handle1);return(false);}
  do
  {
    F1Len = fread(Buf1,1,4096,handle1);
    F2Len = fread(Buf2,1,4096,handle2);
    if (F1Len !=F2Len) return(false);
    for (i=0;i<F1Len;i++) if (Buf1[i] != Buf2[i]) return(false);
  }while(F1Len==4096);
  fclose(handle1);
  fclose(handle2);
  return(true);
}

bool ZStream_File::Directory_Create(const char * NewDir)
{
#ifdef ZENV_OS_LINUX
  return (mkdir(NewDir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP ) ? false : true);
#endif
#ifdef ZENV_OS_WINDOWS
  return ::CreateDirectory(NewDir, 0) ? true:false;
#endif
#ifdef ZENV_OS_OSX
  return (mkdir(NewDir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP ) ? false : true);
#endif
}

bool ZStream_File::Directory_Destroy(const char * DirectoryToRemove)
{
#ifdef ZENV_OS_LINUX
  return(rmdir(DirectoryToRemove) ? false : true);
#endif
#ifdef ZENV_OS_WINDOWS
  return RemoveDirectory( DirectoryToRemove );
#endif
#ifdef ZENV_OS_OSX
  return(rmdir(DirectoryToRemove) ? false : true);
#endif
}

bool ZStream_File::Destroy_File(char * FileToDestroy)
{
#ifdef ZENV_OS_LINUX
  return( unlink(FileToDestroy) ? false:true);
#endif
#ifdef ZENV_OS_WINDOWS
  return DeleteFile(FileToDestroy);
#endif
#ifdef ZENV_OS_OSX
  return( unlink(FileToDestroy) ? false:true);
#endif
}


bool ZStream_File::PutFileContent(ZString & Content)
{
  if (OpenWrite())
  {
    if (1!=WriteObj(Content.String, Content.Len, 1))
    {
      WriteOk = false;
      Error = true;
    }
    else
    {
      WriteOk = true;
      Error = false;
    }
    Close();
  }
  return(Error);
}

bool ZStream_File::GetFileContent(ZString & Result)
{
  ULong Len;
  ULong EffectiveLen;

  // Opening
  if (!Fl)
  {
    if ((Fl = (void *)fopen(FileName.String,"rb")))
    {
      ReadOk = true;
      Error = false;
    }
    else
    {
      ReadOk = false;
      Error = true;
    }
  } else { Error = true; return(false);}


  // Reading
  if (Fl && ReadOk)
  {
    do
    {
      Len = Result.Len;
      Result.SetLen(Len + 4096);

      EffectiveLen = fread(Result.String+Len,1,4096,(FILE *)Fl);
      Result.SetLen(Len + EffectiveLen);
    } while (EffectiveLen == 4096);
  }
  else Error = true;

  // Closing
  ReadOk = false;
  WriteOk = false;
  if (Fl)
  {
    fclose((FILE *)Fl);
    Fl = 0;
    return(true);
  }
  Error = true;
  return(false);

}

bool ZStream_File::Directory_IsExists(const char * DirectoryName)
{
#ifdef ZENV_OS_LINUX

  int Result;
  struct stat StatInfo;

  Result = stat(DirectoryName, &StatInfo);

  if (Result!=0) return(false);
  if (StatInfo.st_mode & S_IFDIR) return(true);
  return(false);

#endif
#ifdef ZENV_OS_WINDOWS

  DWORD Result;

  Result = GetFileAttributesA(DirectoryName);

  if (Result == INVALID_FILE_ATTRIBUTES) return(false);
  if (Result & FILE_ATTRIBUTE_DIRECTORY) return(true);
  return(false);

#endif
#ifdef ZENV_OS_OSX

  int Result;
  struct stat StatInfo;

  Result = stat(DirectoryName, &StatInfo);

  if (Result!=0) return(false);
  if (StatInfo.st_mode & S_IFDIR) return(true);
  return(false);

#endif
}

bool ZStream_File::File_IsExists(const char * FileName)
{
#ifdef ZENV_OS_LINUX

  int Result;
  struct stat StatInfo;

  Result = stat(FileName, &StatInfo);

  if (Result!=0) return(false);
  if (StatInfo.st_mode & S_IFREG) return(true);
  return(false);

#endif
#ifdef ZENV_OS_WINDOWS

  DWORD Result;

  Result = GetFileAttributesA(FileName);

  if (Result == INVALID_FILE_ATTRIBUTES) return(false);
  if (Result & ( FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_READONLY)) return(false);

  return(true);

#endif
#ifdef ZENV_OS_OSX

  int Result;
  struct stat StatInfo;

  Result = stat(FileName, &StatInfo);

  if (Result!=0) return(false);
  if (StatInfo.st_mode & S_IFREG) return(true);
  return(false);

#endif
}


bool ZStream_File::Directory_CreateIfNotExists(const char * NewDir)
{
  if (Directory_IsExists(NewDir)) return(true);

  return(Directory_Create(NewDir));

}
