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
 * ZVoxelExtension_FusionElement.h
 *
 *  Created on: 4 févr. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_FUSIONELEMENT_H
#define Z_ZVOXELEXTENSION_FUSIONELEMENT_H

//#ifndef Z_ZVOXELEXTENSION_FUSIONELEMENT_H
//#  include "ZVoxelExtension_FusionElement.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_FusionElement : public ZVoxelExtension
{
  public:
    enum {Material_Iron, Material_Carbon, Material_Chrome,Material_Nickel, Material_Vanadium, Material_Copper, Material_Uranium,
          Material_Titanium, Material_Aluminium, Material_Tin, Material_Lead};
    enum {Material_MaxQuantity = 16};
    double Quantity[Material_MaxQuantity];
    /*
    double Quantity_Iron;
    double Quantity_Carbon;
    double Quantity_Chrome;
    double Quantity_Nickel;
    double Quantity_Vanadium;
    double Quantity_Copper;
    double Quantity_Uranium;
    */
    double Temperature;
  public:

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_FusionElement * NewCopy;
    NewCopy = new ZVoxelExtension_FusionElement(*this);
    return(NewCopy);
  }

  ZVoxelExtension_FusionElement()
  {
    ULong i;

    ExtensionType = Extension_FusionElement;
/*
    Quantity[Material_Iron]     = 0.0;
    Quantity[Material_Carbon]   = 0.0;
    Quantity[Material_Chrome]   = 0.0;
    Quantity[Material_Nickel]   = 0.0;
    Quantity[Material_Vanadium] = 0.0;
    Quantity[Material_Copper]   = 0.0;
    Quantity[Material_Uranium]  = 0.0;
  */
    for (i=0;i<Material_MaxQuantity;i++) Quantity[i] = 0.0;
    Temperature = 20.0;
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('F','U','S','E') );
  }

  virtual bool Save(ZStream_SpecialRamStream * Stream)
  {
    ULong * ExtensionSize;
    ULong   StartLen, i;

    ExtensionSize = Stream->GetPointer_ULong();
    Stream->Put(0u);       // The size of the extension (defered storage).
    StartLen = Stream->GetActualBufferLen();
    Stream->Put((UShort)1); // Extension Version;

    // Storage informations.
    for (i=0;i<Material_MaxQuantity;i++) Stream->Put(Quantity[i]);
    Stream->Put(Temperature);
    *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

    return(true);
  }

  virtual bool Load(ZStream_SpecialRamStream * Stream)
  {
    bool Ok;
    ULong  ExtensionSize, i;
    UShort ExtensionVersion;
    UByte  Temp_Byte;

    Ok = Stream->Get(ExtensionSize);
    Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

    // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

    if (ExtensionVersion!=1) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

    for (i=0;i<Material_MaxQuantity;i++) Stream->Get(Quantity[i]);
    Stream->Get(Temperature);
    return(Ok);
  }

  void Blend( ZVoxelExtension_FusionElement * Other )
  {
    double Seuil = 0.001;

    ULong i;
    register double m;

    for (i=0;i<Material_MaxQuantity;i++)
    {
      if ( fabs(Quantity[i] - Other->Quantity[i]) > Seuil )
      {
        m = (Quantity[i] + Other->Quantity[i]) / 2.0;
        Quantity[i] = Other->Quantity[i] = m;
      }
    }

    // Temperature blending

    if (fabs(Temperature - Other->Temperature) > Seuil)
    {
      m = (Temperature + Other->Temperature) / 2.0;
      Temperature = Other->Temperature = m;
    }

  }

  UShort Solidify()
  {
    ULong BaseMaterial,i;
    double RefQuantity;
    UShort ResultMaterial;

    // Find the dominant material

    for (i=0, RefQuantity = 0.0, BaseMaterial = 65536 ; i<Material_MaxQuantity ; i++)
    {
      if (Quantity[i] > RefQuantity) {RefQuantity = Quantity[i]; BaseMaterial = i; }
    }
    if (BaseMaterial == 65536) return(116); // Crap

    ResultMaterial = 116;

    // Alliages ferreux

    if (BaseMaterial == Material_Iron)
    {
      //
      if (Quantity[Material_Uranium] > 0.0) return(116); // Crap

      if (Quantity[Material_Carbon] <= 2.0) // Acier
      {
        if (Quantity[Material_Vanadium] > 2.0) return(116);
        if (Quantity[Material_Copper] > 10.0) return(116);
        if (Quantity[Material_Chrome] > 25.0) return(116);
        if (Quantity[Material_Nickel] > 15.0) return(116);
        if (Quantity[Material_Nickel] >6.0 && Quantity[Material_Nickel] <= 15.0 && Quantity[Material_Chrome] >15.0 && Quantity[Material_Chrome] <= 25.0) return(118); // Acier Inox
        return(117); // Acier.
      }
      else
      {
        if (Quantity[Material_Carbon] >= 7.0)  return(116); // Crap
        if (Quantity[Material_Vanadium] > 2.0) return(116);
        if (Quantity[Material_Copper] > 10.0)  return(116);
        if (Quantity[Material_Chrome] > 25.0)  return(116);
        if (Quantity[Material_Nickel] > 15.0)  return(116);
        return(120); // Fonte
      }

    }

    // Alliages de cuivre.

    if (BaseMaterial == Material_Copper)
    {
      for (i=0;i<Material_MaxQuantity;i++)
      {
         if (i!=Material_Copper && i!=Material_Carbon && i!=Material_Tin && Quantity[i]>0.0) return(116); // Crap;
      }
      if (Quantity[Material_Carbon]>1.5) return(116); // Crap
      if (Quantity[Material_Tin] >20.0)  return(116); // Crap
      if (Quantity[Material_Tin] <8.0)   return(119); // Copper
      else                               return(128); // Bronze
    }

    // Alliage de titane

    if (BaseMaterial == Material_Titanium)
    {
      for (i=0;i<Material_MaxQuantity;i++)
      {
         if (i!=Material_Titanium && i!=Material_Carbon && Quantity[i]>0.0) return(116); // Crap;
      }
      if (Quantity[Material_Carbon]>1.0) return(116);
      return(124);
    }

    // Alliage d'aluminium

    if (BaseMaterial == Material_Aluminium)
    {
      for (i=0;i<Material_MaxQuantity;i++)
      {
         if (i!=Material_Aluminium && i!=Material_Carbon && Quantity[i]>0.0) return(116); // Crap;
      }
      if (Quantity[Material_Carbon]>1.0) return(116);
      return(125);
    }

    // Alliage de plomb

    if (BaseMaterial == Material_Lead)
    {
      for (i=0;i<Material_MaxQuantity;i++)
      {
         if (i!=Material_Lead && i!=Material_Carbon && Quantity[i]>0.0) return(116); // Crap;
      }
      if (Quantity[Material_Carbon]>1.0) return(116);
      return(151);
    }

    if (BaseMaterial == Material_Tin)
    {
      for (i=0;i<Material_MaxQuantity;i++)
      {
        if (i!=Material_Tin && i!=Material_Carbon && Quantity[i]>0.0) return(116); // Crap;
      }
      if (Quantity[Material_Carbon]>1.5) return(116); // Crap
      return(129);
    }


    return(ResultMaterial);
  }
};



#endif /* Z_ZVOXELEXTENSION_FUSIONELEMENT_H */
