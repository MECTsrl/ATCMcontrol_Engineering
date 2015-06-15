# Microsoft Developer Studio Project File - Name="asm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=asm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "asm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "asm.mak" CFG="asm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "asm - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "asm - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/CCG/asm", DVUFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "asm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f asm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "asm.exe"
# PROP BASE Bsc_Name "asm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f asm.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "asm.exe"
# PROP Bsc_Name "asm.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "asm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f asm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "asm.exe"
# PROP BASE Bsc_Name "asm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f asm.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "asm.exe"
# PROP Bsc_Name "asm.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "asm - Win32 Release"
# Name "asm - Win32 Debug"

!IF  "$(CFG)" == "asm - Win32 Release"

!ELSEIF  "$(CFG)" == "asm - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\asm.y
# End Source File
# Begin Source File

SOURCE=.\asm_lex.l
# End Source File
# Begin Source File

SOURCE=.\asm_ParseContext.cpp
# End Source File
# Begin Source File

SOURCE=.\asmcps.cpp
# End Source File
# Begin Source File

SOURCE=.\asmerr.cpp
# End Source File
# Begin Source File

SOURCE=.\asmmain.cpp
# End Source File
# Begin Source File

SOURCE=.\asmstmt.cpp
# End Source File
# Begin Source File

SOURCE=.\asmtools.cpp
# End Source File
# Begin Source File

SOURCE=.\asmtype.cpp
# End Source File
# Begin Source File

SOURCE=.\globalContext.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\asm_lex.h
# End Source File
# Begin Source File

SOURCE=.\asm_ParseContext.h
# End Source File
# Begin Source File

SOURCE=.\asmcps.h
# End Source File
# Begin Source File

SOURCE=.\asmerr.h
# End Source File
# Begin Source File

SOURCE=.\AsmParse.h
# End Source File
# Begin Source File

SOURCE=.\asmstmt.h
# End Source File
# Begin Source File

SOURCE=.\asmtools.h
# End Source File
# Begin Source File

SOURCE=.\asmtype.h
# End Source File
# Begin Source File

SOURCE=.\globalContext.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\asm.rc
# End Source File
# Begin Source File

SOURCE=.\ASMmessages.mc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\asm.inf
# End Source File
# End Target
# End Project
