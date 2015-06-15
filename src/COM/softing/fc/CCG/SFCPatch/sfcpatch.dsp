# Microsoft Developer Studio Project File - Name="SFCPatch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=SFCPatch - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sfcpatch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sfcpatch.mak" CFG="SFCPatch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SFCPatch - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "SFCPatch - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CCG/SFCPatch", VRLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "SFCPatch - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f sfcpatch.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "sfcpatch.exe"
# PROP BASE Bsc_Name "sfcpatch.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f sfcpatch.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "sfcpatch.exe"
# PROP Bsc_Name "sfcpatch.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SFCPatch - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f sfcpatch.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "sfcpatch.exe"
# PROP BASE Bsc_Name "sfcpatch.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f sfcpatch.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "sfcpatch.exe"
# PROP Bsc_Name "sfcpatch.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "SFCPatch - Win32 Release"
# Name "SFCPatch - Win32 Debug"

!IF  "$(CFG)" == "SFCPatch - Win32 Release"

!ELSEIF  "$(CFG)" == "SFCPatch - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\msXML_2.IDL
# End Source File
# Begin Source File

SOURCE=.\PatchCommandLine.cpp
# End Source File
# Begin Source File

SOURCE=.\SFCElem.cpp
# End Source File
# Begin Source File

SOURCE=.\SFCPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\SFCPatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\sfcpatchressource.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Dings.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Help.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\PatchCommandLine.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SFCElem.h
# End Source File
# Begin Source File

SOURCE=.\SFCPatcher.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\XML_Dings.h
# End Source File
# Begin Source File

SOURCE=.\XML_Help.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\SFCPatch.inf
# End Source File
# End Target
# End Project
