# Microsoft Developer Studio Project File - Name="Dialogs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Dialogs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Dialogs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dialogs.mak" CFG="Dialogs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dialogs - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "Dialogs - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEAddOnOps/Dialogs/Dialogs", XCMFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Dialogs - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f dialogs.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "dialogs.exe"
# PROP BASE Bsc_Name "dialogs.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f dialogs.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "dialogs.exe"
# PROP Bsc_Name "dialogs.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "Dialogs - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f dialogs.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "dialogs.exe"
# PROP BASE Bsc_Name "dialogs.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f dialogs.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "dialogs.exe"
# PROP Bsc_Name "dialogs.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "Dialogs - Win32 Release"
# Name "Dialogs - Win32 Debug"

!IF  "$(CFG)" == "Dialogs - Win32 Release"

!ELSEIF  "$(CFG)" == "Dialogs - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DialogConst.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogConstantFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogDataType.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogDeclBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogDutDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogFbDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogFunctDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogFunctionBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGen.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenConst.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenGlobVar.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenPB.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenPc.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenProgInst.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenProj.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenResource.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenTask.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGenVarBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGlobVar.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogGlobVarFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogOpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogPB.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogPc.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogProgInst.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogProj.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogResource.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs.def
# End Source File
# Begin Source File

SOURCE=.\DialogTask.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogWiz.cpp
# End Source File
# Begin Source File

SOURCE=.\EventSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\extern.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobConst.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpString.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyDutMember.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyFbMember.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjProp.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropDeclBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropDutDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropFbDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropFbmDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropFunctDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGen.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenConst.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenGlobVar.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenPB.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenPc.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenProgInst.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenProj.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenResource.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenTask.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenVarBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjPropWiz.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertySheetBase.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxCheck.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DialogConst.h
# End Source File
# Begin Source File

SOURCE=.\DialogConstantFile.h
# End Source File
# Begin Source File

SOURCE=.\DialogDataType.h
# End Source File
# Begin Source File

SOURCE=.\DialogDeclBase.h
# End Source File
# Begin Source File

SOURCE=.\DialogDutDecl.h
# End Source File
# Begin Source File

SOURCE=.\DialogFbDecl.h
# End Source File
# Begin Source File

SOURCE=.\DialogFunctDecl.h
# End Source File
# Begin Source File

SOURCE=.\DialogFunction.h
# End Source File
# Begin Source File

SOURCE=.\DialogFunctionBlock.h
# End Source File
# Begin Source File

SOURCE=.\DialogGen.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenBase.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenConst.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenGlobVar.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenPB.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenPc.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenProgInst.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenProj.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenResource.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenTask.h
# End Source File
# Begin Source File

SOURCE=.\DialogGenVarBase.h
# End Source File
# Begin Source File

SOURCE=.\DialogGlobVar.h
# End Source File
# Begin Source File

SOURCE=.\DialogGlobVarFile.h
# End Source File
# Begin Source File

SOURCE=.\DialogHelp.h
# End Source File
# Begin Source File

SOURCE=.\DialogInfo.h
# End Source File
# Begin Source File

SOURCE=.\DialogOpc.h
# End Source File
# Begin Source File

SOURCE=.\DialogPB.h
# End Source File
# Begin Source File

SOURCE=.\DialogPc.h
# End Source File
# Begin Source File

SOURCE=.\DialogProgInst.h
# End Source File
# Begin Source File

SOURCE=.\DialogProgram.h
# End Source File
# Begin Source File

SOURCE=.\DialogProj.h
# End Source File
# Begin Source File

SOURCE=.\DialogResource.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs.h
# End Source File
# Begin Source File

SOURCE=.\DialogTask.h
# End Source File
# Begin Source File

SOURCE=.\DialogType.h
# End Source File
# Begin Source File

SOURCE=.\DialogWiz.h
# End Source File
# Begin Source File

SOURCE=.\EventSelection.h
# End Source File
# Begin Source File

SOURCE=.\extern.h
# End Source File
# Begin Source File

SOURCE=.\GlobConst.h
# End Source File
# Begin Source File

SOURCE=.\HelpString.h
# End Source File
# Begin Source File

SOURCE=.\ModifyDutMember.h
# End Source File
# Begin Source File

SOURCE=.\ModifyFbMember.h
# End Source File
# Begin Source File

SOURCE=.\ObjProp.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropDeclBase.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropDutDecl.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropFbDecl.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropFbmDecl.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropFunctDecl.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGen.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenBase.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenConst.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenGlobVar.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenPB.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenPc.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenProgInst.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenProj.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenResource.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenTask.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropGenVarBase.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropHelp.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropInfo.h
# End Source File
# Begin Source File

SOURCE=.\ObjPropWiz.h
# End Source File
# Begin Source File

SOURCE=.\PropertySheetBase.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxCheck.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;rc"
# Begin Source File

SOURCE=.\Dialogs.INF
# End Source File
# Begin Source File

SOURCE=.\Dialogs.rc
# End Source File
# Begin Source File

SOURCE=.\res\Dialogs.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\selectst.bmp
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\res\wizimage.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
