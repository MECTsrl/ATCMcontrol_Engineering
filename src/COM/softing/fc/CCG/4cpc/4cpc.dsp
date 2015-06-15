# Microsoft Developer Studio Project File - Name="4cpc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=4cpc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4cpc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4cpc.mak" CFG="4cpc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4cpc - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "4cpc - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CCG/4cpc", PKLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "4cpc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f 4cpc.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cpc.exe"
# PROP BASE Bsc_Name "4cpc.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f 4cpc.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "c:\4c2\bin\PC\4cpc.exe"
# PROP Bsc_Name "4cpc.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "4cpc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f 4cpc.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cpc.exe"
# PROP BASE Bsc_Name "4cpc.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f 4cpc.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "c:\4c2\Engineering\bin\4cpc\4cpc.exe "
# PROP Bsc_Name "4cpc.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "4cpc - Win32 Release"
# Name "4cpc - Win32 Debug"

!IF  "$(CFG)" == "4cpc - Win32 Release"

!ELSEIF  "$(CFG)" == "4cpc - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4cpc.cpp
# End Source File
# Begin Source File

SOURCE=.\4cpc.inf
# End Source File
# Begin Source File

SOURCE=.\dumpjava.cpp
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

SOURCE=.\Parse4CH.cpp
# End Source File
# Begin Source File

SOURCE=.\parseKAD.cpp
# End Source File
# Begin Source File

SOURCE=.\PC_InitFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\resinit.cpp
# End Source File
# Begin Source File

SOURCE=.\rtcegen.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\inc\CG_IECFront.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\dumpjava.h
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

SOURCE=.\PC_InitFiles.h
# End Source File
# Begin Source File

SOURCE=.\resinit.h
# End Source File
# Begin Source File

SOURCE=.\rtcegen.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\4cpc.mc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# End Target
# End Project
