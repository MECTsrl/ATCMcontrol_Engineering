# Microsoft Developer Studio Project File - Name="fc_xml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=fc_xml - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fc_xml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fc_xml.mak" CFG="fc_xml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fc_xml - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "fc_xml - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_xml", FSRFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "fc_xml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f fc_xml.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_xml.exe"
# PROP BASE Bsc_Name "fc_xml.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f fc_xml.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_xml.exe"
# PROP Bsc_Name "fc_xml.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "fc_xml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f fc_xml.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_xml.exe"
# PROP BASE Bsc_Name "fc_xml.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f fc_xml.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_xml.exe"
# PROP Bsc_Name "fc_xml.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "fc_xml - Win32 Release"
# Name "fc_xml - Win32 Debug"

!IF  "$(CFG)" == "fc_xml - Win32 Release"

!ELSEIF  "$(CFG)" == "fc_xml - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fc_xml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\fc_xml.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\fc_xml.inf
# End Source File
# Begin Source File

SOURCE=.\fc_xml_Error.mc
# End Source File
# End Target
# End Project
