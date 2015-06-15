// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#include "DialogFunctionBlock.h"
#include "DialogFunction.h"
#include "DialogProgram.h"
#include "DialogDataType.h"
#include "DialogConst.h"
#include "DialogGlobVar.h"
#include "DialogPB.h"
#include "DialogOpc.h"
#include "DialogTask.h"
#include "DialogPc.h"
#include "DialogResource.h"
#include "DialogConstantFile.h"
#include "DialogGlobVarFile.h"
#include "DialogType.h"
#include "DialogProgInst.h"
#include "DialogProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_iDispId = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_CBIndex(pDX, IDC_DISPID, m_iDispId);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    m_iDispId = 0;
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnTest() 
{
    UpdateData(TRUE);

    test(m_iDispId+1);

//	CDialog kiki("kiki");
//	kiki.DoModal();
	
}

void test(int iDispId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    if(iDispId == 1)
    {
	    CDialogFunctionBlock *DFB1;
        DFB1 = CDialogFunctionBlock::CreateNewObject(CVIEWINSERT);
        DFB1->DoModal();
        SAFEARRAY * pMembers;
        pMembers= DFB1->GetMembers();
        int index = DFB1->GetIndexOfSelectedWizardName();

	    CDialogFunctionBlock *DFB2;
        DFB2 = CDialogFunctionBlock::CreateNewObject(CVIEWPROP);
        DFB2->SetMembers(pMembers);
	    DFB2->SetName(DFB1->GetName());
	    DFB2->SetAlias(DFB1->GetAlias());
	    DFB2->SetText1(DFB1->GetText1());
	    DFB2->SetText2(DFB1->GetText2());
	    DFB2->SetText3(DFB1->GetText3());
	    DFB2->SetUrl(DFB1->GetUrl());
	    DFB2->SetHelpUrl(DFB1->GetHelpUrl());
	    DFB2->SetLastModDate("123");
	    DFB2->SetLastModUser("abc");
	    DFB2->SetCreateDate("456");
	    DFB2->SetCreateUser("def");
	    DFB2->DoModal();

	    CDialogFunctionBlock *DFB3;
        DFB3 = CDialogFunctionBlock::CreateNewObject(IVIEWPROP);
        pMembers= DFB2->GetMembers();
	    DFB3->SetName(DFB1->GetName());
	    DFB3->SetAlias(DFB1->GetAlias());
	    DFB3->SetText1(DFB1->GetText1());
	    DFB3->SetText2(DFB1->GetText2());
	    DFB3->SetText3(DFB1->GetText3());
	    DFB3->SetUrl(DFB1->GetUrl());
	    DFB3->SetHelpUrl(DFB1->GetHelpUrl());
	    DFB3->SetLastModDate("123");
	    DFB3->SetLastModUser("abc");
	    DFB3->SetCreateDate("456");
	    DFB3->SetCreateUser("def");
        DFB3->SetMembers(pMembers);
        DFB3->DoModal();

        CDialogFunctionBlock::DeleteObject(DFB1);
        CDialogFunctionBlock::DeleteObject(DFB2);
        CDialogFunctionBlock::DeleteObject(DFB3);
    }
    else if(iDispId == 2)
    {
	    CDialogFunction *DF1;
        DF1 = CDialogFunction::CreateNewObject(CVIEWINSERT);
        DF1->DoModal();
        SAFEARRAY * pMembers;
        pMembers= DF1->GetMembers();
        CString strRetType = _T("");
        strRetType = DF1->GetReturnType();
        int index = DF1->GetIndexOfSelectedWizardName();

	    CDialogFunction *DF2;
        DF2 = CDialogFunction::CreateNewObject(CVIEWPROP);
        DF2->SetMembers(pMembers);
        DF2->SetReturnType(strRetType);
	    DF2->SetName(DF1->GetName());
	    DF2->SetAlias(DF1->GetAlias());
	    DF2->SetText1(DF1->GetText1());
	    DF2->SetText2(DF1->GetText2());
	    DF2->SetText3(DF1->GetText3());
	    DF2->SetUrl(DF1->GetUrl());
	    DF2->SetHelpUrl(DF1->GetHelpUrl());

	    DF2->SetLastModDate("123");
	    DF2->SetLastModUser("abc");
	    DF2->SetCreateDate("456");
	    DF2->SetCreateUser("def");
	    DF2->DoModal();
        strRetType = DF2->GetReturnType();

	    CDialogFunction *DF3;
        DF3 = CDialogFunction::CreateNewObject(IVIEWPROP);
        pMembers= DF2->GetMembers();
        DF3->SetMembers(pMembers);
        DF3->SetReturnType(strRetType);
        DF3->DoModal();

        CDialogFunction::DeleteObject(DF1);
        CDialogFunction::DeleteObject(DF2);
        CDialogFunction::DeleteObject(DF3);
    }
    else if(iDispId == 3)
    {
	    CDialogProgram *DP1;
        DP1 = CDialogProgram::CreateNewObject(CVIEWINSERT);
        DP1->DoModal();
        SAFEARRAY * pMembers;
        pMembers= DP1->GetMembers();
        int index = DP1->GetIndexOfSelectedWizardName();

	    CDialogProgram *DP2;
        DP2 = CDialogProgram::CreateNewObject(CVIEWPROP);
        DP2->SetMembers(pMembers);
	    DP2->SetName(DP1->GetName());
	    DP2->SetAlias(DP1->GetAlias());
	    DP2->SetText1(DP1->GetText1());
	    DP2->SetText2(DP1->GetText2());
	    DP2->SetText3(DP1->GetText3());
	    DP2->SetUrl(DP1->GetUrl());
	    DP2->SetHelpUrl(DP1->GetHelpUrl());
	    DP2->SetLastModDate("123");
	    DP2->SetLastModUser("abc");
	    DP2->SetCreateDate("456");
	    DP2->SetCreateUser("def");
	    DP2->DoModal();

	    CDialogProgram *DP3;
        DP3 = CDialogProgram::CreateNewObject(IVIEWPROP);
        pMembers= DP2->GetMembers();
        DP3->SetMembers(pMembers);
        DP3->DoModal();

        CDialogProgram::DeleteObject(DP1);
        CDialogProgram::DeleteObject(DP2);
        CDialogProgram::DeleteObject(DP3);
    }
    else if(iDispId == 4)
    {
	    CDialogDataType *DDT1;
        DDT1 = CDialogDataType::CreateNewObject(CVIEWINSERT);
        DDT1->DoModal();
        SAFEARRAY * pMembers;
        pMembers= DDT1->GetMembers();
        int index = DDT1->GetIndexOfSelectedWizardName();

	    CDialogDataType *DDT2;
        DDT2 = CDialogDataType::CreateNewObject(CVIEWPROP);
        DDT2->SetMembers(pMembers);
	    DDT2->SetName(DDT1->GetName());
	    DDT2->SetAlias(DDT1->GetAlias());
	    DDT2->SetText1(DDT1->GetText1());
	    DDT2->SetText2(DDT1->GetText2());
	    DDT2->SetText3(DDT1->GetText3());
	    DDT2->SetUrl(DDT1->GetUrl());
	    DDT2->SetHelpUrl(DDT1->GetHelpUrl());
	    DDT2->SetLastModDate("123");
	    DDT2->SetLastModUser("abc");
	    DDT2->SetCreateDate("456");
	    DDT2->SetCreateUser("def");
	    DDT2->DoModal();

	    CDialogDataType *DDT3;
        DDT3 = CDialogDataType::CreateNewObject(IVIEWPROP);
        pMembers= DDT2->GetMembers();
        DDT3->SetMembers(pMembers);
        DDT3->DoModal();

        CDialogDataType::DeleteObject(DDT1);
        CDialogDataType::DeleteObject(DDT2);
        CDialogDataType::DeleteObject(DDT3);
    }
    else if(iDispId == 5)
    {
        CStringArray Types;
        GetTypes(Types);

        CDialogConst *DC1;
        DC1 = CDialogConst::CreateNewObject(CVIEWINSERT);
//        DC1->SetTypes(Types);
        DC1->DoModal();

        CDialogConst *DC2;
        DC2 = CDialogConst::CreateNewObject(CVIEWPROP);
//        DC2->SetTypes(Types);
        DC2->SetName(DC1->GetName());
        DC2->SetAlias(DC1->GetAlias());
        DC2->SetType(DC1->GetType());
        DC2->SetValue(DC1->GetValue());
	    DC2->SetLastModDate("123");
	    DC2->SetLastModUser("abc");
	    DC2->SetCreateDate("456");
	    DC2->SetCreateUser("def");
        DC2->DoModal();
    
        CDialogConst *DC3;
        DC3 = CDialogConst::CreateNewObject(IVIEWPROP);
//        DC3->SetTypes(Types);        
        DC3->SetName(DC2->GetName());
        DC3->SetAlias(DC2->GetAlias());
        DC3->SetType(DC2->GetType());
        DC3->SetValue(DC2->GetValue());
	    DC3->SetLastModDate("123");
	    DC3->SetLastModUser("abc");
	    DC3->SetCreateDate("456");
	    DC3->SetCreateUser("def");
        DC3->DoModal();

        CDialogConst::DeleteObject(DC1);
        CDialogConst::DeleteObject(DC2);
        CDialogConst::DeleteObject(DC3);
    }
    else if(iDispId == 6)
    {
        CStringArray Types;
        GetTypes(Types);

        CDialogGlobVar *GV1;
        GV1 = CDialogGlobVar::CreateNewObject(CVIEWINSERT);
//        GV1->SetTypes(Types);        
        GV1->DoModal();

        CDialogGlobVar *GV2;
        GV2 = CDialogGlobVar::CreateNewObject(CVIEWPROP);
//        GV2->SetTypes(Types);        
        GV2->SetName(GV1->GetName());
        GV2->SetAlias(GV1->GetAlias());
        GV2->SetType(GV1->GetType());
        GV2->SetValue(GV1->GetValue());
        GV2->SetModifier(GV1->GetModifier());
	    GV2->SetLastModDate("123");
	    GV2->SetLastModUser("abc");
	    GV2->SetCreateDate("456");
	    GV2->SetCreateUser("def");
        GV2->DoModal();
    
        CDialogGlobVar *GV3;
        GV3 = CDialogGlobVar::CreateNewObject(IVIEWPROP);
//        GV2->SetTypes(Types);        
        GV3->SetName(GV2->GetName());
        GV3->SetAlias(GV2->GetAlias());
        GV3->SetType(GV2->GetType());
        GV3->SetValue(GV2->GetValue());
        GV3->SetModifier(GV2->GetModifier());
	    GV3->SetLastModDate("123");
	    GV3->SetLastModUser("abc");
	    GV3->SetCreateDate("456");
	    GV3->SetCreateUser("def");
        GV3->DoModal();

        CDialogGlobVar::DeleteObject(GV1);
        CDialogGlobVar::DeleteObject(GV2);
        CDialogGlobVar::DeleteObject(GV3);
    }
    else if(iDispId == 7)
    {
        CDialogPB *PB1;
        PB1 = CDialogPB::CreateNewObject(CVIEWINSERT);
        PB1->DoModal();

        CDialogPB *PB2;
        PB2 = CDialogPB::CreateNewObject(CVIEWPROP);
        PB2->SetName(PB1->GetName());
        PB2->SetAlias(PB1->GetAlias());
        PB2->SetChannelNumber(PB1->GetChannelNumber());
	    PB2->SetLastModDate("123");
	    PB2->SetLastModUser("abc");
	    PB2->SetCreateDate("456");
	    PB2->SetCreateUser("def");
 	    PB2->SetText1(PB1->GetText1());
	    PB2->SetText2(PB1->GetText2());
	    PB2->SetText3(PB1->GetText3());
	    PB2->SetUrl(PB1->GetUrl());
	    PB2->SetHelpUrl(PB1->GetHelpUrl());
        PB2->DoModal();
    
        CDialogPB *PB3;
        PB3 = CDialogPB::CreateNewObject(IVIEWPROP);
        PB3->SetName(PB2->GetName());
        PB3->SetAlias(PB2->GetAlias());
        PB3->SetChannelNumber(PB2->GetChannelNumber());
	    PB3->SetLastModDate("123");
	    PB3->SetLastModUser("abc");
	    PB3->SetCreateDate("456");
	    PB3->SetCreateUser("def");
 	    PB3->SetText1(PB2->GetText1());
	    PB3->SetText2(PB2->GetText2());
	    PB3->SetText3(PB2->GetText3());
	    PB3->SetUrl(PB2->GetUrl());
	    PB3->SetHelpUrl(PB2->GetHelpUrl());
        PB3->DoModal();

        CDialogPB::DeleteObject(PB1);
        CDialogPB::DeleteObject(PB2);
        CDialogPB::DeleteObject(PB3);
    }
    else if(iDispId == 8)
    {
        CDialogOpc *Opc1;
        Opc1 = CDialogOpc::CreateNewObject(CVIEWINSERT);
        Opc1->DoModal();

        CDialogOpc *Opc2;
        Opc2 = CDialogOpc::CreateNewObject(CVIEWPROP);
        Opc2->SetName(Opc1->GetName());
        Opc2->SetAlias(Opc1->GetAlias());
	    Opc2->SetLastModDate("123");
	    Opc2->SetLastModUser("abc");
	    Opc2->SetCreateDate("456");
	    Opc2->SetCreateUser("def");
 	    Opc2->SetText1(Opc1->GetText1());
	    Opc2->SetText2(Opc1->GetText2());
	    Opc2->SetText3(Opc1->GetText3());
	    Opc2->SetUrl(Opc1->GetUrl());
	    Opc2->SetHelpUrl(Opc1->GetHelpUrl());
        Opc2->DoModal();
    
        CDialogOpc *Opc3;
        Opc3 = CDialogOpc::CreateNewObject(IVIEWPROP);
        Opc3->SetName(Opc2->GetName());
        Opc3->SetAlias(Opc2->GetAlias());
	    Opc3->SetLastModDate("123");
	    Opc3->SetLastModUser("abc");
	    Opc3->SetCreateDate("456");
	    Opc3->SetCreateUser("def");
 	    Opc3->SetText1(Opc2->GetText1());
	    Opc3->SetText2(Opc2->GetText2());
	    Opc3->SetText3(Opc2->GetText3());
	    Opc3->SetUrl(Opc2->GetUrl());
	    Opc3->SetHelpUrl(Opc2->GetHelpUrl());
        Opc3->DoModal();

        CDialogOpc::DeleteObject(Opc1);
        CDialogOpc::DeleteObject(Opc2);
        CDialogOpc::DeleteObject(Opc3);
    }
    else if(iDispId == 9)
    {
        CDialogTask *Task1;
        Task1 = CDialogTask::CreateNewObject(CVIEWINSERT);
        Task1->DoModal();

        CDialogTask *Task2;
        Task2 = CDialogTask::CreateNewObject(CVIEWPROP);
        Task2->SetName(Task1->GetName());
        Task2->SetAlias(Task1->GetAlias());
	    Task2->SetLastModDate("123");
	    Task2->SetLastModUser("abc");
	    Task2->SetCreateDate("456");
	    Task2->SetCreateUser("def");
 	    Task2->SetText1(Task1->GetText1());
	    Task2->SetText2(Task1->GetText2());
	    Task2->SetText3(Task1->GetText3());
	    Task2->SetUrl(Task1->GetUrl());
	    Task2->SetHelpUrl(Task1->GetHelpUrl());
 	    Task2->SetAutoStart(Task1->GetAutoStart());
 	    Task2->SetInterval(Task1->GetInterval());
 	    Task2->SetIoSync(Task1->GetIoSync());
 	    Task2->SetPriority(Task1->GetPriority());
        Task2->DoModal();
    
        CDialogTask *Task3;
        Task3 = CDialogTask::CreateNewObject(IVIEWPROP);
        Task3->SetName(Task2->GetName());
        Task3->SetAlias(Task2->GetAlias());
	    Task3->SetLastModDate("123");
	    Task3->SetLastModUser("abc");
	    Task3->SetCreateDate("456");
	    Task3->SetCreateUser("def");
 	    Task3->SetText1(Task2->GetText1());
	    Task3->SetText2(Task2->GetText2());
	    Task3->SetText3(Task2->GetText3());
	    Task3->SetUrl(Task2->GetUrl());
	    Task3->SetHelpUrl(Task2->GetHelpUrl());
 	    Task3->SetAutoStart(Task2->GetAutoStart());
 	    Task3->SetInterval(Task2->GetInterval());
 	    Task3->SetIoSync(Task2->GetIoSync());
 	    Task3->SetPriority(Task2->GetPriority());
        Task3->DoModal();

        CDialogTask::DeleteObject(Task1);
        CDialogTask::DeleteObject(Task2);
        CDialogTask::DeleteObject(Task3);
    }
    else if(iDispId == 10)
    {
        CStringArray Resources;
        GetResources(Resources);

        CDialogPc *Pc1;
        Pc1 = CDialogPc::CreateNewObject(CVIEWINSERT);
        Pc1->SetResources(Resources);
        Pc1->DoModal();

        CDialogPc *Pc2;
        Pc2 = CDialogPc::CreateNewObject(CVIEWPROP);
        Pc2->SetResources(Resources);
        Pc2->SetName(Pc1->GetName());
        Pc2->SetAlias(Pc1->GetAlias());
	    Pc2->SetLastModDate("123");
	    Pc2->SetLastModUser("abc");
	    Pc2->SetCreateDate("456");
	    Pc2->SetCreateUser("def");
 	    Pc2->SetText1(Pc1->GetText1());
	    Pc2->SetText2(Pc1->GetText2());
	    Pc2->SetText3(Pc1->GetText3());
	    Pc2->SetUrl(Pc1->GetUrl());
	    Pc2->SetHelpUrl(Pc1->GetHelpUrl());
 	    Pc2->SetAddress(Pc1->GetAddress());
 	    Pc2->SetAssignment(Pc1->GetAssignment());
        Pc2->DoModal();
    
        CDialogPc *Pc3;
        Pc3 = CDialogPc::CreateNewObject(IVIEWPROP);
        Pc3->SetResources(Resources);
        Pc3->SetName(Pc2->GetName());
        Pc3->SetAlias(Pc2->GetAlias());
	    Pc3->SetLastModDate("123");
	    Pc3->SetLastModUser("abc");
	    Pc3->SetCreateDate("456");
	    Pc3->SetCreateUser("def");
 	    Pc3->SetText1(Pc2->GetText1());
	    Pc3->SetText2(Pc2->GetText2());
	    Pc3->SetText3(Pc2->GetText3());
	    Pc3->SetUrl(Pc2->GetUrl());
	    Pc3->SetHelpUrl(Pc2->GetHelpUrl());
 	    Pc3->SetAddress(Pc2->GetAddress());
 	    Pc3->SetAssignment(Pc2->GetAssignment());
        Pc3->DoModal();

        CDialogPc::DeleteObject(Pc1);
        CDialogPc::DeleteObject(Pc2);
        CDialogPc::DeleteObject(Pc3);
    }
    else if(iDispId == 11)
    {
        CDialogResource *Resource1;
        Resource1 = CDialogResource::CreateNewObject(CVIEWINSERT);
        Resource1->DoModal();

        CDialogResource *Resource2;
        Resource2 = CDialogResource::CreateNewObject(CVIEWPROP);
        Resource2->SetName(Resource1->GetName());
        Resource2->SetAlias(Resource1->GetAlias());
	    Resource2->SetLastModDate("123");
	    Resource2->SetLastModUser("abc");
	    Resource2->SetCreateDate("456");
	    Resource2->SetCreateUser("def");
 	    Resource2->SetText1(Resource1->GetText1());
	    Resource2->SetText2(Resource1->GetText2());
	    Resource2->SetText3(Resource1->GetText3());
	    Resource2->SetUrl(Resource1->GetUrl());
	    Resource2->SetHelpUrl(Resource1->GetHelpUrl());
 	    Resource2->SetType(Resource1->GetType());
        Resource2->DoModal();
    
        CDialogResource *Resource3;
        Resource3 = CDialogResource::CreateNewObject(IVIEWPROP);
        Resource3->SetName(Resource2->GetName());
        Resource3->SetAlias(Resource2->GetAlias());
	    Resource3->SetLastModDate("123");
	    Resource3->SetLastModUser("abc");
	    Resource3->SetCreateDate("456");
	    Resource3->SetCreateUser("def");
 	    Resource3->SetText1(Resource2->GetText1());
	    Resource3->SetText2(Resource2->GetText2());
	    Resource3->SetText3(Resource2->GetText3());
	    Resource3->SetUrl(Resource2->GetUrl());
	    Resource3->SetHelpUrl(Resource2->GetHelpUrl());
  	    Resource3->SetType(Resource2->GetType());
        Resource3->DoModal();

        CDialogResource::DeleteObject(Resource1);
        CDialogResource::DeleteObject(Resource2);
        CDialogResource::DeleteObject(Resource3);
    }
    else if(iDispId == 12)
    {
        CDialogConstantFile *ConstantFile1;
        ConstantFile1 = CDialogConstantFile::CreateNewObject(CVIEWINSERT);
        ConstantFile1->DoModal();

        CDialogConstantFile *ConstantFile2;
        ConstantFile2 = CDialogConstantFile::CreateNewObject(CVIEWPROP);
        ConstantFile2->SetName(ConstantFile1->GetName());
        ConstantFile2->SetAlias(ConstantFile1->GetAlias());
	    ConstantFile2->SetLastModDate("123");
	    ConstantFile2->SetLastModUser("abc");
	    ConstantFile2->SetCreateDate("456");
	    ConstantFile2->SetCreateUser("def");
 	    ConstantFile2->SetText1(ConstantFile1->GetText1());
	    ConstantFile2->SetText2(ConstantFile1->GetText2());
	    ConstantFile2->SetText3(ConstantFile1->GetText3());
	    ConstantFile2->SetUrl(ConstantFile1->GetUrl());
	    ConstantFile2->SetHelpUrl(ConstantFile1->GetHelpUrl());
        ConstantFile2->DoModal();
    
        CDialogConstantFile *ConstantFile3;
        ConstantFile3 = CDialogConstantFile::CreateNewObject(IVIEWPROP);
        ConstantFile3->SetName(ConstantFile2->GetName());
        ConstantFile3->SetAlias(ConstantFile2->GetAlias());
	    ConstantFile3->SetLastModDate("123");
	    ConstantFile3->SetLastModUser("abc");
	    ConstantFile3->SetCreateDate("456");
	    ConstantFile3->SetCreateUser("def");
 	    ConstantFile3->SetText1(ConstantFile2->GetText1());
	    ConstantFile3->SetText2(ConstantFile2->GetText2());
	    ConstantFile3->SetText3(ConstantFile2->GetText3());
	    ConstantFile3->SetUrl(ConstantFile2->GetUrl());
	    ConstantFile3->SetHelpUrl(ConstantFile2->GetHelpUrl());
        ConstantFile3->DoModal();

        CDialogConstantFile::DeleteObject(ConstantFile1);
        CDialogConstantFile::DeleteObject(ConstantFile2);
        CDialogConstantFile::DeleteObject(ConstantFile3);
    }
    else if(iDispId == 13)
    {
        CDialogGlobVarFile *GlobVarFile1;
        GlobVarFile1 = CDialogGlobVarFile::CreateNewObject(CVIEWINSERT);
        GlobVarFile1->DoModal();

        CDialogGlobVarFile *GlobVarFile2;
        GlobVarFile2 = CDialogGlobVarFile::CreateNewObject(CVIEWPROP);
        GlobVarFile2->SetName(GlobVarFile1->GetName());
        GlobVarFile2->SetAlias(GlobVarFile1->GetAlias());
	    GlobVarFile2->SetLastModDate("123");
	    GlobVarFile2->SetLastModUser("abc");
	    GlobVarFile2->SetCreateDate("456");
	    GlobVarFile2->SetCreateUser("def");
 	    GlobVarFile2->SetText1(GlobVarFile1->GetText1());
	    GlobVarFile2->SetText2(GlobVarFile1->GetText2());
	    GlobVarFile2->SetText3(GlobVarFile1->GetText3());
	    GlobVarFile2->SetUrl(GlobVarFile1->GetUrl());
	    GlobVarFile2->SetHelpUrl(GlobVarFile1->GetHelpUrl());
        GlobVarFile2->DoModal();
    
        CDialogGlobVarFile *GlobVarFile3;
        GlobVarFile3 = CDialogGlobVarFile::CreateNewObject(IVIEWPROP);
        GlobVarFile3->SetName(GlobVarFile2->GetName());
        GlobVarFile3->SetAlias(GlobVarFile2->GetAlias());
	    GlobVarFile3->SetLastModDate("123");
	    GlobVarFile3->SetLastModUser("abc");
	    GlobVarFile3->SetCreateDate("456");
	    GlobVarFile3->SetCreateUser("def");
 	    GlobVarFile3->SetText1(GlobVarFile2->GetText1());
	    GlobVarFile3->SetText2(GlobVarFile2->GetText2());
	    GlobVarFile3->SetText3(GlobVarFile2->GetText3());
	    GlobVarFile3->SetUrl(GlobVarFile2->GetUrl());
	    GlobVarFile3->SetHelpUrl(GlobVarFile2->GetHelpUrl());
        GlobVarFile3->DoModal();

        CDialogGlobVarFile::DeleteObject(GlobVarFile1);
        CDialogGlobVarFile::DeleteObject(GlobVarFile2);
        CDialogGlobVarFile::DeleteObject(GlobVarFile3);
    }
    else if(iDispId == 14)
    {
        CDialogProgInst *ProgInst1;
        ProgInst1 = CDialogProgInst::CreateNewObject(CVIEWINSERT);
        ProgInst1->DoModal();

        CDialogProgInst *ProgInst2;
        ProgInst2 = CDialogProgInst::CreateNewObject(CVIEWPROP);
        ProgInst2->SetName(ProgInst1->GetName());
        ProgInst2->SetAlias(ProgInst1->GetAlias());
        ProgInst2->SetProgClass(ProgInst1->GetProgClass());
	    ProgInst2->SetLastModDate("123");
	    ProgInst2->SetLastModUser("abc");
	    ProgInst2->SetCreateDate("456");
	    ProgInst2->SetCreateUser("def");
 	    ProgInst2->SetText1(ProgInst1->GetText1());
	    ProgInst2->SetText2(ProgInst1->GetText2());
	    ProgInst2->SetText3(ProgInst1->GetText3());
	    ProgInst2->SetUrl(ProgInst1->GetUrl());
	    ProgInst2->SetHelpUrl(ProgInst1->GetHelpUrl());
        ProgInst2->DoModal();
    
        CDialogProgInst *ProgInst3;
        ProgInst3 = CDialogProgInst::CreateNewObject(IVIEWPROP);
        ProgInst3->SetName(ProgInst2->GetName());
        ProgInst3->SetAlias(ProgInst2->GetAlias());
        ProgInst3->SetProgClass(ProgInst2->GetProgClass());
	    ProgInst3->SetLastModDate("123");
	    ProgInst3->SetLastModUser("abc");
	    ProgInst3->SetCreateDate("456");
	    ProgInst3->SetCreateUser("def");
 	    ProgInst3->SetText1(ProgInst2->GetText1());
	    ProgInst3->SetText2(ProgInst2->GetText2());
	    ProgInst3->SetText3(ProgInst2->GetText3());
	    ProgInst3->SetUrl(ProgInst2->GetUrl());
	    ProgInst3->SetHelpUrl(ProgInst2->GetHelpUrl());
        ProgInst3->DoModal();

        CDialogProgInst::DeleteObject(ProgInst1);
        CDialogProgInst::DeleteObject(ProgInst2);
        CDialogProgInst::DeleteObject(ProgInst3);
    }
    else if(iDispId == 15)
    {
        CDialogProj *Proj1;
        Proj1 = CDialogProj::CreateNewObject(CVIEWINSERT);
        Proj1->DoModal();

        CDialogProj *Proj2;
        Proj2 = CDialogProj::CreateNewObject(CVIEWPROP);
        Proj2->SetName(Proj1->GetName());
        Proj2->SetAlias(Proj1->GetAlias());
        Proj2->SetLocation(Proj1->GetLocation());
	    Proj2->SetLastModDate("123");
	    Proj2->SetLastModUser("abc");
	    Proj2->SetCreateDate("456");
	    Proj2->SetCreateUser("def");
 	    Proj2->SetText1(Proj1->GetText1());
	    Proj2->SetText2(Proj1->GetText2());
	    Proj2->SetText3(Proj1->GetText3());
	    Proj2->SetUrl(Proj1->GetUrl());
	    Proj2->SetHelpUrl(Proj1->GetHelpUrl());
        Proj2->DoModal();
    
        CDialogProj *Proj3;
        Proj3 = CDialogProj::CreateNewObject(IVIEWPROP);
        Proj3->SetName(Proj2->GetName());
        Proj3->SetAlias(Proj2->GetAlias());
        Proj3->SetLocation(Proj2->GetLocation());
	    Proj3->SetLastModDate("123");
	    Proj3->SetLastModUser("abc");
	    Proj3->SetCreateDate("456");
	    Proj3->SetCreateUser("def");
 	    Proj3->SetText1(Proj2->GetText1());
	    Proj3->SetText2(Proj2->GetText2());
	    Proj3->SetText3(Proj2->GetText3());
	    Proj3->SetUrl(Proj2->GetUrl());
	    Proj3->SetHelpUrl(Proj2->GetHelpUrl());
        Proj3->DoModal();

        CDialogProj::DeleteObject(Proj1);
        CDialogProj::DeleteObject(Proj2);
        CDialogProj::DeleteObject(Proj3);
    }
}

void GetResources(CStringArray &Resources)
{
    Resources.Add(_T("Resource1"));
    Resources.Add(_T("Resource2"));
    Resources.Add(_T("Resource3"));
    Resources.Add(_T("Resource4"));
}

void GetTypes(CStringArray &Types)
{
    Types.Add(_T("BOOL"));
    Types.Add(_T("BYTE"));
    Types.Add(_T("WORD"));
    Types.Add(_T("DWORD"));
    Types.Add(_T("LWORD"));
    Types.Add(_T("DINT"));
    Types.Add(_T("LINT"));
    Types.Add(_T("LREAL"));
    Types.Add(_T("TIME"));
    Types.Add(_T("DT"));
    Types.Add(_T("DATE"));
    Types.Add(_T("TOD"));
    Types.Add(_T("WSTRING"));
}
