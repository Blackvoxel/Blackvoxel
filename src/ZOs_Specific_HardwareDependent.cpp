/*
 * ZOs_Specific_HardwareDependent.cpp
 *
 *  Created on: 22 mars 2016
 *      Author: laurent
 */

#ifndef Z_ZHARDWAREDEPENDENT_H
#  include "ZOs_Specific_HardwareDependent.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

char const * ZHardwareInfo::String_Profile[] =
{
  "X86_GENERAL_PROFILE",
  "X86_NETBOOK_PROFILE",
  "ARM_GENERAL_PROFILE",
  "ARM_RASPBERRY_PI_1",
  "ARM_RASPBERRY_PI_2",
  "ARM_RASPBERRY_PI_3"
};


void ZHardwareInfo::DetectHardware()
{
  Detect_CPU();
}


void ZHardwareInfo::Detect_CPU()
{
  #ifdef ZENV_OS_LINUX
      Detect_CPU_Linux();
  #endif
  #ifdef ZENV_OS_WINDOWS
      Detect_CPU_Windows();
  #endif
}

void ZHardwareInfo::Detect_CPU_Linux()
{
  ZStream_File File;
  ZString FileContent, Key, Value;

  // Detecting Arm/X86

  #ifdef __arm__
  CpuType = CPUTYPE_ARM;
  HardwareProfile = ARM_GENERAL_PROFILE;
  #else
  CpuType = CPUTYPE_X86;
  HardwareProfile = X86_GENERAL_PROFILE;
  #endif


  ZString Info_CPU_Implementer;

  File.SetFileName("/proc/cpuinfo");
  if (!File.GetFileContent(FileContent)) return;

  while (FileContent.Split('\n', Value))
  {
    Value.Split(':',Key);
    Key.StripTrailling('\t');
    Value.StripLeading(' ');

    // Raspberry pi special profile detection
    if (Key=="CPU implementer") Info_CPU_Implementer = Value;
    if (Key=="CPU part" && Info_CPU_Implementer=="0x41")
    {
      if (Value == "0xb76") HardwareProfile = ARM_RASPBERRY_PI_1;
      if (Value == "0xc07") HardwareProfile = ARM_RASPBERRY_PI_2;
      if (Value == "0xd03") HardwareProfile = ARM_RASPBERRY_PI_3;
    }
  }

}

void ZHardwareInfo::Detect_CPU_Windows()
{
  // No detection on Windows at this time.
  // Only x86 and general profile is supported.

  CpuType = CPUTYPE_X86;
  HardwareProfile = X86_GENERAL_PROFILE;
}

