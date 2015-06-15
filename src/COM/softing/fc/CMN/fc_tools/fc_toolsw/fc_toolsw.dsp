# Microsoft Developer Studio Project File - Name="fc_toolsw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=fc_toolsw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fc_toolsw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fc_toolsw.mak" CFG="fc_toolsw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fc_toolsw - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "fc_toolsw - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_toolsw", RQRFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "fc_toolsw - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f fc_toolsw.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_toolsw.dll"
# PROP Bsc_Name "fc_toolsw.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "fc_toolsw - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f fc_toolsw.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_toolsw.exe"
# PROP Bsc_Name "fc_toolsw.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "fc_toolsw - Win32 Release"
# Name "fc_toolsw - Win32 Debug"

!IF  "$(CFG)" == "fc_toolsw - Win32 Release"

!ELSEIF  "$(CFG)" == "fc_toolsw - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\fc_carchive.cpp
# End Source File
# Begin Source File

SOURCE=..\fc_cdump.cpp
# End Source File
# Begin Source File

SOURCE=..\fc_cgarray.c
# End Source File
# Begin Source File

SOURCE=..\fc_crypt.c
# End Source File
# Begin Source File

SOURCE=..\fc_cstring.c
# End Source File
# Begin Source File

SOURCE=..\fc_csvread.cpp
# End Source File
# Begin Source File

SOURCE=..\fc_enumfiles.c
# End Source File
# Begin Source File

SOURCE=..\fc_fileIO.c
# End Source File
# Begin Source File

SOURCE=..\fc_fileIOW.c
# End Source File
# Begin Source File

SOURCE=..\fc_filetomem.c
# End Source File
# Begin Source File

SOURCE=..\fc_hash.c
# End Source File
# Begin Source File

SOURCE=..\fc_regdel.c
# End Source File
# Begin Source File

SOURCE=..\fc_regenum.c
# End Source File
# Begin Source File

SOURCE=..\fc_regex.c
# End Source File
# Begin Source File

SOURCE=..\fc_regval.c
# End Source File
# Begin Source File

SOURCE=..\fc_string.c
# End Source File
# Begin Source File

SOURCE=..\fc_stringFile.c
# End Source File
# Begin Source File

SOURCE=..\fc_system.c
# End Source File
# Begin Source File

SOURCE=.\fc_toolsw.inf
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "inc\fc_tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_carchive.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_cdump.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_cgarray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_crypt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_cstring.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_defines.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_fileIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_hash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_libmem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_registry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_string.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\fc_system.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\fc_tools\README.txt
# End Source File
# End Group
# End Group
# End Target
# End Project
