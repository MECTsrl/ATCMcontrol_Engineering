# Microsoft Developer Studio Project File - Name="bzip2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=bzip2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bzip2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bzip2.mak" CFG="bzip2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bzip2 - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "bzip2 - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/Bzip2", HNTFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "bzip2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "bzip2___Win32_Release"
# PROP BASE Intermediate_Dir "bzip2___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f bzip2.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "bzip2.exe"
# PROP BASE Bsc_Name "bzip2.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bzip2___Win32_Release"
# PROP Intermediate_Dir "bzip2___Win32_Release"
# PROP Cmd_Line "NMAKE /f bzip2.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "bzip2.exe"
# PROP Bsc_Name "bzip2.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "bzip2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "bzip2___Win32_Debug"
# PROP BASE Intermediate_Dir "bzip2___Win32_Debug"
# PROP BASE Cmd_Line "NMAKE /f bzip2.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "bzip2.exe"
# PROP BASE Bsc_Name "bzip2.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bzip2___Win32_Debug"
# PROP Intermediate_Dir "bzip2___Win32_Debug"
# PROP Cmd_Line "NMAKE /f bzip2.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "bzip2.exe"
# PROP Bsc_Name "bzip2.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "bzip2 - Win32 Release"
# Name "bzip2 - Win32 Debug"

!IF  "$(CFG)" == "bzip2 - Win32 Release"

!ELSEIF  "$(CFG)" == "bzip2 - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\blocksort.c
# End Source File
# Begin Source File

SOURCE=.\bzip2.c
# End Source File
# Begin Source File

SOURCE=.\bzlib.c
# End Source File
# Begin Source File

SOURCE=.\compress.c
# End Source File
# Begin Source File

SOURCE=.\crctable.c
# End Source File
# Begin Source File

SOURCE=.\decompress.c
# End Source File
# Begin Source File

SOURCE=.\huffman.c
# End Source File
# Begin Source File

SOURCE=.\randtable.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bzlib.h
# End Source File
# Begin Source File

SOURCE=.\bzlib_private.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\bzip2.inf
# End Source File
# End Target
# End Project
