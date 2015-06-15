
#include "stdafx.h"
#include "XML_Wrapper.h"
#include "TypeDBI.h"
#include "DBIContext.h"
#include "InstanceDBI.h"
#include "utilif.h"
#include "fc_tools\fc_string.h"
#include "CG_AutoDecl_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_POUS _T ("POUS")
#define DBI_XML_PROGRAM _T ("PROGRAM")
#define DBI_XML_FUNCTIONBLOCK _T ("FUNCTION_BLOCK")
#define DBI_XML_STRUCT _T ("STRUCT")
#define DBI_XML_RESOURCE _T ("RESOURCE")
#define DBI_XML_GVLS _T ("GVLS")
#define DBI_XML_CUSTOMGLOBAL _T ("CUSTOMGLOBAL")
#define DBI_XML_VARIABLES _T ("VARIABLES")
#define DBI_XML_BREAKPOINTS _T ("BREAKPOINTS")
#define DBI_XML_INSTANCES _T ("INSTANCES")
#define DBI_XML_ID _T ("ID")
#define DBI_XML_SOURCE _T ("SOURCE")
#define DBI_XML_INSTLIST _T ("INSTLIST")
#define DBI_XML_INST _T ("INST")
#define DBI_XML_TYPE _T ("TYPE")
#define DBI_XML_DBI _T ("DBI")
#define DBI_XML_SFC _T ("SFC")

#define DBI_STEP_ACTIVE_FIELD _T ("X")
#define DBI_ACTION_ACTIVE_FIELD _T ("A")


CTypeDBI::CTypeDBI ()
: m_Kind (DBIAliasType),
  m_bLoaded (false),
  m_bLoading (false),
  m_IECType (BL_ANY),
  m_ulStringLength (0),
  m_pElementType (NULL),
  m_lLowerBound (0),
  m_lUpperBound (0),
  m_Timestamp (0),
  m_Language (DBIST),
  m_pIAutoDecl(NULL)
{
}


CTypeDBI::~CTypeDBI ()
{
    Unload ();

    if (m_pElementType != NULL)
    {
        m_pElementType->RemoveParent (this);
        m_pElementType->Release ();
    };

    if (m_pContext != NULL)
    {
        m_pContext->TypeDieing (this);
    };

    if(m_pIAutoDecl)
        m_pIAutoDecl->Release();
}


HRESULT CTypeDBI::Init (CDBIContext *pContext, BL_IEC_TYP iecType)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_Kind = DBISimpleType;
    m_IECType = iecType;

    return (S_OK);
}


HRESULT CTypeDBI::Init (CDBIContext *pContext, BL_IEC_TYP iecType, unsigned long ulStringLength)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_Kind = DBISimpleType;
    m_IECType = iecType;
    m_ulStringLength = ulStringLength;

    return (S_OK);
}


HRESULT CTypeDBI::Init (CDBIContext *pContext, CTypeDBI *pElementType, long lLowerBound, long lUpperBound)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    ASSERT (pElementType != NULL);
    if (pElementType == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    pElementType->AddRef ();
    pElementType->AddParent (this);

    m_Kind = DBIArrayType;
    m_pElementType = pElementType;
    m_lLowerBound = lLowerBound;
    m_lUpperBound = lUpperBound;

    return (S_OK);
}


HRESULT CTypeDBI::Init (CDBIContext *pContext, const CString &strTypeName, const CString &strNamespace, const CString &strDBIFile)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    ASSERT (!strTypeName.IsEmpty ());
    if (strTypeName.IsEmpty ())
    {
        return (E_INVALIDARG);
    };

    SetContext (pContext);

    m_Kind = DBIAliasType;
    m_strTypeName = strTypeName;
    m_strDBIFile = strDBIFile;

    return (S_OK);
}


HRESULT CTypeDBI::Init (CDBIContext *pContext, DBITypeKind kind)
{
    HRESULT hr;

    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    ASSERT (kind == DBIConfigurationType || kind == DBIStepType || kind == DBIActionType || kind == DBITransitionType);
    if (kind != DBIConfigurationType && kind != DBIStepType && kind != DBIActionType && kind != DBITransitionType)
    {
        return (E_INVALIDARG);
    };

    SetContext (pContext);

    m_Kind = kind;

    if (kind == DBIStepType)
    {
        CTypeDBI *pType;

        pType = pContext->GetSimpleType (BL_BOOL);

        hr = m_Variables.AddVariable (pContext, DBIVar, DBI_STEP_ACTIVE_FIELD, pType, this);
        if (FAILED (hr))
        {
            pType->Release ();

            SetContext (NULL);
            return (hr);
        };

        pType->Release ();
    }
    else if (kind == DBIActionType)
    {
        CTypeDBI *pType;

        pType = pContext->GetSimpleType (BL_BOOL);

        hr = m_Variables.AddVariable (pContext, DBIVar, DBI_ACTION_ACTIVE_FIELD, pType, this);
        if (FAILED (hr))
        {
            pType->Release ();

            SetContext (NULL);
            return (hr);
        };

        pType->Release ();
    };

    return (S_OK);
}


DBITypeKind CTypeDBI::GetKind ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_Kind);
}


BL_IEC_TYP CTypeDBI::GetIECType ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    ASSERT (m_Kind == DBISimpleType);
    if (m_Kind != DBISimpleType)
    {
        throw (E_FAIL);
    };

    return (m_IECType);
}


unsigned long CTypeDBI::GetStringLength ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    ASSERT (m_Kind == DBISimpleType);
    if (m_Kind != DBISimpleType)
    {
        throw (E_FAIL);
    };

    ASSERT (m_IECType == BL_WSTRING || m_IECType == BL_STRING);
    if (m_IECType != BL_WSTRING && m_IECType != BL_STRING)
    {
        throw (E_FAIL);
    };

    return (m_ulStringLength);
}


CTypeDBI *CTypeDBI::GetElementType ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    ASSERT (m_Kind == DBIArrayType);
    if (m_Kind != DBIArrayType)
    {
        throw (E_FAIL);
    };

    return (m_pElementType);
}


long CTypeDBI::GetLowerBound ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    ASSERT (m_Kind == DBIArrayType);
    if (m_Kind != DBIArrayType)
    {
        throw (E_FAIL);
    };

    return (m_lLowerBound);
}


long CTypeDBI::GetUpperBound ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    ASSERT (m_Kind == DBIArrayType);
    if (m_Kind != DBIArrayType)
    {
        throw (E_FAIL);
    };

    return (m_lUpperBound);
}


CString CTypeDBI::GetSource ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    ASSERT (m_Kind == DBIProgramType || m_Kind == DBIFunctionBlockType || m_Kind == DBIStructType);
    if (m_Kind != DBIProgramType && m_Kind != DBIFunctionBlockType && m_Kind != DBIStructType)
    {
        throw (E_FAIL);
    };

    return (m_strSource);
}


CString CTypeDBI::GetTypeName ()
{
    // Do not try to load. Type name is set in Init and not Load.
    //
    //HRESULT hr;
    //hr = Load ();
    //if (FAILED (hr))
    //{
    //    throw hr;
    //};

    return (m_strTypeName);
}


CString CTypeDBI::GetNamespace ()
{
    // Do not try to load. Namespace is set in Init and not Load.
    //
    //HRESULT hr;
    //hr = Load ();
    //if (FAILED (hr))
    //{
    //    throw hr;
    //};

    return (m_strNamespace);
}


CString CTypeDBI::GetDBIFile ()
{
    // Do not try to load. DBI file name is set in Init and not Load.
    //
    //HRESULT hr;
    //hr = Load ();
    //if (FAILED (hr))
    //{
    //    throw hr;
    //};

    return (m_strDBIFile);
}


unsigned long CTypeDBI::GetVariableCount ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_Variables.GetCount ());
}


CVariableDBI *CTypeDBI::LookupVariableByIndex (unsigned long ulIndex)
{
    HRESULT hr;
    CVariableDBI *pVariableDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (ulIndex >= m_Variables.GetCount ())
    {
        throw E_INVALIDARG;
    };

    pVariableDBI = m_Variables.GetVariable (ulIndex);
    ASSERT (pVariableDBI != NULL);
    if (pVariableDBI == NULL)
    {
        throw E_UNEXPECTED;
    };

    return (pVariableDBI);
}


CVariableDBI *CTypeDBI::LookupVariableByName (const CString &strName)
{
    HRESULT hr;
    CVariableDBI *pVariableDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pVariableDBI = m_Variables.LookupByName (strName);
    return (pVariableDBI);
}


CBreakpointDBI *CTypeDBI::LookupBreakpointByNumber (long lNumber)
{
    HRESULT hr;
    CBreakpointDBI *pBreakpointDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pBreakpointDBI = m_Breakpoints.LookupByNumber (lNumber);
    return (pBreakpointDBI);
}


CBreakpointDBI *CTypeDBI::LookupBreakpointByPosition(
    EDP_POS_QUALIFIER qualifier, 
    long lX, long lY, long lCX, long lCY
)
{
    HRESULT hr;
    CBreakpointDBI *pBreakpointDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pBreakpointDBI = m_Breakpoints.LookupByPosition (qualifier, lX, lY, lCX, lCY);
    return (pBreakpointDBI);
}


unsigned long CTypeDBI::GetInstanceCount (IUnknown *pType)
{
    HRESULT hr;
    InstanceList *pInstanceList;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (pType == NULL)
    {
        unsigned long ulCount=0;
        unsigned long ulIndex;

        for (ulIndex = 0; ulIndex < (unsigned long)m_Instances.GetSize (); ++ulIndex)
        {
            pInstanceList = m_Instances[ulIndex];

            ASSERT (pInstanceList != NULL);
            if (pInstanceList == NULL)
            {
                throw E_UNEXPECTED;
            };

            ulCount += pInstanceList->m_Instances.GetSize ();
        };

        return (ulCount);
    };

    pInstanceList = LookupInstanceList (pType);
    if (pInstanceList == NULL)
    {
        return (0);
    };

    return (pInstanceList->m_Instances.GetSize ());
}


CInstanceDBI *CTypeDBI::LookupInstanceByIndex (IUnknown *pType, unsigned long ulIndex)
{
    HRESULT hr;
    InstanceList *pInstanceList;
    CInstanceDBI *pInstanceDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (pType == NULL)
    {
        int iListIndex;

        for (iListIndex = 0; iListIndex < m_Instances.GetSize (); ++iListIndex)
        {
            pInstanceList = m_Instances[iListIndex];
            ASSERT (pInstanceList != NULL);
            if (pInstanceList == NULL)
            {
                throw E_UNEXPECTED;
            };

            if (ulIndex < (unsigned long)pInstanceList->m_Instances.GetSize ())
                break;

            ulIndex -= pInstanceList->m_Instances.GetSize ();
        };

        if (iListIndex == m_Instances.GetSize ())
        {
            throw E_INVALIDARG;
        };
    }
    else
    {
        pInstanceList = LookupInstanceList (pType);
        if (pInstanceList == NULL)
        {
            throw E_INVALIDARG;
        };

        if (ulIndex >= (unsigned long)pInstanceList->m_Instances.GetSize ())
        {
            throw E_INVALIDARG;
        };
    };

    pInstanceDBI = pInstanceList->m_Instances[ulIndex];
    ASSERT (pInstanceDBI != NULL);
    if (pInstanceDBI == NULL)
    {
        throw E_UNEXPECTED;
    };

    return (pInstanceDBI);
}


CInstanceDBI *CTypeDBI::LookupInstanceByPath (const CString &strPath)
{
    HRESULT hr;
    int iListIndex;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    ASSERT (m_Kind == DBIResourceType);
    if (m_Kind != DBIResourceType)
    {
        throw (E_FAIL);
    };

    for (iListIndex = 0; iListIndex < m_Instances.GetSize (); ++iListIndex)
    {
        InstanceList *pInstanceList;
        int iIndex;

        pInstanceList = m_Instances[iListIndex];
        ASSERT (pInstanceList != NULL);
        if (pInstanceList == NULL)
        {
            throw (E_UNEXPECTED);
        };

        for (iIndex = 0; iIndex < pInstanceList->m_Instances.GetSize (); ++iIndex)
        {
            CInstanceDBI *pInstanceDBI;

            pInstanceDBI = pInstanceList->m_Instances[iIndex];
            ASSERT (pInstanceDBI != NULL);
            if (pInstanceDBI == NULL)
            {
                throw (E_UNEXPECTED);
            };

            if (strPath.CompareNoCase (pInstanceDBI->GetPath ()) == 0)
            {
                return (pInstanceDBI);
            };
        };
    };

    return (NULL);
}


CString CTypeDBI::GetText ()
{
    HRESULT hr;
    CString strText;
    CString strElementText;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (!m_strTypeName.IsEmpty ())
    {
        return (m_strTypeName);
    };

    switch (m_Kind)
    {
    case DBIAliasType:
        ASSERT (false);
        strText = "<unknown alias type>";
        break;

    case DBISimpleType:
        strText = BL_DatatypeToTypename (m_IECType, 0);
        break;

    case DBIArrayType:
        if (m_pElementType != NULL)
        {
            strElementText = m_pElementType->GetText ();
        }
        else
        {
            strElementText = "<unknown>";
        };

        strText.Format ("ARRAY [%ld..%ld] OF %s", m_lLowerBound, m_lUpperBound, strElementText);
        break;

    case DBIStructType:
        strText = "STRUCT";
        break;

    case DBIProgramType:
        strText = "PROGRAM";
        break;

    case DBIFunctionBlockType:
        strText = "FUNCTION_BLOCK";
        break;

    case DBIResourceType:
        strText = "RESOURCE";
        break;

    case DBIConfigurationType:
        strText = "CONFIGURATION";
        break;

    case DBIStepType:
        strText = "STEP";
        break;

    case DBIActionType:
        strText = "ACTION";
        break;

    case DBITransitionType:
        strText = "TRANSITION";
        break;
    };

    return (strText);
}


unsigned long CTypeDBI::GetStepCount ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_Steps.GetCount ());
}


CStepDBI *CTypeDBI::LookupStepByIndex (unsigned long ulIndex)
{
    HRESULT hr;
    CStepDBI *pStepDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (ulIndex >= m_Steps.GetCount ())
    {
        throw E_INVALIDARG;
    };

    pStepDBI = m_Steps.GetStep (ulIndex);
    ASSERT (pStepDBI != NULL);
    if (pStepDBI == NULL)
    {
        throw E_UNEXPECTED;
    };

    return (pStepDBI);
}


CStepDBI *CTypeDBI::LookupStepByName (const CString &strName)
{
    HRESULT hr;
    CStepDBI *pStepDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pStepDBI = m_Steps.LookupByName (strName);
    return (pStepDBI);
}


CStepDBI *CTypeDBI::LookupStepByNumber (long lNumber)
{
    HRESULT hr;
    CStepDBI *pStepDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pStepDBI = m_Steps.LookupByNumber (lNumber);
    return (pStepDBI);
}


CStepDBI *CTypeDBI::LookupStepByPosition (long lX, long lY)
{
    HRESULT hr;
    CStepDBI *pStepDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pStepDBI = m_Steps.LookupByPosition (lX, lY);
    return (pStepDBI);
}


unsigned long CTypeDBI::GetActionCount ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_Actions.GetCount ());
}


CActionDBI *CTypeDBI::LookupActionByIndex (unsigned long ulIndex)
{
    HRESULT hr;
    CActionDBI *pActionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (ulIndex >= m_Actions.GetCount ())
    {
        throw E_INVALIDARG;
    };

    pActionDBI = m_Actions.GetAction (ulIndex);
    ASSERT (pActionDBI != NULL);
    if (pActionDBI == NULL)
    {
        throw E_UNEXPECTED;
    };

    return (pActionDBI);
}


CActionDBI *CTypeDBI::LookupActionByName (const CString &strName)
{
    HRESULT hr;
    CActionDBI *pActionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pActionDBI = m_Actions.LookupByName (strName);
    return (pActionDBI);
}


CActionDBI *CTypeDBI::LookupActionByNumber (long lNumber)
{
    HRESULT hr;
    CActionDBI *pActionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pActionDBI = m_Actions.LookupByNumber (lNumber);
    return (pActionDBI);
}


CActionDBI *CTypeDBI::LookupActionByPosition (long lX, long lY)
{
    HRESULT hr;
    CActionDBI *pActionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pActionDBI = m_Actions.LookupByPosition (lX, lY);
    return (pActionDBI);
}


unsigned long CTypeDBI::GetTransitionCount ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_Transitions.GetCount ());
}


CTransitionDBI *CTypeDBI::LookupTransitionByIndex (unsigned long ulIndex)
{
    HRESULT hr;
    CTransitionDBI *pTransitionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (ulIndex >= m_Transitions.GetCount ())
    {
        throw E_INVALIDARG;
    };

    pTransitionDBI = m_Transitions.GetTransition (ulIndex);
    ASSERT (pTransitionDBI != NULL);
    if (pTransitionDBI == NULL)
    {
        throw E_UNEXPECTED;
    };

    return (pTransitionDBI);
}


CTransitionDBI *CTypeDBI::LookupTransitionByName (const CString &strName)
{
    HRESULT hr;
    CTransitionDBI *pTransitionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pTransitionDBI = m_Transitions.LookupByName (strName);
    return (pTransitionDBI);
}


CTransitionDBI *CTypeDBI::LookupTransitionByNumber (long lNumber)
{
    HRESULT hr;
    CTransitionDBI *pTransitionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pTransitionDBI = m_Transitions.LookupByNumber (lNumber);
    return (pTransitionDBI);
}


CTransitionDBI *CTypeDBI::LookupTransitionByPosition (long lX, long lY)
{
    HRESULT hr;
    CTransitionDBI *pTransitionDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pTransitionDBI = m_Transitions.LookupByPosition (lX, lY);
    return (pTransitionDBI);
}

CGvlFileList* CTypeDBI::GetGvlFileList()
{
    return &m_Gvls;
}


DBILanguage CTypeDBI::GetLanguage ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_Language);
}


HRESULT CTypeDBI::Load ()
{
    CString strDBIFile;
    CFile file;
    HRESULT hr;
    CFileStatus fileStatus;

    if (m_bLoaded)
    {
        return (S_OK);
    };

    // Currently loading? Recursive call!!!
    if (m_bLoading)
    {
        return (E_FAIL);
    };
    m_bLoading = true;

    if (m_Kind != DBIAliasType)
    {
        m_bLoading = false;
        return (S_FALSE);
    };

    if (m_strDBIFile.IsEmpty ())
    {
        m_bLoading = false;
        return (S_FALSE);
    };

    ASSERT (m_pContext != NULL);
    if (m_pContext == NULL)
    {
        m_bLoading = false;
        return (E_UNEXPECTED);
    };

    strDBIFile.Format ("%s\\%s", (LPCTSTR)m_pContext->GetDebugInfoPath (), (LPCTSTR)m_strDBIFile);

    if (!file.Open (strDBIFile, CFile::modeRead | CFile::shareDenyWrite))
    {
        m_bLoading = false;
        return (S_FALSE);
    };

    try
    {
        CXMLArchive ar (&file, CArchive::load);

        try
        {
            ar.Load ();
        }
        catch (HRESULT hr)
        {
            try
            {
                ar.Close ();
            }
            catch (HRESULT)
            {
            };

            m_bLoading = false;
            return (hr);
        };

        try
        {
            hr = LoadPOUs (ar);
            if (FAILED (hr))
            {
                throw hr;
            };
        }
        catch (HRESULT hr)
        {
            try
            {
                ar.MoveUp ();
            }
            catch (HRESULT)
            {
            };

            try
            {
                ar.Close ();
            }
            catch (HRESULT)
            {
            };

            m_bLoading = false;
            return (hr);
        };

        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT hr)
        {
            try
            {
                ar.Close ();
            }
            catch (HRESULT)
            {
            };

            return (hr);
        };

        ar.Close ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    m_bLoaded = true;
    m_bLoading = false;

    if (!CFile::GetStatus (strDBIFile, fileStatus))
    {
        fileStatus.m_mtime = 0;
    }
    else
    {
        m_Timestamp = fileStatus.m_mtime;
    };

    return (S_OK);
}


void CTypeDBI::Unload ()
{
    m_Variables.Unload (this);
    m_Breakpoints.Unload (this);

    UnloadInstances ();

    m_Kind = DBIAliasType;

    m_bLoaded = false;
}


HRESULT CTypeDBI::LoadPOUs (CXMLArchive &ar)
{
    HRESULT hr;
    bool bMovedDown=false;

    try
    {
        if (ar.MoveDown (DBI_XML_PROGRAM))
        {
            bMovedDown = true;

            hr = LoadAttributes (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadVariables (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadBreakpoints (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadSFC (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            m_Kind = DBIProgramType;
        }
        else if (ar.MoveDown (DBI_XML_FUNCTIONBLOCK))
        {
            bMovedDown = true;

            hr = LoadAttributes (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadVariables (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadBreakpoints (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadSFC (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            m_Kind = DBIFunctionBlockType;
        }
        else if (ar.MoveDown (DBI_XML_STRUCT))
        {
            bMovedDown = true;

            hr = LoadAttributes (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadVariables (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            m_Kind = DBIStructType;
        }
        else if (ar.MoveDown (DBI_XML_RESOURCE))
        {
            bMovedDown = true;

            hr = LoadResourceAttributes (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadGvls (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadCustomGlobals (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            hr = LoadVariables (ar);
            if (FAILED (hr))
            {
                return (hr);
            };

            if (ar.MoveDown (DBI_XML_INSTANCES))
            {
                hr = LoadInstances (ar);
                if (FAILED (hr))
                {
                    try
                    {
                        ar.MoveUp ();
                    }
                    catch (HRESULT)
                    {
                        // reverting inner MoveDown failed, no sense 
                        // in trying to revert outer MoveDown
                        bMovedDown = false;
                    };

                    throw hr;
                };

                try
                {
                    ar.MoveUp ();
                }
                catch (HRESULT hr)
                {
                    // reverting inner MoveDown failed, no sense 
                    // in trying to revert outer MoveDown
                    bMovedDown = false;
                    throw hr;
                };
            };

            m_Kind = DBIResourceType;
        }
        else
        {
            Corrupt ();
            throw E_UNEXPECTED;
        };
    }
    catch (HRESULT hr)
    {
        if (bMovedDown)
        {
            try
            {
                ar.MoveUp ();
            }
            catch (HRESULT)
            {
            };
        };

        return (hr);
    };

    return (S_OK);
}


HRESULT CTypeDBI::LoadAttributes (CXMLArchive &ar)
{
    CString strName;
    CString strSource;

    try
    {
        if (!ar.GetAttribute (DBI_XML_ID, strName))
        {
            Corrupt ();
            throw E_FAIL;
        };
        if (strName.CompareNoCase (m_strTypeName) != 0)
        {
            Corrupt ();
            throw E_UNEXPECTED;
        };

        if (!ar.GetAttribute (DBI_XML_SOURCE, strSource))
        {
            Corrupt ();
            throw E_FAIL;
        };
        m_strSource = strSource;


        // Debug info file should contain some language tag.
        // Currently it doesn't so we retrieve language from
        // source file extension.
        //TODO !!!!!!!!!!!!
        if (!lstrcmpi(FC_StringGetFileExt(m_strSource), _T("4gr")))
        {
            m_Language = DBIFBD;
        }
        else
        {
            m_Language = DBIST;
        };

    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CTypeDBI::LoadResourceAttributes (CXMLArchive &ar)
{
    CString strName;
    CString strResourceType;

    try
    {
        if (!ar.GetAttribute (DBI_XML_ID, strName))
        {
            Corrupt ();
            throw E_FAIL;
        };
        if (strName.CompareNoCase (m_strTypeName) != 0)
        {
            Corrupt ();
            throw E_UNEXPECTED;
        };

        if (!ar.GetAttribute (DBI_XML_TYPE, strResourceType))
        {
            Corrupt ();
            throw E_FAIL;
        };
        m_strResourceType = strResourceType;
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}



HRESULT CTypeDBI::LoadGvls (CXMLArchive &ar)
{
    HRESULT hr;

    ASSERT (m_pContext != NULL);
    if (m_pContext == NULL)
    {
        return (E_UNEXPECTED);
    };

    try
    {
        if (!ar.MoveDown (DBI_XML_GVLS))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };


    hr = m_Gvls.Load (m_pContext, ar, this);
    if (FAILED (hr))
    {
        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT)
        {
        };

        return (hr);
    };

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}



HRESULT CTypeDBI::LoadCustomGlobals (CXMLArchive &ar)
{
    ASSERT (m_pContext != NULL);
    if (m_pContext == NULL)
    {
        return (E_UNEXPECTED);
    };

    try
    {
        if (!ar.MoveDown (DBI_XML_CUSTOMGLOBAL))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    CString strClsid;
    CString strConfFile;
    CString strTrgType;
    ar.GetAttribute (_T("TARGETTYPE"), strTrgType);
    ar.GetAttribute (_T("CUSTOM_DECLARATION"), strClsid);
    ar.GetAttribute (_T("CUSTOM_DECL_CONFIG"), strConfFile);

    if(S_OK != UTIL_IsAbsolutePath(strConfFile))
    {
        CString tmp;
        TCHAR   szPath[256];
        size_t  nchars = 256;  

        UTIL_GetInstallPath(szPath, &nchars);
        tmp = szPath;
        tmp += "Engineering\\bin\\";
        tmp += strConfFile;
        strConfFile = tmp;
    }

    CLSID clsid;
    BSTR  sClsid = strClsid.AllocSysString();
    
    CLSIDFromString(sClsid, &clsid);
    SysFreeString(sClsid);

    CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_ICG_AutoDecl, (void**)&m_pIAutoDecl);
    if(m_pIAutoDecl)
    {
        BSTR sTrgType = strTrgType.AllocSysString();
        BSTR sConfFile = strConfFile.AllocSysString();

        m_pIAutoDecl->init(sTrgType, sConfFile);  

        SysFreeString(sTrgType);
        SysFreeString(sConfFile);
    }

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT)
    {
    };

    return (S_OK);
}


HRESULT CTypeDBI::LoadVariables (CXMLArchive &ar)
{
    HRESULT hr;

    ASSERT (m_pContext != NULL);
    if (m_pContext == NULL)
    {
        return (E_UNEXPECTED);
    };

    try
    {
        if (!ar.MoveDown (DBI_XML_VARIABLES))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };


    hr = m_Variables.Load (m_pContext, ar, this);
    if (FAILED (hr))
    {
        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT)
        {
        };

        return (hr);
    };

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CTypeDBI::LoadBreakpoints (CXMLArchive &ar)
{
    HRESULT hr;

    ASSERT (m_pContext != NULL);
    if (m_pContext == NULL)
    {
        return (E_UNEXPECTED);
    };

    try
    {
        if (!ar.MoveDown (DBI_XML_BREAKPOINTS))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    hr = m_Breakpoints.Load (m_pContext, ar, this);
    if (FAILED (hr))
    {
        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT)
        {
        };

        return (hr);
    };

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CTypeDBI::LoadSFC (CXMLArchive &ar)
{
    HRESULT hr;
    HRESULT hrSteps;
    HRESULT hrActions;
    HRESULT hrTransitions;

    ASSERT (m_pContext != NULL);
    if (m_pContext == NULL)
    {
        return (E_UNEXPECTED);
    };

    try
    {
        if (!ar.MoveDown (DBI_XML_SFC))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    hr = S_OK;
    hrSteps = m_Steps.Load (m_pContext, ar, this);
    hrActions = m_Actions.Load (m_pContext, ar, this);
    hrTransitions = m_Transitions.Load (m_pContext, ar, this);
    if (FAILED (hrSteps))
    {
        hr = hrSteps;
    }
    else if (FAILED (hrActions))
    {
        hr = hrActions;
    }
    else if (FAILED (hrTransitions))
    {
        hr = hrTransitions;
    };
    if (FAILED (hr))
    {
        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT)
        {
        };

        return (hr);
    };

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


CTypeDBI::InstanceList *CTypeDBI::LookupInstanceList (IUnknown *pType)
{
    HRESULT hr;
    IUnknown *pUnknown;
    int iIndex;

    hr = pType->QueryInterface (&pUnknown);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };

    for (iIndex = 0; iIndex < m_Instances.GetSize (); ++iIndex)
    {
        InstanceList *pInstanceList;

        pInstanceList = m_Instances[iIndex];
        ASSERT (pInstanceList != NULL);
        if (pInstanceList == NULL)
            continue;

        ASSERT (pInstanceList->m_pType != NULL);
        if (pInstanceList->m_pType == NULL)
            continue;

        if (pUnknown == pInstanceList->m_pType->GetUnknown ())
        {
            return (pInstanceList);
        };
    };

    return (NULL);
}


CTypeDBI::InstanceList *CTypeDBI::LookupInstanceList(const CString& crstrTypeName)
{
    int iIndex;
    int iNumInstances = m_Instances.GetSize();

    for (iIndex = 0; iIndex < iNumInstances; ++iIndex)
    {
        InstanceList *pInstanceList;

        pInstanceList = m_Instances[iIndex];
        ASSERT (pInstanceList != NULL);
        if (pInstanceList == NULL)
            continue;

        ASSERT (pInstanceList->m_pType != NULL);
        if (pInstanceList->m_pType == NULL)
            continue;

        if(crstrTypeName.Compare(pInstanceList->m_pType->GetTypeName()) == 0)
        {
            return (pInstanceList);
        }
    }

    return (NULL);
}


HRESULT CTypeDBI::LoadInstances (CXMLArchive &ar)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_INSTLIST))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    try
    {
        do
        {
            HRESULT hr;
            CString strType;
            CString strDBIFile;
            CTypeDBI *pType;

            try
            {
                if (!ar.GetAttribute (DBI_XML_TYPE, strType))
                {
                    Corrupt ();
                    throw E_FAIL;
                };

                (void)ar.GetAttribute (DBI_XML_DBI, strDBIFile);

                hr = m_pContext->BuildDataType (strType, strDBIFile, strDBIFile, pType);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pType != NULL);
                if (pType == NULL)
                {
                    throw E_UNEXPECTED;
                };

                hr = LoadInstList (pType, ar);
                if (FAILED (hr))
                {
                    pType->Release ();
                    throw hr;
                };

                pType->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading instance lists just
                // because one instance list is corrupt.

            };
        } while (ar.MoveNext ());
    }
    catch (HRESULT hr)
    {
        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT)
        {
        };

        return (hr);
    };

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CTypeDBI::LoadInstList (CTypeDBI *pType, CXMLArchive &ar)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_INST))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    try
    {
        do
        {
            try
            {
                HRESULT hr;
                CComObject<CInstanceDBI> *pInstanceDBI;

                hr = CComObject<CInstanceDBI>::CreateInstance (&pInstanceDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pInstanceDBI != NULL);
                if (pInstanceDBI == NULL)
                {
                    throw E_UNEXPECTED;
                };
                pInstanceDBI->AddRef ();

                hr = pInstanceDBI->Init (m_pContext, pType, ar);
                if (FAILED (hr))
                {
                    pInstanceDBI->Release ();
                    throw hr;
                };

                hr = AddInstance (pInstanceDBI);
                if (FAILED (hr))
                {
                    pInstanceDBI->Release ();
                    throw hr;
                };

                pInstanceDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading instances just
                // because one instance is corrupt.

            };
        } while (ar.MoveNext ());
    }
    catch (HRESULT hr)
    {
        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT)
        {
        };

        return (hr);
    };

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CTypeDBI::AddInstance (CInstanceDBI *pInstanceDBI)
{
    int iIndex;
    InstanceList *pInstanceList;

    for (iIndex = 0; iIndex < m_Instances.GetSize (); ++iIndex)
    {
        pInstanceList = m_Instances[iIndex];
        ASSERT (pInstanceList != NULL);
        if (pInstanceList == NULL)
        {
            return (E_UNEXPECTED);
        };

        if (pInstanceList->m_pType == pInstanceDBI->GetType ())
            break;
    };

    if (iIndex == m_Instances.GetSize ())
    {
        pInstanceList = new InstanceList (pInstanceDBI->GetType ());
        m_Instances.Add (pInstanceList);
    };

    pInstanceDBI->AddRef ();
    pInstanceDBI->AddParent (this);
    pInstanceList->m_Instances.Add (pInstanceDBI);

    return (S_OK);
}


void CTypeDBI::UnloadInstances ()
{
    int iListIndex;

    for (iListIndex = 0; iListIndex < m_Instances.GetSize (); ++iListIndex)
    {
        InstanceList *pInstanceList;
        int iIndex;

        pInstanceList = m_Instances[iListIndex];
        ASSERT (pInstanceList != NULL);
        if (pInstanceList == NULL)
            continue;

        for (iIndex = 0; iIndex < pInstanceList->m_Instances.GetSize (); ++iIndex)
        {
            CInstanceDBI *pInstanceDBI;

            pInstanceDBI = pInstanceList->m_Instances[iIndex];
            ASSERT (pInstanceDBI != NULL);
            if (pInstanceDBI == NULL)
                continue;

            pInstanceDBI->RemoveParent (this);
            pInstanceDBI->Release ();
        };

        delete pInstanceList;
    };

    m_Instances.RemoveAll ();
}


STDMETHODIMP CTypeDBI::get_Kind(DBITypeKind *pKind)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pKind != NULL);
    if (pKind == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pKind = GetKind ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_Source(BSTR *psSource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psSource != NULL);
    if (psSource == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psSource = GetSource ().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_TypeName(BSTR *psTypeName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psTypeName != NULL);
    if (psTypeName == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psTypeName = GetTypeName ().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_IECType(short *psIECType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psIECType != NULL);
    if (psIECType == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psIECType = GetIECType ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_ElementType(IUnknown **ppElementType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTypeDBI *pElementType;

    ASSERT (ppElementType != NULL);
    if (ppElementType == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        pElementType = GetElementType ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    ASSERT (pElementType != NULL);
    if (pElementType == NULL)
    {
        return (S_FALSE);
    };

    *ppElementType = pElementType->GetUnknown ();
    (*ppElementType)->AddRef ();

	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_LowerBound(long *plLowerBound)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plLowerBound != NULL);
    if (plLowerBound == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plLowerBound = GetLowerBound ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_UpperBound(long *plUpperBound)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plUpperBound != NULL);
    if (plUpperBound == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plUpperBound = GetUpperBound ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_VariableCount(unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pulCount != NULL);
    if (pulCount == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pulCount = GetVariableCount ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_VariableByIndex(unsigned long ulIndex, IUnknown **ppVariable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CVariableDBI *pVariableDBI;

    ASSERT (ppVariable != NULL);
    if (ppVariable == NULL)
    {
        return (E_POINTER);
    };
    *ppVariable = NULL;

    try
    {
        pVariableDBI = LookupVariableByIndex (ulIndex);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    *ppVariable = pVariableDBI->GetUnknown ();
    ASSERT (*ppVariable != NULL);
    if (*ppVariable == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppVariable)->AddRef ();
	return S_OK;
}


STDMETHODIMP CTypeDBI::get_VariableByName(BSTR sName, IUnknown **ppVariable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CVariableDBI *pVariableDBI;

    ASSERT (ppVariable != NULL);
    if (ppVariable == NULL)
    {
        return (E_POINTER);
    };
    *ppVariable = NULL;

    try
    {
        pVariableDBI = LookupVariableByName (sName);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if(pVariableDBI == NULL)
    {
        if(m_pIAutoDecl) //only !=NULL if m_King==BDIResource, used for auto globals
        {
	        BSTR sIECAddr = NULL;
	        BSTR sIECType = NULL;
            BOOL bReadOnly;

            HRESULT hr = m_pIAutoDecl->getAddressInfo(       
		                    sName,      //IEC identifier to test 
		                    &sIECAddr,  //Target specific direct address to be used.
		                    &sIECType,  //The IEC data type of the predefined variable
		                    &bReadOnly  //true if not writable.
		                 );

            if(hr==S_OK && sIECType && sIECAddr)
            {
                CString   strType;
                CTypeDBI* pType   = NULL;
                strType = sIECType;
                m_pContext->BuildDataType(strType, "<POUS>", "", pType);

                BSTR sOnlString = NULL;
                hr = m_pIAutoDecl->getOnlStringFromAddr(
		                   sIECAddr,     //IEC direct address
		                   sIECType,     //the data type
		                   &sOnlString   //__system.direct.W.2.2"
		        );
                
                SysFreeString(sIECType);
                SysFreeString(sIECAddr);

                ASSERT(hr==S_OK && pType && sOnlString);
                if(hr==S_OK && pType && sOnlString)
                {
                    CString strName(sName);
                    CString strOnlString(sOnlString);
                    CComObject<CVariableDBI> *pVariableDBIx;
                    CComObject<CVariableDBI>::CreateInstance (&pVariableDBIx);
                    
                    pVariableDBIx->Init(m_pContext, DBIVarGlobal, strName, pType, strOnlString);
                    
                    pType->Release();
                    pVariableDBI = pVariableDBIx;
                }

                if(sOnlString)
                    SysFreeString(sOnlString);
            }
            else
            {
                return (S_FALSE);
            }

        }
        else
        {
            return (S_FALSE);
        }
    };

    *ppVariable = pVariableDBI->GetUnknown ();
    ASSERT (*ppVariable != NULL);
    if (*ppVariable == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppVariable)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_BreakpointByNumber(long lNumber, IUnknown **ppBreakpoint)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CBreakpointDBI *pBreakpointDBI;

    if (ppBreakpoint != NULL)
    {
        *ppBreakpoint = NULL;
    };

    try
    {
        pBreakpointDBI = LookupBreakpointByNumber (lNumber);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pBreakpointDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppBreakpoint = pBreakpointDBI->GetUnknown ();
    ASSERT (*ppBreakpoint != NULL);
    if (*ppBreakpoint == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppBreakpoint)->AddRef ();
	return (S_OK);
}



STDMETHODIMP CTypeDBI::get_BreakpointByPositionEx(
    EDP_POS_QUALIFIER qualifier, long lX, long lY, long lCX, long lCY, IUnknown **ppBreakpoint)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CBreakpointDBI *pBreakpointDBI;

    if (ppBreakpoint != NULL)
    {
        *ppBreakpoint = NULL;
    };

    try
    {
        pBreakpointDBI = LookupBreakpointByPosition (qualifier, lX, lY, lCX, lCY);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pBreakpointDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppBreakpoint = pBreakpointDBI->GetUnknown ();
    ASSERT (*ppBreakpoint != NULL);
    if (*ppBreakpoint == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppBreakpoint)->AddRef ();
	return (S_OK);
}



STDMETHODIMP CTypeDBI::get_InstanceCount(IUnknown *pType, unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pulCount != NULL);
    if (pulCount == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pulCount = GetInstanceCount (pType);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_InstanceByIndex(IUnknown *pType, unsigned long ulIndex, IUnknown **ppInstance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CInstanceDBI *pInstanceDBI;

    ASSERT (ppInstance != NULL);
    if (ppInstance == NULL)
    {
        return (E_POINTER);
    };
    *ppInstance = NULL;

    try
    {
        pInstanceDBI = LookupInstanceByIndex (pType, ulIndex);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    *ppInstance = pInstanceDBI->GetUnknown ();
    ASSERT (*ppInstance != NULL);
    if (*ppInstance == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppInstance)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_InstanceByPath(BSTR sPath, IUnknown **ppInstance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CInstanceDBI *pInstanceDBI;

    ASSERT (ppInstance != NULL);
    if (ppInstance == NULL)
    {
        return (E_POINTER);
    };
    *ppInstance = NULL;

    try
    {
        pInstanceDBI = LookupInstanceByPath (sPath);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pInstanceDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppInstance = pInstanceDBI->GetUnknown ();
    ASSERT (*ppInstance != NULL);
    if (*ppInstance == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppInstance)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_Text (BSTR *psText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psText != NULL);
    if (psText == NULL)
    {
        return (E_POINTER);
    };

    *psText = GetText ().AllocSysString ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_StepCount (unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pulCount != NULL);
    if (pulCount == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pulCount = GetStepCount ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_StepByIndex(unsigned long ulIndex, IUnknown **ppStep)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CStepDBI *pStepDBI;

    ASSERT (ppStep != NULL);
    if (ppStep == NULL)
    {
        return (E_POINTER);
    };
    *ppStep = NULL;

    try
    {
        pStepDBI = LookupStepByIndex (ulIndex);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    *ppStep = pStepDBI->GetUnknown ();
    ASSERT (*ppStep != NULL);
    if (*ppStep == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppStep)->AddRef ();
	return S_OK;
}


STDMETHODIMP CTypeDBI::get_StepByName(BSTR sName, IUnknown **ppStep)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CStepDBI *pStepDBI;

    ASSERT (ppStep != NULL);
    if (ppStep == NULL)
    {
        return (E_POINTER);
    };
    *ppStep = NULL;

    try
    {
        pStepDBI = LookupStepByName (sName);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pStepDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppStep = pStepDBI->GetUnknown ();
    ASSERT (*ppStep != NULL);
    if (*ppStep == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppStep)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_StepByNumber(long lNumber, IUnknown **ppStep)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CStepDBI *pStepDBI;

    if (ppStep != NULL)
    {
        *ppStep = NULL;
    };

    try
    {
        pStepDBI = LookupStepByNumber (lNumber);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pStepDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppStep = pStepDBI->GetUnknown ();
    ASSERT (*ppStep != NULL);
    if (*ppStep == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppStep)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_StepByPosition(long lX, long lY, IUnknown **ppStep)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CStepDBI *pStepDBI;

    if (ppStep != NULL)
    {
        *ppStep = NULL;
    };

    try
    {
        pStepDBI = LookupStepByPosition (lX, lY);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pStepDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppStep = pStepDBI->GetUnknown ();
    ASSERT (*ppStep != NULL);
    if (*ppStep == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppStep)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_ActionCount (unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pulCount != NULL);
    if (pulCount == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pulCount = GetActionCount ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_ActionByIndex(unsigned long ulIndex, IUnknown **ppAction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CActionDBI *pActionDBI;

    ASSERT (ppAction != NULL);
    if (ppAction == NULL)
    {
        return (E_POINTER);
    };
    *ppAction = NULL;

    try
    {
        pActionDBI = LookupActionByIndex (ulIndex);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    *ppAction = pActionDBI->GetUnknown ();
    ASSERT (*ppAction != NULL);
    if (*ppAction == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppAction)->AddRef ();
	return S_OK;
}


STDMETHODIMP CTypeDBI::get_ActionByName(BSTR sName, IUnknown **ppAction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CActionDBI *pActionDBI;

    ASSERT (ppAction != NULL);
    if (ppAction == NULL)
    {
        return (E_POINTER);
    };
    *ppAction = NULL;

    try
    {
        pActionDBI = LookupActionByName (sName);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pActionDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppAction = pActionDBI->GetUnknown ();
    ASSERT (*ppAction != NULL);
    if (*ppAction == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppAction)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_ActionByNumber(long lNumber, IUnknown **ppAction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CActionDBI *pActionDBI;

    if (ppAction != NULL)
    {
        *ppAction = NULL;
    };

    try
    {
        pActionDBI = LookupActionByNumber (lNumber);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pActionDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppAction = pActionDBI->GetUnknown ();
    ASSERT (*ppAction != NULL);
    if (*ppAction == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppAction)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_ActionByPosition(long lX, long lY, IUnknown **ppAction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CActionDBI *pActionDBI;

    if (ppAction != NULL)
    {
        *ppAction = NULL;
    };

    try
    {
        pActionDBI = LookupActionByPosition (lX, lY);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pActionDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppAction = pActionDBI->GetUnknown ();
    ASSERT (*ppAction != NULL);
    if (*ppAction == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppAction)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_TransitionCount (unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pulCount != NULL);
    if (pulCount == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pulCount = GetTransitionCount ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CTypeDBI::get_TransitionByIndex(unsigned long ulIndex, IUnknown **ppTransition)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTransitionDBI *pTransitionDBI;

    ASSERT (ppTransition != NULL);
    if (ppTransition == NULL)
    {
        return (E_POINTER);
    };
    *ppTransition = NULL;

    try
    {
        pTransitionDBI = LookupTransitionByIndex (ulIndex);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    *ppTransition = pTransitionDBI->GetUnknown ();
    ASSERT (*ppTransition != NULL);
    if (*ppTransition == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppTransition)->AddRef ();
	return S_OK;
}


STDMETHODIMP CTypeDBI::get_TransitionByName(BSTR sName, IUnknown **ppTransition)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTransitionDBI *pTransitionDBI;

    ASSERT (ppTransition != NULL);
    if (ppTransition == NULL)
    {
        return (E_POINTER);
    };
    *ppTransition = NULL;

    try
    {
        pTransitionDBI = LookupTransitionByName (sName);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pTransitionDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppTransition = pTransitionDBI->GetUnknown ();
    ASSERT (*ppTransition != NULL);
    if (*ppTransition == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppTransition)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_TransitionByNumber(long lNumber, IUnknown **ppTransition)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTransitionDBI *pTransitionDBI;

    if (ppTransition != NULL)
    {
        *ppTransition = NULL;
    };

    try
    {
        pTransitionDBI = LookupTransitionByNumber (lNumber);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pTransitionDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppTransition = pTransitionDBI->GetUnknown ();
    ASSERT (*ppTransition != NULL);
    if (*ppTransition == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppTransition)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_TransitionByPosition(long lX, long lY, IUnknown **ppTransition)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTransitionDBI *pTransitionDBI;

    if (ppTransition != NULL)
    {
        *ppTransition = NULL;
    };

    try
    {
        pTransitionDBI = LookupTransitionByPosition (lX, lY);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pTransitionDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppTransition = pTransitionDBI->GetUnknown ();
    ASSERT (*ppTransition != NULL);
    if (*ppTransition == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppTransition)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CTypeDBI::get_Language(DBILanguage *pLanguage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pLanguage != NULL);
    if (pLanguage == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pLanguage = GetLanguage ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}
