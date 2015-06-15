

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "PropDataType.h"
#include "DialogDataType.h"
#include "CEContainer.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPropDataType::CPropDataType(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_DataType = NULL;
}

CPropDataType::~CPropDataType()
{
    delete m_DataType;
    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
}

BOOL CPropDataType::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    Init();
    InsertVarList(m_DataTypeText, m_DataType);
    InsertSysComment2(m_DataTypeText, m_DataType);
    // SIS: bug fix d-2019
    // create help file if template is not empty
    if(!m_sHelpTemplate.IsEmpty())
    {
        CreateUsrHelp();
    }
    WriteFile(m_DataTypeText);
    InformProjectManager();

    return TRUE;
}

void CPropDataType::Init()
{
    m_sAlias          = GetDialog()->GetAlias();
    m_sName           = GetDialog()->GetName();
    m_sUsrText[0]     = GetDialog()->GetText1();
    m_sUsrText[1]     = GetDialog()->GetText2();
    m_sUsrText[2]     = GetDialog()->GetText3();
    m_sHelpUrl        = GetDialog()->GetHelpUrl();
    m_sUrl            = GetDialog()->GetUrl();
    m_sHelpTemplate   = GetDialog()->GetPathOfSelectedTemplate();
    
    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
    m_pMembers        = GetDialog()->GetMembers();

    m_aVarList.RemoveAll();

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
            CString strVarClass = _T("STRUCT");
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

void CPropDataType::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}

void CPropDataType::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    if(!m_sHelpUrl.IsEmpty())
    {
        CreateUsrHelpLink();
    }
    hRes = m_pICEProjectInfo->fileParseNow(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


CDialogDataType* CPropDataType::GetDialog()
{
    return (CDialogDataType*) m_pDialog;
}

void CPropDataType::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
 
    SetFileName();
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_DataTypeText = buf.c_str();
    m_DataType = (StStruct *)ParsePou(m_DataTypeText, m_DataType);
    InitFromFile(m_DataType);
    //
    //create the safe array for the CVar to write them in the dialogs
	SAFEARRAYBOUND bound[2];
    HRESULT hr = S_OK;
	long arrIndex[2];
    
    bound[0].lLbound = 0;
    bound[0].cElements = m_aVarList.GetCount(); 
    bound[1].lLbound = 0;
    bound[1].cElements = 8; 
    
    //create the safe array for the CVar list m_aVarList
    m_pMembers = ::SafeArrayCreate(VT_BSTR , 2, bound);
    if (!m_pMembers)
    {
        return;
    }
    
    POSITION pos = m_aVarList.GetHeadPosition();
    for (long i=bound[0].lLbound; i < bound[0].cElements; i++) 
    {
        arrIndex[0] = (bound[0].cElements - i - 1);//Convert the order of the variables
        //copy from the CVar array into a safe array
        CVar var1(m_aVarList.GetNext(pos));
        CString strTmp;
        CArray<CString, CString> Dummy;//for converting CVar -> Element of SAFEARRAY
        //Dummy.Add((var1.GetVarStorageClass()).AllocSysString());
        Dummy.Add(var1.GetObjName());
        Dummy.Add(var1.GetObjAlias());
        Dummy.Add(var1.GetVarType());
        Dummy.Add(var1.GetVarInitVal());
        Dummy.Add(var1.GetObjVarComment());
        strTmp.Format("%d", var1.GetIP());
        Dummy.Add(strTmp);
        strTmp.Format("%d", var1.GetVarDeclRange());
        Dummy.Add(strTmp);
        Dummy.Add(var1.GetHint());
        for (long k = bound[1].lLbound; k < bound[1].cElements; k++)
        {
            arrIndex[1] = k;
            CString strDummy = Dummy[k];
            BSTR bstrDummy = strDummy.AllocSysString();
            hr = ::SafeArrayPutElement(m_pMembers, arrIndex, (void*)bstrDummy);
            SysFreeString(bstrDummy);
            if(FAILED(hr)) 
            {
                return;
            }
        }
    }
    //
    // Was COperation::FillDialog();
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

    GetDialog()->SetMembers(m_pMembers);
    m_pMembers = NULL;
}

int CPropDataType::InsertNewVar(CVar & var, CString & sPouText, POU*  Pou)
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

int CPropDataType::InsertVarList(CString & sPouText, POU*  Pou)
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

