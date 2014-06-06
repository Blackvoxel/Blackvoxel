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
 * ZRandom_MakeGoodNumbers.h
 *
 *  Created on: 17 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZRANDOM_MAKEGOODNUMBERS_H
#define Z_ZRANDOM_MAKEGOODNUMBERS_H

//#ifndef Z_ZRANDOM_MAKEGOODNUMBERS_H
//#  include "ZRandom_MakeGoodNumbers.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

class ZRandom_MakeGoodNumbers
{
  ZString Input_FileName;
  ZString Output_FileName;

  public:

    void Set_InputFileName(char const * FileName)  { Input_FileName  = FileName; }
    void Set_OutputFileName(char const * FileName) { Output_FileName = FileName; }

    bool Run(ULong Quantity)
    {
      ZStream_File InputFile, OutputFile;
      ULong * Mem;

      ULong Data, OldData, i,j;

      Mem = new ULong[Quantity]; if (Mem ==0) return(false);

      InputFile.SetFileName(Input_FileName.String);
      OutputFile.SetFileName(Output_FileName.String);

      if (!InputFile.OpenRead()) return(false);
      if (Quantity!=InputFile.ReadObj(Mem,4,Quantity)) {return(false);}
      InputFile.Close();

      for (i=0; i<Quantity; i++)
      {
        Data = Mem[i];

        for (j=0;j<64 + (OldData & 15); j++) Data = (Data >> 1) ^ (-(Data & 1ul) & 0xD0000001ul);

        Mem[i]=Data;
      }

      if (!OutputFile.OpenWrite()) return(false);
      if (Quantity!=OutputFile.WriteObj(Mem,4,Quantity)) return(false);
      OutputFile.Close();

      return(true);



    }



};




#endif /* Z_ZRANDOM_MAKEGOODNUMBERS_H */
