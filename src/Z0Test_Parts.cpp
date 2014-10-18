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

bool ZTest_Parts::RunTestCode()
{
  printf("Test Parts");

  ZString Text;

  Text = "  move.b #10,r0";

  ZMacroAssembler Mass;

  Mass.Assemble(Text);


  return(false);
}


