# Microsoft Developer Studio Project File - Name="OnlTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OnlTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OnlTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OnlTest.mak" CFG="OnlTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OnlTest - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "OnlTest - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/Online/OnlTest", RBXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "OnlTest - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f OnlTest.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlTest.exe"
# PROP BASE Bsc_Name "OnlTest.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f OnlTest.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "OnlTest.exe"
# PROP Bsc_Name "OnlTest.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "OnlTest - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f OnlTest.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlTest.exe"
# PROP BASE Bsc_Name "OnlTest.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f OnlTest.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "OnlTest.exe"
# PROP Bsc_Name "OnlTest.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "OnlTest - Win32 Release"
# Name "OnlTest - Win32 Debug"

!IF  "$(CFG)" == "OnlTest - Win32 Release"

!ELSEIF  "$(CFG)" == "OnlTest - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CommBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CommDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\CommVisu.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlTest.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlTest.rc
# End Source File
# Begin Source File

SOURCE=.\OnlTestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\4CLayer.h
# End Source File
# Begin Source File

SOURCE=.\CommBase.h
# End Source File
# Begin Source File

SOURCE=.\CommDLL.h
# End Source File
# Begin Source File

SOURCE=.\CommVisu.h
# End Source File
# Begin Source File

SOURCE=.\OnlTest.h
# End Source File
# Begin Source File

SOURCE=.\OnlTestDlg.h
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

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\OnlTest.ico
# End Source File
# Begin Source File

SOURCE=.\res\OnlTest.rc2
# End Source File
# End Group
# Begin Group "RTS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Control\inc\intDef.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\intOpcds.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\libDef.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\osAlign.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\osDef.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\osFile.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\osFirst.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\osLast.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\osSocket.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\osTarget.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\stdInc.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc.vis\visDef.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc.vis\visMain.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\visShared.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\vmmDef.h
# End Source File
# Begin Source File

SOURCE=..\..\Control\inc\vmmMain.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmShared.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmSharedDef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\OnlTest.inf
# End Source File
# End Target
# End Project
