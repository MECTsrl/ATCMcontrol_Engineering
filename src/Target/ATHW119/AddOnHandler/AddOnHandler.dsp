# Microsoft Developer Studio Project File - Name="AddOnHandler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=AddOnHandler - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AddOnHandler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AddOnHandler.mak" CFG="AddOnHandler - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AddOnHandler - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "AddOnHandler - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/AOReleased/ATHW119_V2.20.00/AddOnHandler", OCSEAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "AddOnHandler - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f AddOnhandler.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AddOnhandler.exe"
# PROP BASE Bsc_Name "AddOnhandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f AddOnhandler.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AddOnhandler.exe"
# PROP Bsc_Name "AddOnhandler.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\AddOnhandler.exe
InputPath=.\AddOnhandler.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "AddOnHandler - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AddOnHandler___Win32_Release"
# PROP BASE Intermediate_Dir "AddOnHandler___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f AddOnhandler.mak /nologo"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AddOnhandler.exe"
# PROP BASE Bsc_Name "AddOnhandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AddOnHandler___Win32_Release"
# PROP Intermediate_Dir "AddOnHandler___Win32_Release"
# PROP Cmd_Line "NMAKE /f AddOnhandler.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AddOnhandler.exe"
# PROP Bsc_Name "AddOnhandler.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\AddOnHandler___Win32_Release
TargetPath=.\AddOnhandler.exe
InputPath=.\AddOnhandler.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "AddOnHandler - Win32 Debug"
# Name "AddOnHandler - Win32 Release"

!IF  "$(CFG)" == "AddOnHandler - Win32 Debug"

!ELSEIF  "$(CFG)" == "AddOnHandler - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AAddOnObject.cpp
# End Source File
# Begin Source File

SOURCE=.\AAddOnParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\AddOnHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\AddOnHandler.def
# End Source File
# Begin Source File

SOURCE=.\AddOnHandler.idl
# End Source File
# Begin Source File

SOURCE=.\AddOnHandler.rc
# End Source File
# Begin Source File

SOURCE=.\AProjectData.cpp
# End Source File
# Begin Source File

SOURCE=.\ATargetData.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AAddOnObject.h
# End Source File
# Begin Source File

SOURCE=.\AAddOnParameter.h
# End Source File
# Begin Source File

SOURCE=.\AProjectData.h
# End Source File
# Begin Source File

SOURCE=.\ATargetData.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardDialog.h
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
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AAddOnObject.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\AddOnHandler.inf
# End Source File
# End Target
# End Project
