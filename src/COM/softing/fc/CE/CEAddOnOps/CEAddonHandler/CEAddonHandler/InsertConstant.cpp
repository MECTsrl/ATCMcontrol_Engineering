


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <fstream>
#include "DllInterface.h"
#include "InsertConstant.h"
#include "DialogConst.h"
#include "CEContainer.h"
#include "resource.h"
#include "Operation.h"
#include "SBLLexTree.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::CInsertConstant
 * Description			
 * @param			CPropertySheetBase *Dialog
 * @param			BSTR sSourceFile
 * @param			BSTR sId
 * @param			ICEProjInfo *pICEProjectInfo
 * @param			BSTR sAdditionalInfo
 * @return			
 * @exception			
 * @see			
*/
CInsertConstant::CInsertConstant(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_pConstList = NULL;
    m_sName = _T("Const");
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::~CInsertConstant
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertConstant::~CInsertConstant()
{
    if (m_pConstList != NULL)
    {
        delete m_pConstList;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertConstant::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    Init();
    SetFileName();
    ParseConstFile();
    InitFromFile(m_pConstList);
    if(DoesConstExist(m_Const.GetObjName()))
    {
        return FALSE;
    }
    InsertConst();
    CString ConstFileText = CString(m_ConstFileText.c_str());
    WriteFile(ConstFileText);
    InformProjectManager();
    
    m_ConstFileText.empty();
    m_aVarList.RemoveAll();
    ConstFileText.Empty();
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::ParseConstFile
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstant::ParseConstFile()
{
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    m_ConstFileText);

    if(m_pConstList) {
        delete m_pConstList;
        m_pConstList = NULL;
    }
    parse_string(m_ConstFileText, &m_pConstList);
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::GenConst
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstant::GenConst(CString & StrOut)
{
    ;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::InsertTask
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstant::InsertConst()
{
    //m_ConstFileText.insert(m_ConstList->getNextTaskConfInsertPos(), m_ConstText);
    CString ConstFileText = CString(m_ConstFileText.c_str());
    InsertNewVar(m_Const, ConstFileText, m_pConstList);
    m_ConstFileText = ConstFileText;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstant::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstant::SetFileName()
{
    m_sFileName = m_sSourceFile;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::GetDialog
 * Description			
 * @return			CDialogTask* 
 * @exception			
 * @see			
*/
CDialogConst* CInsertConstant::GetDialog()
{
    return (CDialogConst*) m_pDialog;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstant::Init()
{
    //Init Constant from Dialog
    CString Dummy;
    Dummy = GetDialog()->GetName();
    m_Const.SetObjName(Dummy);
    Dummy = GetDialog()->GetAlias();
    m_Const.SetObjAlias(Dummy);
    Dummy = GetDialog()->GetVarComment();
    m_Const.SetObjVarComment(Dummy);
    Dummy = GetDialog()->GetType();
    m_Const.SetVarType(Dummy);
    Dummy.LoadString(IDS_KEY_CONST);
    m_Const.SetVarStorageClass(Dummy);
    int iClass = VarStorageClassByString(Dummy);
    ASSERT(iClass != -1);
    m_Const.SetVarAttr(iClass);
    Dummy = GetDialog()->GetValue();
    m_Const.SetVarInitVal(Dummy);

    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertConstant::FillDialog()
{
    SetFileName();
    ParseConstFile();
    if(!m_pConstList)
    {
        return;
    }
    InitFromFile(m_pConstList);
    //Standart name
    CString strName = _T("Const");
    strName = GetNameProposal(strName);
    
    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    
    GetDialog()->SetName(strName);
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::DoesConstExist
 * Description			
 * @param			CString strConstName
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CInsertConstant::DoesConstExist(CString strConstName)
{
    ASSERT(!strConstName.IsEmpty());
    CVar Var;
    CString strHelp;

    int iCount = m_aVarList.GetCount();
    POSITION pos = m_aVarList.GetHeadPosition();
    for(int iIndex = 0; iIndex < iCount; iIndex++)
    {    
        if(pos)
        {
            strHelp = (m_aVarList.GetNext(pos)).GetObjName();
            if(!strConstName.CompareNoCase(strHelp))
            {
                if(!m_bSilent)
                {
                    CString errorMsg;
                    errorMsg.Format(IDS_CONSTNAME_EXISTS, strConstName);
                    ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
                }
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertConstant::GetNameProposal
 * Description			
 * @param			CString strBasis
 * @return			CString 
 * @exception			
 * @see			
*/
CString CInsertConstant::GetNameProposal(CString strBasis)
{
    ASSERT(!strBasis.IsEmpty());
    
    CSBLLexTree<bool>   tAssoc;             // association memory for names
    bool*               pbTmp = (bool*) 1;  // dummy element for insertion

    int iIndex = 1;
    CString strIndex = _T("");
    CString strConstName = _T("");
    CString  strConstNameFromList = _T("");
    POSITION pos = m_aVarList.GetHeadPosition();

    while(pos)
    {
        strConstNameFromList = (m_aVarList.GetNext(pos)).GetObjName();
        tAssoc.insertData(strConstNameFromList, pbTmp);
    }

    for(iIndex = 1; iIndex < MAX_PROPOSAL_COUNT; ++iIndex)
    {    
        strConstName.Format("%s%d", strBasis, iIndex);
        if(!tAssoc.getData(strConstName))
        {
            break;
        }
    }
    return strConstName;
}


