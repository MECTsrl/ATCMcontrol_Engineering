# Microsoft Developer Studio Project File - Name="4cvmlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=4cvmlib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4cvmlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4cvmlib.mak" CFG="4cvmlib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4cvmlib - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "4cvmlib - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/CCG/4CVMLib", WTUFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "4cvmlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f 4cvmlib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cvm.lib"
# PROP BASE Bsc_Name "4cvmlib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f 4cvmlib.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "c:\4c2\bin\PC\4cvm.exe"
# PROP Bsc_Name "4cvmlib.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "4cvmlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f 4cvmlib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cvm.lib"
# PROP BASE Bsc_Name "4cvmlib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f 4cvmlib.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "d:\4c2\Engineering\bin\4cvm.exe"
# PROP Bsc_Name "4cvmlib.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "4cvmlib - Win32 Release"
# Name "4cvmlib - Win32 Debug"

!IF  "$(CFG)" == "4cvmlib - Win32 Release"

!ELSEIF  "$(CFG)" == "4cvmlib - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4cvm.cpp
# End Source File
# Begin Source File

SOURCE=.\cgmem.cpp
# End Source File
# Begin Source File

SOURCE=.\dump4asm.cpp
# End Source File
# Begin Source File

SOURCE=.\exprasm.cpp
# End Source File
# Begin Source File

SOURCE=.\exprcode.cpp
# End Source File
# Begin Source File

SOURCE=.\globctx.cpp
# End Source File
# Begin Source File

SOURCE=.\helper.cpp
# End Source File
# Begin Source File

SOURCE=.\instview.cpp
# End Source File
# Begin Source File

SOURCE=.\license.cpp
# End Source File
# Begin Source File

SOURCE=.\newdel.cpp
# End Source File
# Begin Source File

SOURCE=.\opexasm.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse4CH.cpp
# End Source File
# Begin Source File

SOURCE=.\parseKAD.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\inc\4cvmAdapt.h
# End Source File
# Begin Source File

SOURCE=..\inc\VmConstants.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cgmem.h
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\dump4asm.h
# End Source File
# Begin Source File

SOURCE=.\exprcode.h
# End Source File
# Begin Source File

SOURCE=.\globctx.h
# End Source File
# Begin Source File

SOURCE=.\helper.h
# End Source File
# Begin Source File

SOURCE=.\instview.h
# End Source File
# Begin Source File

SOURCE=.\license.h
# End Source File
# Begin Source File

SOURCE=.\newdel.h
# End Source File
# Begin Source File

SOURCE=.\Parse4CH.h
# End Source File
# Begin Source File

SOURCE=.\parseKAD.h
# End Source File
# Begin Source File

SOURCE=.\vmasm.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\4cvm.mc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\4cvmlib.inf
# End Source File
# Begin Source File

SOURCE=.\opcodeuse.txt
# End Source File
# End Target
# End Project
