# Microsoft Developer Studio Project File - Name="AddonBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=AddonBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AddonBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AddonBase.mak" CFG="AddonBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AddonBase - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "AddonBase - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/AddOnHandler/AddonBase", VCUFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "AddonBase - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f AddonBase.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AddonBase.exe"
# PROP BASE Bsc_Name "AddonBase.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /nologo /f AddonBase.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AddonBase.exe"
# PROP Bsc_Name "AddonBase.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "AddonBase - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f AddonBase.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AddonBase.exe"
# PROP BASE Bsc_Name "AddonBase.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /nologo /f AddonBase.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AddonBase.exe"
# PROP Bsc_Name "AddonBase.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "AddonBase - Win32 Release"
# Name "AddonBase - Win32 Debug"

!IF  "$(CFG)" == "AddonBase - Win32 Release"

!ELSEIF  "$(CFG)" == "AddonBase - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\inc\AddonBase.rc
# End Source File
# Begin Source File

SOURCE=.\AssignmentDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseAddonParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseCommPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseProjectData.cpp
# End Source File
# Begin Source File

SOURCE=.\BasePropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTargetData.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTargetObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProjectInfoUpload.cpp
# End Source File
# Begin Source File

SOURCE=.\EnterPasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FirmwareData.cpp
# End Source File
# Begin Source File

SOURCE=.\GHFile.cpp
# End Source File
# Begin Source File

SOURCE=.\PouProtectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\proppagecustomdownload.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\proppagenetbuicomm.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageSerialComm.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageSocketComm.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageTargetGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetSettingsSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Toolbox.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPageNames.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPageResource.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\AddonBaseCP.h
# End Source File
# Begin Source File

SOURCE=..\inc\AddonBaseResource.h
# End Source File
# Begin Source File

SOURCE=.\AssignmentDialog.h
# End Source File
# Begin Source File

SOURCE=..\inc\BaseAddonParameter.h
# End Source File
# Begin Source File

SOURCE=..\inc\BaseCommPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\basedefs.h
# End Source File
# Begin Source File

SOURCE=..\inc\BaseProjectData.h
# End Source File
# Begin Source File

SOURCE=..\inc\BasePropertyPage.h
# End Source File
# Begin Source File

SOURCE=..\inc\BaseTargetData.h
# End Source File
# Begin Source File

SOURCE=..\inc\BaseTargetObject.h
# End Source File
# Begin Source File

SOURCE=..\inc\ChangePasswordDlg.h
# End Source File
# Begin Source File

SOURCE=..\inc\CommDefinitions.h
# End Source File
# Begin Source File

SOURCE=..\inc\CommPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\DlgProjectInfoUpload.h
# End Source File
# Begin Source File

SOURCE=..\inc\EnterPasswordDlg.h
# End Source File
# Begin Source File

SOURCE=..\inc\FirmwareData.h
# End Source File
# Begin Source File

SOURCE=..\inc\GHFile.h
# End Source File
# Begin Source File

SOURCE=..\inc\PouProtectData.h
# End Source File
# Begin Source File

SOURCE=..\inc\PouProtectionDlg.h
# End Source File
# Begin Source File

SOURCE=..\inc\proppagecustomdownload.h
# End Source File
# Begin Source File

SOURCE=..\inc\PropPageInfo.h
# End Source File
# Begin Source File

SOURCE=..\inc\proppagenetbuicomm.h
# End Source File
# Begin Source File

SOURCE=..\inc\PropPageSerialComm.h
# End Source File
# Begin Source File

SOURCE=..\inc\PropPageSocketComm.h
# End Source File
# Begin Source File

SOURCE=..\inc\PropPageTargetGeneral.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\inc\TargetPropertySheet.h
# End Source File
# Begin Source File

SOURCE=..\inc\TargetSettingsSheet.h
# End Source File
# Begin Source File

SOURCE=..\inc\Toolbox.h
# End Source File
# Begin Source File

SOURCE=..\inc\WizardPageNames.h
# End Source File
# Begin Source File

SOURCE=..\inc\WizardPageResource.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AddonBase.inf
# End Source File
# Begin Source File

SOURCE=..\inc\clvwimag.bmp
# End Source File
# End Target
# End Project
