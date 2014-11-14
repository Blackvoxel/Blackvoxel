/*
 * ZVMachine_T1.h
 *
 *  Created on: 7 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZVMACHINE_T1_H
#define Z_ZVMACHINE_T1_H

//#ifndef Z_ZVMACHINE_T1_H
//#  include "ZVMachine_T1.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#define ZVMACHINE_MEMSIZE 65536

class ZVMachine_T1
{

   UByte Memory[ZVMACHINE_MEMSIZE];



   void Envcall() {}

   inline UByte FetchOpcode(ULong Address)
   {
     return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
   }

   inline UByte FetchOpcode_8(ULong Address)
   {
     return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
   }

   inline UByte FetchOpcode_16(ULong Address)
   {
     return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
   }

   inline UByte FetchOpcode_32(ULong Address)
   {
     return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
   }

   inline UByte ReadMemory_8(ULong Address)
   {
     if (!(Address >> 31))
     {
       return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
     }
     else
     {
       switch(Address >> 27)
       {
         case 0x10: return(0);
         default:   return(0);
       }
     }
   }

   inline UByte ReadMemory_16(ULong Address)
   {
     if (!(Address >> 31))
     {
       return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
     }
     else
     {
       switch(Address >> 27)
       {
         case 0x10: return(0);
         default:   return(0);
       }
     }
   }

   inline UByte ReadMemory_32(ULong Address)
   {
     if (!(Address >> 31))
     {
       return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
     }
     else
     {
       switch(Address >> 27)
       {
         case 0x10: return(0);
         default:   return(0);
       }
     }
   }
};

#endif /* Z_ZVMACHINE_T1_H */
