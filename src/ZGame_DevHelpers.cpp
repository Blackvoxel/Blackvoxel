/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2015 Laurent Thiebaut & Olivia Merle
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
 * ZGame_DevHelpers.cpp
 *
 *  Created on: 6 janv. 2015
 *      Author: laurent
 */

#ifndef Z_ZGAME_DEVHELPERS_H
#  include "ZGame_DevHelpers.h"
#endif

#ifndef Z_ZVCPU_H
#  include "ZVCPU.h"
#endif


bool ZGame_DevHelpers::OutputHelpers(ZString & HelperDirectory)
{
  ZString Out, FileName;

  // Output BlackCPU V1 Opcode table

  FileName = HelperDirectory;
  FileName.AddToPath("BlackCPU_V1_OpcodeList.txt");
  Out.Clear();
  Out = BlackCPU<int>::OutputOpcodeDatabase(0);
  if (!Out.SaveToFile(FileName.String)) return(false);

  // Output BlackCPU V1 Opcode table (SQL version);

  FileName = HelperDirectory;
  FileName.AddToPath("BlackCPU_V1_OpcodeList.sql");
  Out.Clear();
  Out = BlackCPU<int>::OutputOpcodeDatabase(1);
  if (!Out.SaveToFile(FileName.String)) return(false);

  // Outpur BlackCPU V1 Opcode table (Compact html version)

  FileName = HelperDirectory;
  FileName.AddToPath("BlackCPU_V1_OpcodeListCompact.htm");
  Out.Clear();
  Out = BlackCPU<int>::OutputOpcodeDatabase(2);
  if (!Out.SaveToFile(FileName.String)) return(false);


  return(true);
}
