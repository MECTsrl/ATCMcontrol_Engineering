# Microsoft Developer Studio Project File - Name="FcHtmlEd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=FcHtmlEd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "htmleditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "htmleditor.mak" CFG="FcHtmlEd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FcHtmlEd - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "FcHtmlEd - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/HtmlEditor", WWNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "FcHtmlEd - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f htmleditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "htmleditor.exe"
# PROP BASE Bsc_Name "htmleditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f htmleditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "htmleditor.exe"
# PROP Bsc_Name "htmleditor.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "FcHtmlEd - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f htmleditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "htmleditor.exe"
# PROP BASE Bsc_Name "htmleditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f htmleditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "htmleditor.exe"
# PROP Bsc_Name "htmleditor.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "FcHtmlEd - Win32 Release"
# Name "FcHtmlEd - Win32 Debug"

!IF  "$(CFG)" == "FcHtmlEd - Win32 Release"

!ELSEIF  "$(CFG)" == "FcHtmlEd - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\FcColorComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FcDlgInsertHtml.cpp
# End Source File
# Begin Source File

SOURCE=.\FcDlgPageProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\FcFontBar.cpp
# End Source File
# Begin Source File

SOURCE=.\FcHtmlColor.cpp
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEd.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\FcHtmlEd.hpj

!IF  "$(CFG)" == "FcHtmlEd - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__FCHTM="hlp\AfxCore.rtf"	"hlp\AfxPrint.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Release
TargetName=htmleditor
InputPath=.\hlp\FcHtmlEd.hpj
InputName=FcHtmlEd

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FcHtmlEd - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__FCHTM="hlp\AfxCore.rtf"	"hlp\AfxPrint.rtf"	"hlp\$(TargetName).hm"	
# Begin Custom Build - Making help file...
OutDir=.\Debug
TargetName=htmleditor
InputPath=.\hlp\FcHtmlEd.hpj
InputName=FcHtmlEd

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FcHtmlEd.rc
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdIF.idl
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdView.cpp
# End Source File
# Begin Source File

SOURCE=.\FcIDispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\FcIDocHostShowUI.cpp
# End Source File
# Begin Source File

SOURCE=.\FcIDocHostUIHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\FcInsertTableDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FcIOleClientSite.cpp
# End Source File
# Begin Source File

SOURCE=.\FcIOleDocumentSite.cpp
# End Source File
# Begin Source File

SOURCE=.\FcIOleInPlaceSite.cpp
# End Source File
# Begin Source File

SOURCE=.\FcPropertyPageGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\FcStyleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SITE.CPP
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\FcColorComboBox.h
# End Source File
# Begin Source File

SOURCE=.\FcDlgInsertHtml.h
# End Source File
# Begin Source File

SOURCE=.\FcDlgPageProperties.h
# End Source File
# Begin Source File

SOURCE=.\FcFontBar.h
# End Source File
# Begin Source File

SOURCE=.\FcHtmlColor.h
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEd.h
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdDoc.h
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdOptions.h
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdView.h
# End Source File
# Begin Source File

SOURCE=.\FcIDispatch.h
# End Source File
# Begin Source File

SOURCE=.\FcIDocHostShowUI.h
# End Source File
# Begin Source File

SOURCE=.\FcIDocHostUIHandler.h
# End Source File
# Begin Source File

SOURCE=.\FcInsertTableDlg.h
# End Source File
# Begin Source File

SOURCE=.\FcIOleClientSite.h
# End Source File
# Begin Source File

SOURCE=.\FcIOleDocumentSite.h
# End Source File
# Begin Source File

SOURCE=.\FcIOleInPlaceSite.h
# End Source File
# Begin Source File

SOURCE=.\FcPropertyPageGrid.h
# End Source File
# Begin Source File

SOURCE=.\FcStyleDlg.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "FcHtmlEd - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=htmleditor
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

!ELSEIF  "$(CFG)" == "FcHtmlEd - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=htmleditor
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

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SITE.H
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Inc\StdEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\VERSION.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FcHtmlEd.ico
# End Source File
# Begin Source File

SOURCE=.\res\FcHtmlEd.rc2
# End Source File
# Begin Source File

SOURCE=.\res\FcHtmlEdDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\FcHtmlEd.reg
# End Source File
# Begin Source File

SOURCE=.\FcHtmlEdIF.tlb
# End Source File
# Begin Source File

SOURCE=.\HtmlEditor.inf
# End Source File
# End Target
# End Project
# Section FcHtmlEd : {3050F32D-98B5-11CF-BB82-00AA00BDCE0B}
# 	2:5:Class:CHTMLEventObj
# 	2:10:HeaderFile:htmleventobj.h
# 	2:8:ImplFile:htmleventobj.cpp
# End Section
# Section FcHtmlEd : {2D360200-FFF5-11D1-8D03-00A0C959BC0A}
# 	2:21:DefaultSinkHeaderFile:dhtmledit.h
# 	2:16:DefaultSinkClass:CDHTMLEdit
# End Section
# Section FcHtmlEd : {72ADFD6C-2C39-11D0-9903-00A0C91BC942}
# 	1:26:CG_IDR_POPUP_FC_FORMAT_BAR:102
# End Section
# Section FcHtmlEd : {CE04B591-2B1F-11D2-8D1E-00A0C959BC0A}
# 	2:5:Class:CDHTMLEdit
# 	2:10:HeaderFile:dhtmledit.h
# 	2:8:ImplFile:dhtmledit.cpp
# End Section
