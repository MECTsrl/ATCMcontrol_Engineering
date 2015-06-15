# Microsoft Developer Studio Project File - Name="OPCEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OPCEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "opcEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "opcEngine.mak" CFG="OPCEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OPCEngine - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "OPCEngine - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CSC/opcEngine/engine", AOSFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "OPCEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f opcEngine.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "opcEngine.exe"
# PROP BASE Bsc_Name "opcEngine.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f opcEngine.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "opcEngine.exe"
# PROP Bsc_Name "opcEngine.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "OPCEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f opcEngine.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "opcEngine.exe"
# PROP BASE Bsc_Name "opcEngine.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f opcEngine.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "opcEngine.exe"
# PROP Bsc_Name "opcEngine.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "OPCEngine - Win32 Release"
# Name "OPCEngine - Win32 Debug"

!IF  "$(CFG)" == "OPCEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "OPCEngine - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\common\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=.\enum.cpp
# End Source File
# Begin Source File

SOURCE=.\extimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\GenCache.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GenError.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GenFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\GenGrp.cpp
# End Source File
# Begin Source File

SOURCE=.\GenItem.cpp
# End Source File
# Begin Source File

SOURCE=.\GenNS.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GenObj.cpp
# End Source File
# Begin Source File

SOURCE=.\GenReq.cpp
# End Source File
# Begin Source File

SOURCE=.\GenSrv.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GenTree.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCEngine.rc
# End Source File
# Begin Source File

SOURCE=..\opc\opcenum_clsid.c
# End Source File
# Begin Source File

SOURCE=.\OPCGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCObj.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCServer.cpp
# End Source File
# Begin Source File

SOURCE=..\common\OPCTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCTrans.cpp
# End Source File
# Begin Source File

SOURCE=..\common\softing\softingopctb.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\buildnr.h
# End Source File
# Begin Source File

SOURCE=..\common\dllmain.h
# End Source File
# Begin Source File

SOURCE=.\enum.h
# End Source File
# Begin Source File

SOURCE=..\include\GenCache.h
# End Source File
# Begin Source File

SOURCE=..\include\GenError.h
# End Source File
# Begin Source File

SOURCE=..\include\GenFunctions.h
# End Source File
# Begin Source File

SOURCE=..\include\GenGrp.h
# End Source File
# Begin Source File

SOURCE=..\include\GenItem.h
# End Source File
# Begin Source File

SOURCE=..\include\GenNS.h
# End Source File
# Begin Source File

SOURCE=..\include\GenObj.h
# End Source File
# Begin Source File

SOURCE=..\include\GenReq.h
# End Source File
# Begin Source File

SOURCE=..\include\GenSrv.h
# End Source File
# Begin Source File

SOURCE=..\include\GenTree.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=..\include\OPCAdviseSink.h
# End Source File
# Begin Source File

SOURCE=..\include\OPCEngine.h
# End Source File
# Begin Source File

SOURCE=.\OPCGroup.h
# End Source File
# Begin Source File

SOURCE=.\OPCObj.h
# End Source File
# Begin Source File

SOURCE=.\OPCServer.h
# End Source File
# Begin Source File

SOURCE=..\include\opcsession.h
# End Source File
# Begin Source File

SOURCE=..\include\OPCTrace.h
# End Source File
# Begin Source File

SOURCE=..\include\OPCTrans.h
# End Source File
# Begin Source File

SOURCE=..\include\softing\softingopctb.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE=..\opc\OPCCOMN.IDL
# End Source File
# Begin Source File

SOURCE=..\opc\OPCDA.IDL
# End Source File
# End Group
# Begin Source File

SOURCE=.\opcEngine.inf
# End Source File
# Begin Source File

SOURCE=.\opcerror.mc
# End Source File
# End Target
# End Project
