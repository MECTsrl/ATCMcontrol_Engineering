# Microsoft Developer Studio Project File - Name="GrEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=GrEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "greditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "greditor.mak" CFG="GrEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GrEditor - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "GrEditor - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/GREditor", DRNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "GrEditor - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f greditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "greditor.exe"
# PROP BASE Bsc_Name "greditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f greditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "greditor.exe"
# PROP Bsc_Name "greditor.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "GrEditor - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f greditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "greditor.exe"
# PROP BASE Bsc_Name "greditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f greditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "greditor.exe"
# PROP Bsc_Name "greditor.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=update minor version number
PostBuild_Cmds=BuildNr.exe  4cgr_version.h GREDITOR_VERSION minor	copyto4c.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "GrEditor - Win32 Release"
# Name "GrEditor - Win32 Debug"

!IF  "$(CFG)" == "GrEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "GrEditor - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\aboutdlg.rc
# End Source File
# Begin Source File

SOURCE=..\..\inc\CEMsg.IDL

!IF  "$(CFG)" == "GrEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "GrEditor - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Inc\cewatchbe.idl
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\inc\CSC_Online.idl
# End Source File
# Begin Source File

SOURCE=.\DbgEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\Elem.cpp
# End Source File
# Begin Source File

SOURCE=.\ElemEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\ElemFB.cpp
# End Source File
# Begin Source File

SOURCE=.\ElemLd.cpp
# End Source File
# Begin Source File

SOURCE=.\ElemPtrList.cpp
# End Source File
# Begin Source File

SOURCE=.\ElemSfc.cpp
# End Source File
# Begin Source File

SOURCE=.\GrEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\GrEditor.rc
# End Source File
# Begin Source File

SOURCE=.\GrEditorAction.cpp
# End Source File
# Begin Source File

SOURCE=.\GrEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\GrEditorIF.idl
# End Source File
# Begin Source File

SOURCE=.\GrEditorUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\GrEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GrPrintOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsActDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsClDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsCmtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsConDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsCtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsDivDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsFbDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsLabDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsStepDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsTranDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsVarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\Inc\key.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PosArray.cpp
# End Source File
# Begin Source File

SOURCE=.\PropEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PropLineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPRail.cpp
# End Source File
# Begin Source File

SOURCE=.\Router.cpp
# End Source File
# Begin Source File

SOURCE=..\Inc\SecEdit.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\STEditor.rc
# End Source File
# Begin Source File

SOURCE=.\STEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\4CGR_Version.h

!IF  "$(CFG)" == "GrEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "GrEditor - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BookMark.h
# End Source File
# Begin Source File

SOURCE=..\..\res\BuildNr.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DbgEditorView.h
# End Source File
# Begin Source File

SOURCE=..\Inc\editres.h
# End Source File
# Begin Source File

SOURCE=.\Elem.h
# End Source File
# Begin Source File

SOURCE=.\ElemEditorView.h
# End Source File
# Begin Source File

SOURCE=.\ElemFB.h
# End Source File
# Begin Source File

SOURCE=.\ElemLd.h
# End Source File
# Begin Source File

SOURCE=.\ElemPtrList.h
# End Source File
# Begin Source File

SOURCE=.\ElemSfc.h
# End Source File
# Begin Source File

SOURCE=..\inc\Flag.h
# End Source File
# Begin Source File

SOURCE=.\GrEditor.h
# End Source File
# Begin Source File

SOURCE=.\GrEditorAction.h
# End Source File
# Begin Source File

SOURCE=.\GrEditorBase.h
# End Source File
# Begin Source File

SOURCE=.\GrEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\GrEditorUtil.h
# End Source File
# Begin Source File

SOURCE=.\GrEditorView.h
# End Source File
# Begin Source File

SOURCE=.\GridDlg.h
# End Source File
# Begin Source File

SOURCE=.\GrPrintOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\GrResource.h
# End Source File
# Begin Source File

SOURCE=.\InsActDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsClDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsCmtDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsConDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsCtDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsDivDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsFbDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsLabDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsStepDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsTranDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsVarDlg.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=..\Inc\keyrc.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PosArray.h
# End Source File
# Begin Source File

SOURCE=..\inc\PrgEditorView.h
# End Source File
# Begin Source File

SOURCE=..\..\res\Product.h
# End Source File
# Begin Source File

SOURCE=.\PropEdit.h
# End Source File
# Begin Source File

SOURCE=.\PropLineDlg.h
# End Source File
# Begin Source File

SOURCE=.\PropPRail.h
# End Source File
# Begin Source File

SOURCE=.\Router.h
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEdit.h
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEditCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEditView.h
# End Source File
# Begin Source File

SOURCE=.\Set.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StatDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STEditorView.h
# End Source File
# Begin Source File

SOURCE=.\STResource.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=..\..\res\VersionNr.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\XML_4gr_names.h
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cecon01.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cecon02.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cecon03.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cecon10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cecon20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cecon30.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\celab01.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\celab02.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cevar01.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cevar02.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cevar03.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cevar04.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cevar05.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cevar06.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\delcol.cur
# End Source File
# Begin Source File

SOURCE=.\Res\delcol1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\delrow1.cur
# End Source File
# Begin Source File

SOURCE=.\res\gr_cl_pr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gr_ct_pr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gr_toolb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GrEditor.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\GrEditorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Res\iconcmt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iconcon.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconcon1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconcon2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconcon3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconcon4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iconcst.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconlab.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconlabr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iconret.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconretl.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconretr.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconvarf.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconvarr.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\iconvarw.bmp
# End Source File
# Begin Source File

SOURCE=..\res\idccurso.cur
# End Source File
# Begin Source File

SOURCE=.\Res\inscol.cur
# End Source File
# Begin Source File

SOURCE=.\Res\inscol1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\inscol2.cur
# End Source File
# Begin Source File

SOURCE=.\Res\insrow.cur
# End Source File
# Begin Source File

SOURCE=.\Res\insrow1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\pencur1.cur
# End Source File
# Begin Source File

SOURCE=..\res\seceditgutter.bmp
# End Source File
# Begin Source File

SOURCE=.\res\STEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=..\..\res\Version.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\res\CEBPManError.mc

!IF  "$(CFG)" == "GrEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "GrEditor - Win32 Debug"

# Begin Custom Build
InputPath=..\..\res\CEBPManError.mc
InputName=CEBPManError

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"CEBPMANMessagesDEU.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\res\CEGRError.mc

!IF  "$(CFG)" == "GrEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "GrEditor - Win32 Debug"

# Begin Custom Build
InputPath=..\..\res\CEGRError.mc
InputName=CEGRError

BuildCmds= \
	mc $(InputPath)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"CEGRMessagesDEU.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"CEGRMessagesENU.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\res\CESTError.mc
# End Source File
# Begin Source File

SOURCE=.\GrEditor.inf
# End Source File
# Begin Source File

SOURCE=.\GrEditorIF.tlb
# End Source File
# Begin Source File

SOURCE=..\..\lib\fc_todebug.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\fc_tools.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\fc_util.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\fc_baselib.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\XML_Wrapper.lib
# End Source File
# Begin Source File

SOURCE=..\Lib\StdEditor.lib
# End Source File
# Begin Source File

SOURCE=..\Lib\OE.lib
# End Source File
# Begin Source File

SOURCE=..\Lib\PrgEditor.lib
# End Source File
# Begin Source File

SOURCE=..\Lib\CESubscr.lib
# End Source File
# Begin Source File

SOURCE=..\..\EXT\HTMLHelp\htmlhelp.lib
# End Source File
# End Target
# End Project
