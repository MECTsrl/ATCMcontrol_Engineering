


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <fstream>
#include "DllInterface.h"
#include "InsertGlobVar.h"
#include "DialogGlobVar.h"
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
 * Function name			CInsertGlobVar::CInsertGlobVar
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
CInsertGlobVar::CInsertGlobVar(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_pGlobVarList = NULL;
    m_sName = _T("GlobVar");
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::~CInsertGlobVar
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertGlobVar::~CInsertGlobVar()
{
    if (m_pGlobVarList != NULL)
    {
        delete m_pGlobVarList;
        m_pGlobVarList = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertGlobVar::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    SetFileName();
    ParseGlobVarFile();
    if(DoesGVarExist(m_GlobVar.GetObjName()))
    {
        return FALSE;
    }
    InsertGlobVar();
    CString GlobVarFileText = CString(m_GlobVarFileText.c_str());
    WriteFile(GlobVarFileText);
    m_GlobVarFileText.empty();
    m_aVarList.RemoveAll();
    InformProjectManager();

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::ParseConstFile
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVar::ParseGlobVarFile()
{
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    m_GlobVarFileText);

    if (m_pGlobVarList != NULL) //remove glob var list if existing
    {
        delete m_pGlobVarList;
        m_pGlobVarList = NULL;
    }
    parse_string(m_GlobVarFileText, &m_pGlobVarList);
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::GenConst
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVar::GenGlobVar(CString & StrOut)
{
    ;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::InsertTask
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVar::InsertGlobVar()
{
    //m_ConstFileText.insert(m_ConstList->getNextTaskConfInsertPos(), m_ConstText);
    CString GlobVarFileText = CString(m_GlobVarFileText.c_str());
    InsertNewVar(m_GlobVar, GlobVarFileText, m_pGlobVarList);
    m_GlobVarFileText = GlobVarFileText;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVar::InformProjectManager()
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
void CInsertGlobVar::SetFileName()
{
    m_sFileName = m_sSourceFile;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::GetDialog
 * Description			
 * @return			CDialogTask* 
 * @exception			
 * @see			
*/
CDialogGlobVar* CInsertGlobVar::GetDialog()
{
    return (CDialogGlobVar*) m_pDialog;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVar::Init()
{
    //Init Constant from Dialog
    CString Dummy;
    Dummy = GetDialog()->GetName();
    m_GlobVar.SetObjName(Dummy);
    Dummy = GetDialog()->GetAlias();
    m_GlobVar.SetObjAlias(Dummy);
    Dummy = GetDialog()->GetVarComment();
    m_GlobVar.SetObjVarComment(Dummy);
    Dummy = GetDialog()->GetType();
    m_GlobVar.SetVarType(Dummy);
    Dummy = GetDialog()->GetModifier();
    if(Dummy == _T("VAR_GLOBAL CONSTANT"))
    {
        Dummy = _T("VAR_GLOBAL CONST");
    }
    m_GlobVar.SetVarStorageClass(Dummy);
    int iClass = VarStorageClassByString(Dummy);
    ASSERT(iClass != -1);
    m_GlobVar.SetVarAttr(iClass);
    Dummy = GetDialog()->GetValue();
    m_GlobVar.SetVarInitVal(Dummy);

    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertGlobVar::FillDialog()
{
    SetFileName();
    ParseGlobVarFile();
    if(!m_pGlobVarList)
    {
        return;
    }
    InitFromFile(m_pGlobVarList);
    //Standart name
    CString strName = _T("GlobVar");
    strName = GetNameProposal(strName);
    
    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    
    GetDialog()->SetName(strName);
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertGlobVar::DoesConstExist
 * Description			
 * @param			CString strGlobVarName
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CInsertGlobVar::DoesGVarExist(CString strGlobVarName)
{
    ASSERT(!strGlobVarName.IsEmpty());
    CVar Var;
    CString strHelp;

    int iCount = m_aVarList.GetCount();
    POSITION pos = m_aVarList.GetHeadPosition();
    for(int iIndex = 0; iIndex < iCount; iIndex++)
    {    
        if(pos)
        {
            strHelp = (m_aVarList.GetNext(pos)).GetObjName();
            if(!strGlobVarName.CompareNoCase(strHelp))
            {
                if(!m_bSilent)
                {
                    CString errorMsg;
                    errorMsg.Format(IDS_GVARNAME_EXISTS, strGlobVarName);
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
 * Function name			CInsertGlobVar::GetNameProposal
 * Description			
 * @param			CString strBasis
 * @return			CString 
 * @exception			
 * @see			
*/
CString CInsertGlobVar::GetNameProposal(CString strBasis)
{
    ASSERT(!strBasis.IsEmpty());
    
    CSBLLexTree<bool>   tAssoc;             // association memory for names
    bool*               pbTmp = (bool*) 1;  // dummy element for insertion

    int         iIndex = 1;
    CString     strIndex;
    CString     strGVarName;
    CString     strGVarNameFromList;
    POSITION    pos = m_aVarList.GetHeadPosition();

    strGVarName.Format("%s%d", strBasis, iIndex);

    while(pos)
    {
        strGVarNameFromList = (m_aVarList.GetNext(pos)).GetObjName();
        tAssoc.insertData(strGVarNameFromList, pbTmp);
    }

    for(iIndex = 1; iIndex < MAX_PROPOSAL_COUNT; ++iIndex)
    {    
        strGVarName.Format("%s%d", strBasis, iIndex);
        if(!tAssoc.getData(strGVarName))
        {
            break;
        }
    }
    return strGVarName;
}





