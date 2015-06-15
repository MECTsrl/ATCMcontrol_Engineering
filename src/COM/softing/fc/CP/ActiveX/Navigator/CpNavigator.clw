; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCpNavTab
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "cpnavigator.h"
LastPage=0
CDK=Y

ClassCount=5
Class1=CCpNavigatorCtrl
Class2=CCpNavigatorPropPage
Class3=CCpNavPageFrame
Class4=CCpNavTab
Class5=CCpNavTreeCtrl

ResourceCount=2
Resource1=IDD_PROPPAGE_CPNAVIGATOR
Resource2=IDD_DIALOG1

[CLS:CCpNavigatorCtrl]
Type=0
BaseClass=COleControl
HeaderFile=CpNavigatorCtl.h
ImplementationFile=CpNavigatorCtl.cpp
Filter=W
VirtualFilter=wWC
LastObject=CCpNavigatorCtrl

[CLS:CCpNavigatorPropPage]
Type=0
BaseClass=COlePropertyPage
HeaderFile=CpNavigatorPpg.h
ImplementationFile=CpNavigatorPpg.cpp

[CLS:CCpNavPageFrame]
Type=0
BaseClass=CWnd
HeaderFile=CpNavPageFrame.h
ImplementationFile=CpNavPageFrame.cpp

[CLS:CCpNavTab]
Type=0
BaseClass=CTabCtrl
HeaderFile=CpNavTab.h
ImplementationFile=CpNavTab.cpp
Filter=W
VirtualFilter=UWC
LastObject=CCpNavTab

[CLS:CCpNavTreeCtrl]
Type=0
BaseClass=CTreeCtrl
HeaderFile=CpNavTreeCtrl.h
ImplementationFile=CpNavTreeCtrl.cpp
Filter=W
VirtualFilter=GWC
LastObject=CCpNavTreeCtrl

[DLG:IDD_PROPPAGE_CPNAVIGATOR]
Type=1
Class=CCpNavigatorPropPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352

