# Microsoft Developer Studio Project File - Name="BackParserDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=BackParserDLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BackParserDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BackParserDLL.mak" CFG="BackParserDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BackParserDLL - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "BackParserDLL - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEBackparser/BackParserDLL", VIMFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "BackParserDLL - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f backparserdll.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "backparserdll.exe"
# PROP BASE Bsc_Name "backparserdll.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f backparserdll.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "backparserdll.dll"
# PROP Bsc_Name "backparserdll.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "BackParserDLL - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f backparserdll.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "backparserdll.exe"
# PROP BASE Bsc_Name "backparserdll.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f backparserdll.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "backparserdll.dll"
# PROP Bsc_Name "backparserdll.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "BackParserDLL - Win32 Release"
# Name "BackParserDLL - Win32 Debug"

!IF  "$(CFG)" == "BackParserDLL - Win32 Release"

!ELSEIF  "$(CFG)" == "BackParserDLL - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BackParserDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\BackParserDLL.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\BackParserDLL.rc
# End Source File
# Begin Source File

SOURCE=.\CE_SRCOPS_IEC_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\CE_SRCOPS_IEC_Scanner.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassViewDescrGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\Configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\DllInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\EmptyFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Function.cpp
# End Source File
# Begin Source File

SOURCE=.\FunctionBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobVarConstList.cpp
# End Source File
# Begin Source File

SOURCE=.\IECSRCVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportDirective.cpp
# End Source File
# Begin Source File

SOURCE=.\LexVal.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserContext.cpp
# End Source File
# Begin Source File

SOURCE=.\POU.CPP
# End Source File
# Begin Source File

SOURCE=.\PouParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Program.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgramConfiguration.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StreamScan.cpp
# End Source File
# Begin Source File

SOURCE=.\StResource.cpp
# End Source File
# Begin Source File

SOURCE=.\StringPosRange.cpp
# End Source File
# Begin Source File

SOURCE=.\StStruct.cpp
# End Source File
# Begin Source File

SOURCE=.\SysComment.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskConfiguration.cpp
# End Source File
# Begin Source File

SOURCE=.\VarDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\VarDeclSection.cpp
# End Source File
# Begin Source File

SOURCE=.\VarListElem.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BackParserDLL.h
# End Source File
# Begin Source File

SOURCE=.\CE_SRCOPS_IEC_IGNOREWRN.h
# End Source File
# Begin Source File

SOURCE=.\ClassViewDescrGenerator.h
# End Source File
# Begin Source File

SOURCE=.\Configuration.h
# End Source File
# Begin Source File

SOURCE=.\DllInterface.h
# End Source File
# Begin Source File

SOURCE=.\EmptyFile.h
# End Source File
# Begin Source File

SOURCE=.\Function.h
# End Source File
# Begin Source File

SOURCE=.\FunctionBlock.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\GlobVarConstList.h
# End Source File
# Begin Source File

SOURCE=.\IECSRCVisitor.h
# End Source File
# Begin Source File

SOURCE=.\ImportDirective.h
# End Source File
# Begin Source File

SOURCE=.\LexVal.h
# End Source File
# Begin Source File

SOURCE=.\ParserContext.h
# End Source File
# Begin Source File

SOURCE=.\POU.h
# End Source File
# Begin Source File

SOURCE=.\PouParser.h
# End Source File
# Begin Source File

SOURCE=.\Program.h
# End Source File
# Begin Source File

SOURCE=.\ProgramConfiguration.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StreamScan.h
# End Source File
# Begin Source File

SOURCE=.\StResource.h
# End Source File
# Begin Source File

SOURCE=.\StringPosRange.h
# End Source File
# Begin Source File

SOURCE=.\StStruct.h
# End Source File
# Begin Source File

SOURCE=.\SysComment.h
# End Source File
# Begin Source File

SOURCE=.\TaskConfiguration.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\traceif.h
# End Source File
# Begin Source File

SOURCE=.\VarDecl.h
# End Source File
# Begin Source File

SOURCE=.\VarDeclSection.h
# End Source File
# Begin Source File

SOURCE=.\VarListElem.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BackParserDLL.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\BackParserDLL.INF
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
