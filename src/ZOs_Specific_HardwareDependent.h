/*
 * ZOs_Specific_HardwareDependent.h
 *
 *  Created on: 21 mars 2016
 *      Author: laurent
 */

#ifndef Z_ZHARDWAREDEPENDENT_H
#define Z_ZHARDWAREDEPENDENT_H

//#ifndef Z_ZHARDWAREDEPENDENT_H
//#  include "ZOs_Specific_HardwareDependent.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZHardwareInfo
{
  protected:

    // Platform specific processor detection
    void Detect_CPU_Linux();
    void Detect_CPU_Windows();
    void Detect_CPU_OSX();
    static char const * String_Profile[];
    // Actions

    void Detect_CPU();

  public:
    enum {CPUTYPE_X86, CPUTYPE_ARM} CpuType;
    enum {X86_GENERAL_PROFILE, X86_NETBOOK_PROFILE, ARM_GENERAL_PROFILE, ARM_RASPBERRY_PI_1, ARM_RASPBERRY_PI_2, ARM_RASPBERRY_PI_3} HardwareProfile;

    char * GetProfileText() { return ((char *)String_Profile[HardwareProfile]);}

    void DetectHardware();

};





#endif /* Z_ZHARDWAREDEPENDENT_H */
