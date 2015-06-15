# Microsoft Developer Studio Project File - Name="CEMAIN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEMAIN - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEMAIN.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEMAIN.mak" CFG="CEMAIN - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEMAIN - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CEMAIN - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEMAIN", MSMFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEMAIN - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f cemain.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cemain.exe"
# PROP BASE Bsc_Name "cemain.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f cemain.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cemain.exe"
# PROP Bsc_Name "cemain.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "CEMAIN - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f cemain.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cemain.exe"
# PROP BASE Bsc_Name "cemain.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f cemain.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cemain.exe"
# PROP Bsc_Name "cemain.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "CEMAIN - Win32 Release"
# Name "CEMAIN - Win32 Debug"

!IF  "$(CFG)" == "CEMAIN - Win32 Release"

!ELSEIF  "$(CFG)" == "CEMAIN - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BPViewWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\BPViewWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\CEAddonActionEventsComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\CEFRAME\CECompMan.cpp
# End Source File
# Begin Source File

SOURCE=.\CEDocuManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CEFileHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\CEMAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\CEMAIN.rc
# End Source File
# Begin Source File

SOURCE=.\CEMAINDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CEMAINView.cpp
# End Source File
# Begin Source File

SOURCE=.\CEFRAME\CEStateMan.cpp
# End Source File
# Begin Source File

SOURCE=..\inc\CESysIF_i.c
# End Source File
# Begin Source File

SOURCE=.\CEWindowPlacement.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeCaptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\connectdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\CoverPageDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CPPFileCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRLEXT.CPP
# End Source File
# Begin Source File

SOURCE=.\download.cpp
# End Source File
# Begin Source File

SOURCE=.\Emboss.cpp
# End Source File
# Begin Source File

SOURCE=.\FileStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\findfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HWTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ImplSessNot.cpp
# End Source File
# Begin Source File

SOURCE=.\InstTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\maindroptarget.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiSelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Navigator.cpp
# End Source File
# Begin Source File

SOURCE=.\NavTreeControl.cpp
# End Source File
# Begin Source File

SOURCE=.\NavTreeDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\pouvarselctrlwrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\PouVarSelWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintFooterDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintHeaderDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Printing.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjManEventsImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjManListEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjSaveAsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\SubscrElement.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetOnl.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetOnlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetSet.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XRefWindow.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BPViewWindow.h
# End Source File
# Begin Source File

SOURCE=.\BPViewWrapper.h
# End Source File
# Begin Source File

SOURCE=.\CEAddOnActionEventsComObj.h
# End Source File
# Begin Source File

SOURCE=.\inc\CECompMan.h
# End Source File
# Begin Source File

SOURCE=.\CEDocuManager.h
# End Source File
# Begin Source File

SOURCE=.\CEFileHandler.h
# End Source File
# Begin Source File

SOURCE=.\CEMAIN.h
# End Source File
# Begin Source File

SOURCE=.\CEMAINDoc.h
# End Source File
# Begin Source File

SOURCE=.\inc\CEMAINError.h
# End Source File
# Begin Source File

SOURCE=.\CEMAINView.h
# End Source File
# Begin Source File

SOURCE=.\inc\CEStateMan.h
# End Source File
# Begin Source File

SOURCE=..\inc\CESysDef.h
# End Source File
# Begin Source File

SOURCE=.\CEWindowPlacement.h
# End Source File
# Begin Source File

SOURCE=.\ChangeCaptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClassTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\inc\connectdlg.h
# End Source File
# Begin Source File

SOURCE=.\inc\CoolMenu.h
# End Source File
# Begin Source File

SOURCE=.\CoverPageDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\CPPFileCollection.h
# End Source File
# Begin Source File

SOURCE=.\inc\CTRLEXT.H
# End Source File
# Begin Source File

SOURCE=.\inc\download.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\fccallback.h
# End Source File
# Begin Source File

SOURCE=..\inc\fcpump.h
# End Source File
# Begin Source File

SOURCE=.\FileStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\findfiledlg.h
# End Source File
# Begin Source File

SOURCE=.\HWTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\inc\ImplSessNot.h
# End Source File
# Begin Source File

SOURCE=.\InstTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\maindroptarget.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\mfcmap.h
# End Source File
# Begin Source File

SOURCE=.\MultiSelDlg.h
# End Source File
# Begin Source File

SOURCE=.\NavDef.h
# End Source File
# Begin Source File

SOURCE=.\Navigator.h
# End Source File
# Begin Source File

SOURCE=.\NavTreeControl.h
# End Source File
# Begin Source File

SOURCE=.\NavTreeDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\pouvarselctrlwrapper.h
# End Source File
# Begin Source File

SOURCE=.\PouVarSelWindow.h
# End Source File
# Begin Source File

SOURCE=.\PrintFooterDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\PrintHeaderDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\Printing.h
# End Source File
# Begin Source File

SOURCE=.\PrintLayout.h
# End Source File
# Begin Source File

SOURCE=.\PrintOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\PrintSettings.h
# End Source File
# Begin Source File

SOURCE=.\PrintSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProjManEventsImpl.h
# End Source File
# Begin Source File

SOURCE=.\ProjManListEvents.h
# End Source File
# Begin Source File

SOURCE=.\ProjSaveAsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "CEMAIN - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=cemain
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEMAIN - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\SettingsDialogs.h
# End Source File
# Begin Source File

SOURCE=.\inc\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\inc\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\SubscrElement.h
# End Source File
# Begin Source File

SOURCE=..\inc\TargetOnl.h
# End Source File
# Begin Source File

SOURCE=..\inc\TargetOnlBase.h
# End Source File
# Begin Source File

SOURCE=.\inc\TargetSet.h
# End Source File
# Begin Source File

SOURCE=.\inc\ToolBox.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\XRefWindow.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\4Control3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addvar.ico
# End Source File
# Begin Source File

SOURCE=.\res\CEMAIN.ico
# End Source File
# Begin Source File

SOURCE=.\res\CEMAIN.rc2
# End Source File
# Begin Source File

SOURCE=.\res\CEMAINDoc.ico
# End Source File
# Begin Source File

SOURCE=..\..\res\CEMAINError.mc
# End Source File
# Begin Source File

SOURCE=.\res\clvwimag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\continue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\debugtoo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\helptool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_file.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_info.ico
# End Source File
# Begin Source File

SOURCE=.\res\ietoolba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\log4ctr1small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\log4ctrlprint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\msgtoolb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\removeal.ico
# End Source File
# Begin Source File

SOURCE=.\res\removeva.ico
# End Source File
# Begin Source File

SOURCE=.\res\stateimagelist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabimage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewtool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wizimage.bmp
# End Source File
# End Group
# Begin Group "GenEdCont"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GenEdDoc\gened.idl
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEd.rc
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEdCntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdCntrItem.h
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEdCont.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdCont.h
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdDef.h
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEdDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdDoc.h
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEdDocTempl.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdDocTempl.h
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEdFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdFrame.h
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdRC.h
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEdTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdTabWnd.h
# End Source File
# Begin Source File

SOURCE=.\GenEdDoc\GenEdView.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\GenEdView.h
# End Source File
# End Group
# Begin Group "Idl Files"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE=..\inc\CEAddOnActionEvents.idl
# End Source File
# Begin Source File

SOURCE=..\INC\cecompmanev.idl
# End Source File
# Begin Source File

SOURCE=.\CEMAINInterface.idl
# End Source File
# End Group
# Begin Group "Panel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PanelDoc\panel.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\panel.h
# End Source File
# Begin Source File

SOURCE=.\PanelDoc\paneldoc.rc
# End Source File
# Begin Source File

SOURCE=.\inc\panelrc.h
# End Source File
# Begin Source File

SOURCE=.\PanelDoc\panelvw.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\panelvw.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CEMAIN.INF
# End Source File
# Begin Source File

SOURCE=..\..\res\csc_onlineerr.mc
# End Source File
# End Target
# End Project
