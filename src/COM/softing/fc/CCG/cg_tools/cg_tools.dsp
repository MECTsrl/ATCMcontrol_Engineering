# Microsoft Developer Studio Project File - Name="cg_tools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=cg_tools - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cg_tools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cg_tools.mak" CFG="cg_tools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cg_tools - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "cg_tools - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CCG/cg_tools", DQLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "cg_tools - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f cg_tools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cg_tools.exe"
# PROP BASE Bsc_Name "cg_tools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f cg_tools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cg_tools.exe"
# PROP Bsc_Name "cg_tools.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "cg_tools - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f cg_tools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cg_tools.exe"
# PROP BASE Bsc_Name "cg_tools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f cg_tools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cg_tools.exe"
# PROP Bsc_Name "cg_tools.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "cg_tools - Win32 Release"
# Name "cg_tools - Win32 Debug"

!IF  "$(CFG)" == "cg_tools - Win32 Release"

!ELSEIF  "$(CFG)" == "cg_tools - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cgt_address.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_CDiagMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_checkVarExternals.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_csvread.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_dmp.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_dumpdbi.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_enumtostr.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_expr.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_link.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_literal.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_misc.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_readlink.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_tools.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_varextacc.cpp
# End Source File
# Begin Source File

SOURCE=.\cgt_xmlparse.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "cg_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\inc\cgt_address.h
# End Source File
# Begin Source File

SOURCE=..\inc\cgt_CDiagMsg.h
# End Source File
# Begin Source File

SOURCE=..\inc\cgt_link.h
# End Source File
# Begin Source File

SOURCE=..\inc\cgt_literal.h
# End Source File
# Begin Source File

SOURCE=..\inc\cgt_misc.h
# End Source File
# Begin Source File

SOURCE=..\inc\cgt_tools.h
# End Source File
# Begin Source File

SOURCE=..\inc\cgt_xmlparse.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XML_dbi_names.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cg_tools.inf
# End Source File
# End Target
# End Project
