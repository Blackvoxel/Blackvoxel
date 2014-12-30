/*
 * ZVMachine_T1.cpp
 *
 *  Created on: 7 nov. 2014
 *      Author: laurent
 */

#include "ZVMachine_T1.h"

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

#include <stdio.h>

bool ZVMachine_T1::BinaryLoad(ZString * BinaryFile)
{
  bool Ok;
  ULong Header, Section, Origin, Lenght, SpaceLenght, SpaceType, i;
  UByte Data;
  ZStream_SpecialRamStream Stream;

  //
  DisassemblyTags.Clear();
  //
  Stream.OpenRead(BinaryFile);

  // File Header

  Ok = Stream.Get(Header); if (!Ok) return(false);
  if(Header != MulticharConst('Z','B','I','N')) {printf("Error: Not a Blacvkvoxel T1 ASM file.\n"); return(false); }
  Ok = Stream.Get(Lenght); if (!Ok) return(false);

  while(true)
  {
    Ok = Stream.Get(Section); if (!Ok) return(false);
    switch(Section)
    {
      // Code section
      case MulticharConst('C','O','D','E'):   Ok = Stream.Get(Origin); if (!Ok) return(false);
                                              Ok = Stream.Get(Lenght); if (!Ok) return(false);
                                              while (Lenght--)
                                              {
                                                Ok=Stream.Get(Data); if (!Ok) return(false);
                                                WriteMemory_8(Origin++, Data);
                                              }
                                              break;

      // Uninitialized space section
      case MulticharConst('S','P','C','E'):   Ok = Stream.Get(Origin); if (!Ok) return(false);
                                              Ok = Stream.Get(Lenght); if (!Ok) return(false);
                                              Ok = Stream.Get(SpaceType); if (!Ok) return(false);
                                              Ok = Stream.Get(SpaceLenght); if (!Ok) return(false);
                                              switch(SpaceType)
                                              {
                                                default: break;
                                                case 1: SpaceLenght <<= 1; break;
                                                case 2: SpaceLenght <<= 2; break;
                                              }
                                              for (i=0;i<SpaceLenght;i++) WriteMemory_8(Origin++, 0);
                                              break;

      case MulticharConst('E','N','D','*'):   return(true);

      default:                                printf("Error: Unknown section in asm file.\n");
                                              return(false);

    }
  }

  return(false);
}


bool ZVMachine_T1::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * BlockSize;
  ULong   StartLen;
  ZMemSize i;
  ZString CircuitName;

  BlockSize = Stream->GetPointer_ULong();
  Stream->Put(0u);        // The size of the block (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)1); // Data Block Version;

  // Save virtual machine informations.

  Stream->Put(Running);
  Stream->Put(Servo_MovePos);

  // Save virtual machine RAM.

  Stream->Put((UELong)ZVMACHINE_MEMSIZE); // Ram size
  for(i=0;i<(ZVMACHINE_MEMSIZE/4);i++) Stream->Put( ((ULong *)(&Memory))[i]);

  // Save dissassembly tags array

  DisassemblyTags.Save(Stream);

  // Circuit Informations

  CircuitName = "ZCPU_001";
   Stream->Put(CircuitName);
   CPU.Save(Stream);

  CircuitName = "ZPIA_001";
   Stream->Put(CircuitName);
   PIA_1.Save(Stream);

  CircuitName = "ZDISP_01";
   Stream->Put(CircuitName);
   Display_1.Save(Stream);

  // End of the saving block
  CircuitName = "CHIPEND.";
   Stream->Put(CircuitName);

  *BlockSize = Stream->GetActualBufferLen() - StartLen;

  return(true);
}

bool ZVMachine_T1::Load(ZStream_SpecialRamStream * Stream)
{
  bool Ok;

  ULong  BlockSize, SubBlockSize, Dummy;
  UShort BlockVersion, SubBlockVersion;
  UByte  Temp_Byte;
  ZString String;
  ZMemSize i;

  Ok = Stream->Get(BlockSize);
  Ok&= Stream->Get(BlockVersion);     if(!Ok) return(false);

  // Check for supported block version. If unsupported new version, throw content and continue with a blank dataset.

  if (BlockVersion!=1) { BlockSize-=2; for(ZMemSize i=0;i<BlockSize;i++) Ok = Stream->Get(Temp_Byte); }

  // Load virtual machine informations

  Ok &= Stream->Get(Running);
  Ok &= Stream->Get(Servo_MovePos);    if (!Ok) return(false);

  // Load Virtual Machine Ram

  // Load virtual machine RAM.

  UELong _RamSize;
  ZMemSize ReadMemSize, ThrowSize, RamSize;
  Stream->Get(_RamSize); // Ram size
  RamSize = (ZMemSize)_RamSize;

  // Adjust read lenght and throw lenght depending on difference between stored data and actual robot ram size

  if      (RamSize == ZVMACHINE_MEMSIZE) {ReadMemSize = ZVMACHINE_MEMSIZE; ThrowSize = 0; }
  else if (RamSize <  ZVMACHINE_MEMSIZE) {ReadMemSize = RamSize; ThrowSize = 0; }
  else                                   {ReadMemSize = ZVMACHINE_MEMSIZE; ThrowSize = ZVMACHINE_MEMSIZE - RamSize; }

  ReadMemSize>>=2;
  ThrowSize>>=2;

  for(i=0;i<ReadMemSize;i++) Ok &= Stream->Get( ((ULong *)(&Memory))[i]);
  for(i=0;i<ThrowSize;i++)   Ok &= Stream->Get( Dummy);
  if (!Ok) return(false);

  // Load Disassembly tag information

  if (!DisassemblyTags.Load(Stream, ZVMACHINE_MEMSIZE)) return(false);


  // Load the data for the chips

  while(true)
  {
    if (!Ok) return(false);
    Ok &= Stream->Get(String); if(!Ok) return(false);
    if (String == "CHIPEND.") break;

    if (String == "ZCPU_001")
    {

      Ok &= CPU.Load(Stream);        if(!Ok) return(false);
    }
    else if (String == "ZPIA_001")
    {
      Ok &= PIA_1.Load(Stream);        if(!Ok) return(false);
    }
    else if (String == "ZDISP_01")
    {
      Ok &= Display_1.Load(Stream);        if(!Ok) return(false);
    }
    else
    {
      Ok = Stream->Get(SubBlockSize);
      Ok&= Stream->Get(SubBlockVersion);     if(!Ok) return(false);
      SubBlockSize-=2;
      for(ZMemSize i=0;i<SubBlockSize;i++) Ok = Stream->Get(Temp_Byte);
    }
  }

  return(true);

}

