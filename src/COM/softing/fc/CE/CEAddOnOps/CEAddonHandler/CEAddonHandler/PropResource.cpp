


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "PropResource.h"
#include "DialogResource.h"
#include "CEContainer.h"

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------*
/**
 * Function name			CPropResource::CPropResource
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
CPropResource::CPropResource(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo) ,
    m_ResourceText()//, m_sResType()
{
    m_Resource = NULL;
//    m_iResTypePos = 0;
//    m_iResTypeLength = 0;
}


//------------------------------------------------------------------*
/**
 * Function name			CPropResource::~CPropResource
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CPropResource::~CPropResource()
{
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CPropResource::Execute
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CPropResource::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
//    InsertResourceType(m_ResourceText);
    InsertSysComment2(m_ResourceText, m_Resource);
    WriteFile(m_ResourceText);
    InformProjectManager();

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CPropResource::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CPropResource::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
    m_sUsrText[1]     = GetDialog()->GetText2();
    m_sUsrText[2]     = GetDialog()->GetText3();
    m_sHelpUrl        = GetDialog()->GetHelpUrl();
    m_sUrl            = GetDialog()->GetUrl();
//    m_sResType        = GetDialog()->GetType();

    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}


//------------------------------------------------------------------*
/**
 * Function name			CPropResource::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CPropResource::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}


//------------------------------------------------------------------*
/**
 * Function name			CPropResource::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CPropResource::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}



//------------------------------------------------------------------*
/**
 * Function name			CPropResource::GetDialog
 * Description			
 * @return			CDialogResource* 
 * @exception			
 * @see			
*/
CDialogResource* CPropResource::GetDialog()
{
    return (CDialogResource*) m_pDialog;
}


//------------------------------------------------------------------*
/**
 * Function name			CPropResource::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CPropResource::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
  
    SetFileName();
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_ResourceText = buf.c_str();
    ASSERT(!m_ResourceText.IsEmpty());
    m_Resource = (Resource *)ParsePou(m_ResourceText, m_Resource);
    InitFromFile(m_Resource);
    COperation::FillDialog();
    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    GetDialog()->SetAlias(m_sAlias);
    GetDialog()->SetName(m_sName);
    GetDialog()->SetText1(m_sUsrText[0]);
	GetDialog()->SetText2(m_sUsrText[1]);
	GetDialog()->SetText3(m_sUsrText[2]);
	GetDialog()->SetHelpUrl(m_sHelpUrl);
	GetDialog()->SetUrl(m_sUrl);
//    GetDialog()->SetType(m_sResType);
}

void CPropResource::InitFromFile(Resource* Pou)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    ASSERT(Pou);
    COperation::Init();
    SetPouDT(FALSE); //TRUE for Insert (FALSE for Prop)

    //System commentar
    COperation::InitFromFile(Pou);
    //Resource type
//    string ResType = Pou->getTarget();
//    m_sResType = ResType.c_str();
//    StringPosRange TypeRange = Pou->getTargetRange();
//    StringPosRange NameRange = Pou->getNameRange();
//    m_iResTypePos = NameRange.getLast();
//    if(!m_sResType.IsEmpty())
//    {
//        m_iResTypeLength = TypeRange.getLast() - m_iResTypePos;
//    }
}

//void CPropResource::InsertResourceType(CString & ResourceText)
//{
//    CString StrHelp,Strlf(NEWLINE),StrOut(_T("")),StrIndent(SPACETAB);
//    ResourceText.Delete(m_iResTypePos, m_iResTypeLength);
//    
//    if(!m_sResType.IsEmpty())
//    {
//        StrOut += StrIndent;
//        StrOut += StrIndent;
//        StrHelp.Format(IDS_KEY_RES_ON, m_sResType);
//        StrOut += StrHelp;
//    }
//    ResourceText.Insert(m_iResTypePos, StrOut);
//}

