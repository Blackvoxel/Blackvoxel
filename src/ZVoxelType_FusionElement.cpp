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
 * ZVoxelType_FusionElement.cpp
 *
 *  Created on: 4 févr. 2013
 *      Author: laurent
 */

#include "ZVoxelType_FusionElement.h"

#ifndef Z_ZVOXELEXTENSION_FUSIONELEMENT_H
#  include "ZVoxelExtension_FusionElement.h"
#endif

#include <stdio.h>

ZVoxelExtension * ZVoxelType_FusionElement::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_FusionElement;

  return (NewVoxelExtension);
}

void  ZVoxelType_FusionElement::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  ZVoxelExtension_FusionElement * Ext = (ZVoxelExtension_FusionElement *) VoxelInfo;

  printf("------Voxel Information------\n");
  printf("Quantity_Iron   : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Iron]);
  printf("Quantity_Carbon   : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Carbon]);
  printf("Quantity_Chrome   : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Chrome]);
  printf("Quantity_Nickel   : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Nickel]);
  printf("Quantity_Vanadium   : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Vanadium]);
  printf("Quantity_Copper   : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Copper]);
  printf("Quantity_Uranium   : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Uranium]);
  printf("Quantity_Tin     : %lf\n",Ext->Quantity[ZVoxelExtension_FusionElement::Material_Tin]);
  printf("Temperature : %lf°C\n", Ext->Temperature);

}

void ZVoxelType_FusionElement::GetScanInformations(ZVoxelCoords * VoxelCoords, UShort VoxelType, ZMemSize VoxelInfo, ZString & Inf)
{
  char Buffer[1024];
  ZVoxelExtension_FusionElement * Ext = (ZVoxelExtension_FusionElement *) VoxelInfo;
  Inf.Append_pchar("TEMP : "); sprintf(Buffer, "%.02f°\n\n", Ext->Temperature);     Inf.Append_pchar(Buffer);
  Inf.Append_pchar("Metalurgical Analysis\n");
  Inf.Append_pchar("\nIRON     : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Iron]);     Inf.Append_pchar(Buffer);
  Inf.Append_pchar("\nCARBON   : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Carbon]);   Inf.Append_pchar(Buffer);
  Inf.Append_pchar("\nCHROME   : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Chrome]);   Inf.Append_pchar(Buffer);
  Inf.Append_pchar("\nNICKEL   : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Nickel]);   Inf.Append_pchar(Buffer);
  Inf.Append_pchar("\nVANADIUM : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Vanadium]); Inf.Append_pchar(Buffer);
  Inf.Append_pchar("\nCOPPER   : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Copper]);   Inf.Append_pchar(Buffer);
  Inf.Append_pchar("\nTIN      : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Tin]);      Inf.Append_pchar(Buffer);
  Inf.Append_pchar("\nURANIUM  : ");  sprintf(Buffer, "%.02f", Ext->Quantity[ZVoxelExtension_FusionElement::Material_Uranium]);  Inf.Append_pchar(Buffer);
}
