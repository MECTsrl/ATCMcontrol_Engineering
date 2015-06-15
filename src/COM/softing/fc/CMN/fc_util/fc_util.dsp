# Microsoft Developer Studio Project File - Name="UTIL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=UTIL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fc_util.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fc_util.mak" CFG="UTIL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UTIL - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "UTIL - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util", YQRFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "UTIL - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f fc_util.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_util.dll"
# PROP BASE Bsc_Name "fc_util.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f fc_util.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_util.dll"
# PROP Bsc_Name "fc_util.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "UTIL - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f fc_util.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_util.dll"
# PROP BASE Bsc_Name "util.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f fc_util.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_util.dll"
# PROP Bsc_Name "fc_util.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "UTIL - Win32 Release"
# Name "UTIL - Win32 Debug"

!IF  "$(CFG)" == "UTIL - Win32 Release"

!ELSEIF  "$(CFG)" == "UTIL - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fc_util.inf
# End Source File
# Begin Source File

SOURCE=.\fcpump.cpp
# End Source File
# Begin Source File

SOURCE=.\genkwmap.pl
# End Source File
# Begin Source File

SOURCE=.\ice.cpp
# End Source File
# Begin Source File

SOURCE=.\iecsyntax.l
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\OsVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\UTIL.def
# End Source File
# Begin Source File

SOURCE=.\utilfile.cpp
# End Source File
# Begin Source File

SOURCE=.\utilif.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLencode.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\inc\fcpump.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\regex.h
# End Source File
# Begin Source File

SOURCE=..\..\Inc\utilif.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\utilsts.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ice.h
# End Source File
# Begin Source File

SOURCE=.\IECkeywords.txt
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\UTIL.rc
# End Source File
# End Group
# Begin Group "Regex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Regex\cclass.h
# End Source File
# Begin Source File

SOURCE=.\Regex\cname.h
# End Source File
# Begin Source File

SOURCE=.\Regex\engine.c2
# End Source File
# Begin Source File

SOURCE=.\Regex\regcomp.c
# End Source File
# Begin Source File

SOURCE=.\Regex\regerror.c
# End Source File
# Begin Source File

SOURCE=.\Regex\regex2.h
# End Source File
# Begin Source File

SOURCE=.\Regex\regexec.c
# End Source File
# Begin Source File

SOURCE=.\Regex\regfree.c
# End Source File
# Begin Source File

SOURCE=.\Regex\utils.h
# End Source File
# End Group
# End Target
# End Project
