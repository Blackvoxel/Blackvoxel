/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZWebSimpleRequestParser.cpp
 *
 *  Created on: 25 mai 2017
 *      Author: laurent
 */

#include "ZSimpleRequestParser.h"
#include <stdio.h>

bool ZSimpleRequestParser::ParseRequest(ZString & Text)
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

void ZSimpleRequestParser::Dump()
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

bool ZSimpleRequestParser::FindEntry(char * Name, ULong & Num)
{
  ULong i;

  for (i=0;i<16;i++)
  {
    if (Arg_Name[i]==Name) {Num = i; return(true);};
  }

  return(false);
}

bool ZSimpleRequestParser::FindEntryText(char * Name, ZString & Text)
{
  ULong i;

  for (i=0;i<16;i++)
  {
    if (Arg_Name[i]==Name) { Text = Arg_Value[i]; return(true);};
  }

  return(false);
}

