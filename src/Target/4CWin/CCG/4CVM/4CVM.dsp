# Microsoft Developer Studio Project File - Name="4CVM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=4CVM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4CVM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4CVM.mak" CFG="4CVM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4CVM - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "4CVM - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/Target/4CWin/CCG/4CVM", XGXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "4CVM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f 4cvm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cvm.exe"
# PROP BASE Bsc_Name "4cvm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f 4cvm.mak /nologo "DEBUG=0""
# PROP Rebuild_Opt "clean all"
# PROP Target_File "4cvm.exe"
# PROP Bsc_Name "4cvm.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "4CVM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f 4cvm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cvm.exe"
# PROP BASE Bsc_Name "4cvm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f 4cvm.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "4cvm.exe"
# PROP Bsc_Name "4cvm.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "4CVM - Win32 Release"
# Name "4CVM - Win32 Debug"

!IF  "$(CFG)" == "4CVM - Win32 Release"

!ELSEIF  "$(CFG)" == "4CVM - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4CVMAdapt.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\4CVMResource.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\4CVMResource.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\4CVM.inf
# End Source File
# End Target
# End Project
