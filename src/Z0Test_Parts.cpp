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
 * ZTest_Parts.cpp
 *
 *  Created on: 23 sept. 2014
 *      Author: laurent
 */

#include "Z0Test_Parts.h"
#include <stdio.h>

#ifndef Z_MACROASSEMBLER_H
#  include "ZMacroAssembler.h"
#endif

#ifndef Z_ZVCPU_H
#  include "ZVCPU.h"
#endif

#ifndef Z_ZVMACHINE_T1_H
#  include "ZVMachine_T1.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

#ifndef Z_BITMAP_IMAGE_H
#  include "z/ZBitmapImage.h"
#endif


//#ifndef Z_ZVCPU_H
//#  include "ZVCPU.h"
//#endif

#ifndef Z_ZHARDWAREDEPENDENT_H
#  include "ZOs_Specific_HardwareDependent.h"
#endif

    class ParsedRequest
    {
      public:
        bool ParseRequest(ZString & Text)
        {
          ZString Txt,Txt2,Arg;
          ULong i;
          Bool Res, Stop;

          Txt = Text;

          Res = Txt.Split(' ',RequestType);  if (!Res) return(false);
          Txt2 = Txt;
          Res = Txt2.Split(' ',Txt); if (!Res) return(false);
          Res = Txt2.Split('\n', Proto); if (!Res) return(false);
          Txt.StripLeading('/');

          if (!Txt.Split('?', Command)) {Command = Txt; return(true);}

          for (i=0, Stop=false;i<16 && !Stop;i++)
          {
            if (!Txt.Split('&',Arg)) {Arg = Txt; Stop = true;}
            if (!Arg.Split('=', Arg_Name[i])) break;
            Arg_Value[i] = Arg;
          }

          return(true);
        }

        void Dump()
        {
          ULong i;

          printf("-----------------------\n");
          printf("Request Type :%s|\n",RequestType.String);
          printf("Command      :%s|\n",Command.String);
          printf("Proto        :%s|\n",Proto.String);

          for(i=0;i<16;i++)
          {
            printf("%s=%s\n",Arg_Name[i].String, Arg_Value[i].String);
          }

        }

        bool FindEntry(char * Name, ULong & Num)
        {
          ULong i;

          for (i=0;i<16;i++)
          {
            if (Arg_Name[i]==Name) {Num = i; return(true);};
          }

          return(false);
        }


        ZString RequestType;
        ZString Command;
        ZString Proto;
        ZString Arg_Name[16];
        ZString Arg_Value[16];
    };


bool ZTest_Parts::RunTestCode()
{

  ZStream_File Stream;
  ZString Text;
  bool Result;

  ParsedRequest Pr;

  Stream.SetFileName("/home/laurent/workspace/a_newblackvoxel/Devdoc/Scratch_Request_Example.txt");
  Result = Stream.GetFileContent(Text);

  Pr.ParseRequest(Text);
  Pr.Dump();




  //Bmp.SetPixel()

//  ZTCPNet_Socket::Test();


  // ZHardwareInfo Hi;

  // Hi.DetectHardware();

  /*
  ZNet_SocketTest Test;

  Test.TestReceive();
  */
/*
  ZBitmapImage Bmp;
  long x,y;
  unsigned char xp,yp;
  ULong i;


  Bmp.CreateBitmap(512,256,32);
  Bmp.Clear();
  return(true);
  for (y=0;y<256;y++)
  {
    for (x=0;x<256;x++)
    {
      xp = x; yp =y;
      long v;
      v=x-y;
      if ( (v<0) || (v>255) )    Bmp.SetPixel(x,y,0,0,255,0);
      else                       Bmp.SetPixel(x,y,255,255,255,0);

      unsigned char vp;
      vp =xp-yp;
      if (vp>xp)  Bmp.SetPixel(x+256,y,0,0,255,0);
      else                          Bmp.SetPixel(x+256,y,255,255,255,0);

    }
  }

  for (i=0;i<256;i++)
  {
    Bmp.SetPixel(i,128,0,0,0,0);
    Bmp.SetPixel(128,i,0,0,0,0);
  }

  Bmp.SaveBMP("Out/test.bmp");
*/
  return(false);
}


