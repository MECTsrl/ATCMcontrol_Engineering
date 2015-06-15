


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "InsertDataType.h"
#include "DialogDataType.h"
#include "CEContainer.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInsertDataType::CInsertDataType(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_DataType = NULL;
    m_sName = _T("DataType");
//    CreateGUID();
}

CInsertDataType::~CInsertDataType()
{
}

BOOL CInsertDataType::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    BOOL bRes = !DoesFileExist(m_sName, FALSE);
    if(bRes)
    {
        GenDataType(m_DataTypeText);
        InsertVarList(m_DataTypeText, m_DataType);
        WriteFile(m_DataTypeText);
        InformProjectManager();
    }
    m_DataTypeText.Empty();
    m_aVarList.RemoveAll();
    return bRes;
}

void CInsertDataType::GenDataType(CString & StrOut)
{
    CString StrHelp;
    CString StrBlank = _T(' ');
    StrOut = _T("");

    GetStdAttribString(StrOut);
    StrHelp.LoadString(IDS_KEY_TYPE);
    StrOut += StrHelp;
    StrOut += NEWLINE;
    StrOut += SPACETAB;
	StrHelp.Format(IDS_KEY_STRUCT, m_sName);
    StrOut += StrHelp;
    StrOut += NEWLINE;
    StrOut += SPACETAB;
    StrHelp.LoadString(IDS_KEY_ENDSTRUCT);
    StrOut += StrHelp;
    StrHelp.LoadString(IDS_KEY_SEMICOL);
    StrOut += StrHelp;
    StrOut += NEWLINE;
    StrHelp.LoadString(IDS_KEY_ENDTYPE);
    StrOut += StrHelp;
}

void CInsertDataType::SetFileName()
{
    m_sFileName = m_sName + _T(".sdt"); //DataType file
}

CDialogDataType* CInsertDataType::GetDialog()
{
    return (CDialogDataType*) m_pDialog;
}

void CInsertDataType::Init()
{
    CDialogDataType* pDialog = GetDialog();
    ASSERT(pDialog);
    if(!pDialog)
    {
        return;
    }
    m_sAlias          = pDialog->GetAlias();
    m_sName           = pDialog->GetName();
    m_sUsrText[0]     = pDialog->GetText1();
    m_sUsrText[1]     = pDialog->GetText2();
    m_sUsrText[2]     = pDialog->GetText3();
    m_sHelpUrl        = pDialog->GetHelpUrl();
    m_sUrl            = pDialog->GetUrl();
    m_pMembers        = pDialog->GetMembers();
    m_sHelpTemplate   = pDialog->GetPathOfSelectedTemplate();
    
    GetCurrentUserName(m_sCreateUser);
    m_sLastModUser = m_sCreateUser;
    // extract variables from safe array
    if (m_pMembers)
    {
        long lSize;
        long lLBound;
        long lUBound;
        long arrIndex[2];
        UINT uiDim = ::SafeArrayGetDim(m_pMembers);
        CVar var;

        ASSERT(2 == uiDim);
        ::SafeArrayGetLBound(m_pMembers, 1, &lLBound);
        ::SafeArrayGetUBound(m_pMembers, 1, &lUBound);
        lSize = lUBound - lLBound + 1;
        
        ::SafeArrayGetLBound(m_pMembers, 2, &lLBound);
        ASSERT(0 == lLBound);
        ::SafeArrayGetUBound(m_pMembers, 2, &lUBound);
        ASSERT(7 == lUBound);
        
        for (long li=0; li<lSize; li++)
        {
            BSTR sVarName;
            BSTR sVarAlias;
            BSTR sVarComment;
            BSTR sVarType;
            BSTR sVarInitVal;
            BSTR sIP;
            BSTR sLength;
            BSTR sHint;
            CString strVarName;
            CString strVarAlias;
            CString strVarComment;
            CString strVarType;
            CString strVarInitVal;
            CString strIP;
            CString strLength;
            CString strHint;
            
            arrIndex[0] = li;
            
            arrIndex[1] = 0;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarName);
            strVarName = sVarName;
            SysFreeString(sVarName);
            
            arrIndex[1] = 1;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarAlias);
            strVarAlias = sVarAlias;
            SysFreeString(sVarAlias);

            arrIndex[1] = 2;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarType);
            strVarType = sVarType;
            SysFreeString(sVarType);
            
            arrIndex[1] = 3;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarInitVal);
            strVarInitVal = sVarInitVal;
            SysFreeString(sVarInitVal);

            arrIndex[1] = 4;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sVarComment);
            strVarComment = sVarComment;
            SysFreeString(sVarComment);
            
            arrIndex[1] = 5;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sIP);
            strIP = sIP;
            SysFreeString(sIP);

            arrIndex[1] = 6;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sLength);
            strLength = sLength;
            SysFreeString(sLength);

            arrIndex[1] = 7;
            ::SafeArrayGetElement(m_pMembers, arrIndex, &sHint);
            strHint = sHint;
            SysFreeString(sHint);
                      
            var.SetObjName(strVarName);
            var.SetObjAlias(strVarAlias);
            var.SetObjVarComment(strVarComment);
            var.SetVarType(strVarType);
            CString strVarClass = _T("STRUCT");
            var.SetVarStorageClass(strVarClass);
            var.SetVarInitVal(strVarInitVal);
            var.SetIP(_ttoi((LPCTSTR)strIP));
            var.SetVarDeclRange(_ttoi((LPCTSTR)strLength));
            var.SetHint(strHint);
            AddVar(var);
        }
    }
    
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

void CInsertDataType::FillDialog()
{
    //Standart name
    CString sName = GetNameProposal(m_sName);
    CDialogDataType*    pDialog = GetDialog();
    ASSERT(pDialog);
    if(!pDialog)
    {
        return;
    }
    pDialog->SetCreateUser(m_sCreateUser);
    pDialog->SetCreateDate(m_sCreateDate);
    pDialog->SetLastModUser(m_sLastModUser);
    pDialog->SetLastModDate(m_sLastModDate);
    
    pDialog->SetName(sName);
}

int CInsertDataType::InsertNewVar(CVar & var, CString & sPouText, POU*  Pou)
{
    CString strOut;
	CString strHelp,strInsert;
	bool bSection;
	StringPos IP;

	var.GetInsertString(strInsert);
    //Get the actual Pou with the actual Text
    Pou = COperation::ParsePou(sPouText, Pou);
    //Get the Insertion point
    IP = GetIP(var, bSection, Pou);

    delete Pou;
    
    //strOut += SPACETAB;
    strOut = strInsert;
    strOut += NEWLINE;
    strOut += SPACETAB;
    sPouText.Insert(IP, strOut);
    return 0;
}

int CInsertDataType::InsertVarList(CString & sPouText, POU*  Pou)
{
	int iRet;
	CVar var;

	if(!m_aVarList.GetCount())
    {
		return 0;
    }

    POSITION pos = m_aVarList.GetHeadPosition();
	for(int i = 0;i< m_aVarList.GetCount();i++) 
    {
		var = m_aVarList.GetNext(pos);
		//var.SetClass(m_sName);
		if(var.GetHint() == _T("Mod"))
        {
            iRet = ModifyVar(var, sPouText, Pou);
        }
        else if (var.GetHint() == _T("Del"))
        {
            iRet = DeleteVar(var, sPouText, Pou);
        }
        else if(var.GetHint() == _T("Add"))
        {
            iRet = InsertNewVar(var, sPouText, Pou);
        }
        else if(var.GetHint() == _T("No"))
        {
            ;
        }
	}
	return 0;
}

