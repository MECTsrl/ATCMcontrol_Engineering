//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CEProjMan.h"

#include "lcconfig.h"
#include "xml_wrapper.h"
#include "resource.h"
#include "celatederror.h"
#include "blocksock.h"
#include "blocksock.cpp"

#include "CEAddonHandler_i.c"
#include "CEIECOperation.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


SecObjMap CLCConfig::s_Objects;

static struct {
    LATED_CONN_TYPE type;
    LPCTSTR szDescr; 
}   ConnTypeTbl[] = {
	{UNKNOWN,_T("")},
	{P2P,LATED_CONNTYPE_P2P},
    {PUBLISH,LATED_CONNTYPE_PUBLISH},
    {SUBSCRIBE,LATED_CONNTYPE_SUBSCRIBE},
};

static struct {
    BL_IEC_TYP Type;
    size_t sizeBytes;
}   IECTypeTbl[] = {
    {BL_BOOL,1},
    {BL_BYTE,1},
    {BL_WORD,2},
    {BL_DWORD,4},
    {BL_LWORD,8},
    {BL_INT,2},
    {BL_DINT,4},
    {BL_LINT,8},
    {BL_REAL,4},
    {BL_LREAL,8},
    {BL_TIME,8},
    {BL_DT,4},
    {BL_DATE,4},
    {BL_TOD,4},
    {BL_STRING,255},
    {BL_WSTRING,255},
};

CTargetInfo::CTargetInfo()
: m_strName(), m_strType() , m_strResource(), 
  m_strAddress(), m_strIP()
{
    m_ulIP = 0UL;
    m_bValid = true;
}

IMPLEMENT_SERIAL(CLCConfig, CXMLObject, LATED_SERIAL_VERSION)

CLCConfig::CLCConfig() 
    : m_DataTypes() , m_Listeners(), m_strPath() 
    , m_IECTypes() , m_TargetInfo()
{
    AddRef();
    m_pPM = NULL;
}

CLCConfig::CLCConfig(LPCTSTR pszFilePath) 
    : m_DataTypes() , m_Listeners(), m_strPath(pszFilePath) 
    , m_IECTypes() , m_TargetInfo()
{
    AddRef();
    m_pPM = NULL;
}

CLCConfig::~CLCConfig()
{
    m_IECTypes.RemoveAll();
    m_LinkList.RemoveAll();
    m_DataTypes.ForgetAll();
    m_Listeners.RemoveAll();
    m_TargetInfo.RemoveAll();
    m_GenObjs.RemoveAll();
    m_NewGenObjs.RemoveAll();
    FC_RELEASE_PTR(m_pPM);
}

void CLCConfig::SerializeXML(CXMLArchive & ar)
{
    CLCDataType* pDatatype = NULL;
    CLCLink* pLink = NULL;
    CLCGenObj* pGenObj = NULL;
    CString str;
    CXMLObject* pObj;
    BOOL bRet;

    if(ar.IsStoring()) { //storing
        int i;
        //datatypes first
        CLCDataType** ppTypes = m_DataTypes.GetAssocList();
        if(!ppTypes) {
            AfxThrowMemoryException();
        }

        for(i= 0; ppTypes[i]; i++) {
            ar << ppTypes[i];
        }

        m_DataTypes.FreeAssocList(ppTypes);

        //links...
        CLCLink** ppLinks = m_LinkList.GetItemList();
        if(!ppLinks) {
            AfxThrowMemoryException();
        }

        for(i= 0; ppLinks[i]; i++) {
            ar << ppLinks[i];
        }
        
        m_LinkList.FreeItemList(ppLinks);

        // generated objects
        POSITION pos = m_GenObjs.GetHeadPosition();
        while(pos)
        {
            CLCGenObj genObj = m_GenObjs.GetNext(pos);
            ar << &genObj;
        }
    }
    else { //loading
        m_strPath = ar.m_strFileName;

        bRet = ar.IsElementExisting(LATED_ELEM_LC_DATATYPE);
        if(bRet) {
            for(bRet = ar.MoveDown( LATED_ELEM_LC_DATATYPE );
                bRet; bRet= ar.MoveNext()) {

                pObj = ar.ReadObject();
                pDatatype = dynamic_cast<CLCDataType*>(pObj);
                if(pDatatype) {
                    pDatatype->GetKey(str);
                    m_DataTypes.Assoc(str,pDatatype);
                    pDatatype = NULL;
                    str.Empty();
                }
            }
            ar.MoveUp();
        }

        bRet = ar.IsElementExisting(LATED_ELEM_LC_LINK);
        if(bRet) {
            for(bRet = ar.MoveDown( LATED_ELEM_LC_LINK );
                bRet; bRet= ar.MoveNext()) {

                pObj = ar.ReadObject();
                pLink = dynamic_cast<CLCLink*>(pObj);
                if(pLink) {
                    m_LinkList.Add(pLink);
                    pLink = NULL;
                }
            }
            ar.MoveUp();
        }

        m_GenObjs.RemoveAll();
        m_NewGenObjs.RemoveAll();
        bRet = ar.IsElementExisting(LATED_ELEM_LC_GEN);
        if (bRet)
        {
            for(bRet = ar.MoveDown( LATED_ELEM_LC_GEN );
                bRet; bRet= ar.MoveNext()) {

                pObj = ar.ReadObject();
                pGenObj = dynamic_cast<CLCGenObj*>(pObj);
                if(pGenObj) {
                    m_GenObjs.AddTail(*pGenObj);
                    delete(pGenObj);
                    pGenObj = NULL;
                }
            }
            ar.MoveUp();
        }

        ar.MoveUp();
    }
}

void CLCConfig::ResolveDatatypes()
{
    int iCount = m_LinkList.GetCount();

    for(int i= 0; i < iCount; i++) {
        CLCLink* pLink = m_LinkList[i];
        if(pLink) {
            pLink->ResolveDatatypes(this);
            pLink->Release();
        }
    }
}

unsigned short CLCConfig::AllocDataID()
{
    int i;
    unsigned short unID = 0;
    unsigned char* pArr = NULL;
    CLCDataType** ppTypes = NULL;
    int size = LC_LAST_DATA_ID-LC_FIRST_DATA_ID+1;
    
    try {
        pArr = new unsigned char[size];
        if(!pArr) {
            AfxThrowMemoryException();
        }

        memset(pArr,0,sizeof(unsigned char) * size);
        
        CLCDataType** ppTypes = m_DataTypes.GetAssocList();
        if(!ppTypes) {
            AfxThrowMemoryException();
        }
        
        for(i= 0; ppTypes[i]; i++) {
            unID = ppTypes[i]->GetDataID();

            if(unID > LC_LAST_DATA_ID || unID < LC_FIRST_DATA_ID) {
                ASSERT(!"Invalid data ID");
                continue;
            }

            pArr[unID-LC_FIRST_DATA_ID] = 1;
        }

        m_DataTypes.FreeAssocList(ppTypes);
        ppTypes = NULL;

        for(i = 0,unID = 0; i < size; i++) {
            if(pArr[i] == 0) {
                unID = LC_FIRST_DATA_ID + i;
                break;
            }
        }

    }
    catch(CMemoryException *e) {
        e->Delete();
    }
    catch(CException* e) {
        e->Delete();
    }

    if(ppTypes) {
        m_DataTypes.FreeAssocList(ppTypes);
        ppTypes = NULL;
    }

    if(pArr) {
        delete [] pArr;
        pArr = NULL;
    }

    return unID;
}

void CLCConfig::AddDatatype(CLCDataType *pDatatype)
{
    CString str;
    ULONG ulid;
    POSITION Pos;
    CLCConfigListener* pListener = NULL;

    if(!pDatatype) {
        ASSERT(pDatatype);
        return;
    }
    
    pDatatype->GetKey(str);
    m_DataTypes.Assoc(str,pDatatype);

    for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
        m_Listeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pListener->OnDataType(AddDataType,pDatatype);
            pDatatype->TraverseMembers(AddDataType,pListener);
        }
    }
}

void CLCConfig::DeleteDatatype(CLCDataType* pDatatype)
{
    CString str;
    ULONG ulid;
    POSITION Pos;
    CLCConfigListener* pListener = NULL;

    pDatatype->AddRef();
    pDatatype->GetKey(str);
    m_DataTypes.Forget(str);
    
    //now Notify listeners
    for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
        m_Listeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pDatatype->TraverseMembers(DelDataType,pListener);
            pListener->OnDataType(DelDataType,pDatatype);
        }
    }
  
    pDatatype->Release();
}

void CLCConfig::RenameDatatype(CString & strNewName, CLCDataType* pDatatype)
{
    CString str;

    pDatatype->AddRef(); //for the assoc
    pDatatype->GetKey(str);
    m_DataTypes.Forget(str);
    
    pDatatype->SetName(strNewName);
    pDatatype->GetKey(str);
    m_DataTypes.Assoc(str,pDatatype);
}

CLCDataType * CLCConfig::GetDatatype(CString & str)
{
    CString strKey = str;
    //key for lookup is always lowercase.
    strKey.MakeLower();
    return m_DataTypes.Get(strKey);
}



void CLCConfig::AddObject(CLCBase* p)
{
    ULONG ulKey = (ULONG) p;
    s_Objects.SetAt(ulKey,p);
}

void CLCConfig::RemoveObject(CLCBase*p)
{
    ULONG ulKey = (ULONG) p;
    s_Objects.RemoveKey(ulKey);
}

bool CLCConfig::IsObjectValid(CLCBase*p)
{
    CLCBase *pFound = NULL;
    ULONG ulKey = (ULONG) p;

    if(s_Objects.Lookup(ulKey,pFound)) {
        return true;
    }

    return false;
}

LATED_CONN_TYPE CLCConfig::ToConnType(LPCTSTR psz)
{
    CString str = psz;
    LATED_CONN_TYPE type = UNKNOWN;
    int nEntries  = FC_ARRAY_LEN(ConnTypeTbl);

    for(int i = 0;i<nEntries;i++) {
        if(!str.CompareNoCase(ConnTypeTbl[i].szDescr))
            return ConnTypeTbl[i].type;
    }

    return type;
}

LPCTSTR CLCConfig::FromConnType(LATED_CONN_TYPE type)
{
    int nEntries  = FC_ARRAY_LEN(ConnTypeTbl);

    for(int i = 0;i<nEntries;i++) {
        if(type == ConnTypeTbl[i].type)
            return ConnTypeTbl[i].szDescr;
    }

    //unkown is first
    return ConnTypeTbl[0].szDescr;
}

size_t CLCConfig::GetDataLenFromType(BL_IEC_TYP Type)
{
    int nEntries  = FC_ARRAY_LEN(IECTypeTbl);

    for(int i = 0;i<nEntries;i++) {
        if(Type == IECTypeTbl[i].Type)
            return IECTypeTbl[i].sizeBytes;
    }

    return 0;
}


bool CLCConfig::GetSubscript(CString & strType,CString &strBasicType,size_t & size)
{
    CString str;
    int iIndex;
    LPTSTR pszEnd;

    size = 0UL;
    strBasicType.Empty();

    iIndex = strType.Find(LATED_BRACKET_E);
    if(iIndex == -1) {
        return false;
    }

    iIndex = strType.Find(LATED_BRACKET_S);

    if(iIndex == -1) {
        return false;
    }

    //assure its a integer value.

    str = strType.Mid(iIndex+1);
    size = _tcstoul(str,&pszEnd,10);

    if(str == pszEnd) { //no conversion possible
        size = 0UL;
        return false;
    }

    strBasicType  = strType.Left(iIndex);
    strBasicType.TrimLeft();
    strBasicType.TrimRight();
    return true;
}


void CLCConfig::SetPM(ICEProjectManager* pM)
{
    m_pPM = pM;
}

//TODO DEPRECATED.
HRESULT CLCConfig::FillIECTypes()
{
    HRESULT hr = S_OK;
    SAFEARRAY* pArray = NULL;
    CComQIPtr<ICEProjInfo>pInfo;
    CComBSTR sTarget,s;
    DWORD dwModifier = 0L;

    m_IECTypes.RemoveAll();

    if(!m_pPM) {
        return E_NOINTERFACE;
    }

    if(m_pPM) {
        BL_IEC_TYP Type;
        CString str;
        long lLowerBound,l,lUpperBound;
        unsigned char arr[BL_LASTTYPE+1];
        unsigned char chMax = 0;

        pInfo = m_pPM;
        if(!pInfo) {
            return E_NOINTERFACE;
        }

        memset(arr,0,FC_ARRAY_LEN(arr));

        sTarget = LATED_TARGET_TYPE_PC;
        hr = pInfo->getSupportedIECDataTypesForTarget(sTarget,&pArray);
        if(hr == S_OK) {

            chMax++;
            
            ::SafeArrayGetLBound(pArray, 1, &lLowerBound);
            ::SafeArrayGetUBound(pArray, 1, &lUpperBound);
        
            for (l = lLowerBound; l <= lUpperBound; l++)
            {
                s.Empty();
                hr = SafeArrayGetElement(pArray, &l, &s);
                if(FAILED(hr)) {
                    break;
                }
                
                BL_TypenameToDatatype(s,&Type,&dwModifier);
                
                if(dwModifier == 0) {
                    arr[Type]++;
                }
            }

            if(pArray) {
                ::SafeArrayDestroy(pArray);
                pArray = NULL;
            }
        }
        
        sTarget = LATED_TARGET_TYPE_EC;
        hr = pInfo->getSupportedIECDataTypesForTarget(sTarget,&pArray);
        if(hr == S_OK) {
            chMax++;

            ::SafeArrayGetLBound(pArray, 1, &lLowerBound);
            ::SafeArrayGetUBound(pArray, 1, &lUpperBound);
        
            for (l = lLowerBound; l <= lUpperBound; l++)
            {
                s.Empty();
                hr = SafeArrayGetElement(pArray, &l, &s);
                if(FAILED(hr)) {
                    break;
                }

                BL_TypenameToDatatype(s,&Type,&dwModifier);

                if(dwModifier == 0) {
                    arr[Type]++;
                }
            }

            if(pArray) {
                ::SafeArrayDestroy(pArray);    
                pArray = NULL;
            }
        }

        if(chMax == 0) {
            //no target at all
            return E_FAIL;
        }

        dwModifier = 0L;

        for(l = 0; l <= BL_LASTTYPE; l++) {
            if(arr[l] == chMax) { 
                Type = (BL_IEC_TYP)l;
                s.Empty();
                s = BL_DatatypeToTypename(Type,dwModifier);
                str = s;
                m_IECTypes.SetAt(Type,str); 
            }
        }

    }

    return S_OK;
}

void CLCConfig::GetIECTypes()
{
    BL_IEC_TYP Type;
    CComBSTR s;
    CString str;
    DWORD dw = 0UL;

    m_IECTypes.RemoveAll();

    for(int i = 1; i <= BL_LASTTYPE; i++) {
        Type = (BL_IEC_TYP)i;
        s.Empty();
        s = BL_DatatypeToTypename(Type,dw);
        str = s;
        m_IECTypes.SetAt(Type,str); 
    }
}

void CLCConfig::AddConfigListener(CLCConfigListener *p)
{
    CLCConfigListener *pTemp = NULL;
    ULONG ulid = (ULONG) p;

    if(m_Listeners.Lookup(ulid,pTemp)) {
	    return;
    }

    m_Listeners.SetAt(ulid,p);
}

void CLCConfig::RemoveConfigListener(CLCConfigListener *p)
{
    CLCConfigListener *pTemp = NULL;
    ULONG ulid = (ULONG) p;

    if(!m_Listeners.Lookup(ulid,pTemp)) {
	    return;
    }

    m_Listeners.RemoveKey(ulid);
}

void CLCConfig::TraverseDatatypes(int id)
{
    try {
        int i;

        CLCDataType** ppTypes = m_DataTypes.GetAssocList();
        if(!ppTypes) {
            return;
        }
        
        for(i= 0; ppTypes[i]; i++) {
            TraverseDatatype(id,ppTypes[i]);
        }

        m_DataTypes.FreeAssocList(ppTypes);
    }
    catch(CMemoryException *e) {
        e->Delete();
    }
    catch(CException* e) {
        e->Delete();
    }
}

void CLCConfig::TraverseDatatype(int id,CLCDataType* pDataType)
{
    ULONG ulid;
    POSITION Pos;
    CLCConfigListener* pListener = NULL;
    
    for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
        m_Listeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pListener->OnDataType(id,pDataType);
            pDataType->TraverseMembers(id,pListener);
            pListener->OnEndDataType(id,pDataType);
        }
    }
}


void CLCConfig::TraverseLinks(int id)
{
    try {
        ULONG ulid;
        POSITION Pos;
        CLCConfigListener* pListener = NULL;
        CLCLink** ppLinks = NULL;

        ppLinks = m_LinkList.GetItemList();
        if(!ppLinks) {
            AfxThrowMemoryException();
        }

        for(int i= 0; ppLinks[i]; i++) {
            //source link
            for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
                m_Listeners.GetNextAssoc(Pos,ulid,pListener);
                if(pListener) {
                    pListener->OnLink(id,ppLinks[i]);
                    ppLinks[i]->TraverseDestLinks(id,pListener);
                    pListener->OnEndLink(id,ppLinks[i]);
                }
            }
        }

        m_LinkList.FreeItemList(ppLinks);
    }
    catch(CMemoryException *e) {
        e->Delete();
    }
    catch(CException* e) {
        e->Delete();
    }

}

void CLCConfig::TraverseIECTypes(int id)
{
    ULONG ulid;
    POSITION Pos;
    CString strType;
    BL_IEC_TYP Type;
    CLCConfigListener* pListener = NULL;

    for(POSITION posTyp = m_IECTypes.GetStartPosition(); posTyp;) {
        m_IECTypes.GetNextAssoc(posTyp,Type,strType);

        for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
            m_Listeners.GetNextAssoc(Pos,ulid,pListener);
            if(pListener) {
                pListener->OnIECType(id,Type,strType);
            }
        }
    }
}

HRESULT CLCConfig::GetTargetList()
{
    HRESULT hr = S_OK;
    SAFEARRAY* pArray = NULL;
    CComQIPtr<ICEProjInfo>pInfo;
    CComBSTR sTarget,s;
    CString str;
    long lLowerBound,l,lUpperBound;

    m_TargetInfo.RemoveAll();

    if(!m_pPM) {
        return E_NOINTERFACE;
    }

    pInfo = m_pPM;
    if(!pInfo) {
        return E_NOINTERFACE;
    }

    //get ID paths for all target instances.
    hr = pInfo->getAllTargetsAsIdPaths(&pArray);
    if(FAILED(hr) || pArray == NULL) {
        return hr;
    }

    if(!pArray) {
        //assume no targets
        return S_OK;
    }

    ::SafeArrayGetLBound(pArray, 1, &lLowerBound);
    ::SafeArrayGetUBound(pArray, 1, &lUpperBound);
        
    for (l = lLowerBound; l <= lUpperBound; l++)
    {
        CTargetInfo info;

        s.Empty();
        hr = SafeArrayGetElement(pArray, &l, &s);
        if(FAILED(hr)) {
            break;
        }
        
        str = s;
        int iIndex = str.ReverseFind(CE_XML_IDPATH_SEP_CHAR);
        if(iIndex == -1) {
            continue;
        }
        
        info.m_strName = str.Mid(iIndex+1);
        sTarget = info.m_strName; 
        
        s.Empty();
        hr = pInfo->getTypeOfTarget(sTarget,&s);
        if(FAILED(hr)) {
            continue;
        }

        info.m_strType = s;

        //TODO this should be a property of the target type
        //called CANLATCOMM=TRUE,FALSE
        //cause we're running out of time here...
        if(info.m_strType.CompareNoCase(LATED_TARGET_TYPE_PC) 
            && info.m_strType.CompareNoCase(LATED_TARGET_TYPE_EC)
            && info.m_strType.CompareNoCase(LATED_TARGET_TYPE_CMZ))
        {
            //no PC no WAGO and no CMZ ...
            continue;
        }

        s.Empty();
        hr = pInfo->getResourceNameForTarget(sTarget,&s);
        if(FAILED(hr)) {
            continue;
        }

        info.m_strResource = s;
        
        s.Empty();
        hr = pInfo->getAddressStringForTarget(sTarget,&s);
        if(FAILED(hr)) {
            continue;
        }

        info.m_strAddress = s;
        ConvertToIP(info.m_strAddress,info.m_strIP,info.m_ulIP);
        m_TargetInfo.SetAt(info.m_strName,info);
    }

    ::SafeArrayDestroy(pArray);

    return S_OK;
}

void CLCConfig::TraverseTargetInfo(int id)
{
    ULONG ulid;
    POSITION pos1,pos2;
    CString strTarget;
    CLCConfigListener* pListener = NULL;
    CTargetInfo info;

    for(pos1 = m_TargetInfo.GetStartPosition(); pos1 != 0;) {
        m_TargetInfo.GetNextAssoc(pos1,strTarget,info);

        for(pos2 = m_Listeners.GetStartPosition();pos2 != NULL;) {
            m_Listeners.GetNextAssoc(pos2,ulid,pListener);
            if(pListener) {
                pListener->OnTarget(id,info);
            }
        }
    }
}

//------------------------------------------------------------------*
/**
 * Returns number of source link objects.
 * Should be used to iterate through the source link objects
 * via GetLink()
 * @return	int - number of associated link objects.
 * @see		GetLink(int)	
*/
int CLCConfig::GetLinkCount()
{
    return m_LinkList.GetCount();
}

//------------------------------------------------------------------*
/**
 * Returns reference to source link object at index of argument
 * iIndex if such an object exists within the list of source 
 * link objects or a NULL reference if no source link object
 * exists.
 * Call Release() with the returned reference if you give up the
 * reference.
 * @param		int iIndex - index of within list of 
 *              source link objects.
 * @return		CLCLink* - reference of source link
 *              object or NULL if no reference exists.
 * @see			GetLinkCount()
*/
CLCLink* CLCConfig::GetLink(int iIndex)
{
    return m_LinkList[iIndex];
}

void CLCConfig::AddLink(CLCLink* pLink)
{
    ULONG ulid;
    POSITION Pos;
    CLCConfigListener* pListener = NULL;

    if(!pLink) {
        ASSERT(pLink);
        return;
    }

    m_LinkList.Add(pLink);

    for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
        m_Listeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pListener->OnLink(Add_Link,pLink);
            pLink->TraverseDestLinks(Add_Link,pListener);
            pListener->OnEndLink(Add_Link,pLink);
        }
    }
}


void CLCConfig::AddDestLink(CLCLink* pLink, CLCDestLink* pDest)
{
    ULONG ulid;
    POSITION Pos;
    CLCConfigListener* pListener = NULL;

    ASSERT(pLink);
    ASSERT(pDest);

    if(!pLink || !pDest) {
        return;
    }
    
    pLink->AddDestLink(pDest);

    for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
        m_Listeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pListener->OnDestLink(Add_Link,pLink,pDest);
        }
    }
}

void CLCConfig::DeleteLink(CLCLink* pLink)
{
    ULONG ulid;
    POSITION Pos;
    CLCConfigListener* pListener = NULL;
    CLCLink* pDel = NULL;

    if(!pLink) {
        ASSERT(pLink);
        return;
    }

    pDel = m_LinkList.GetItem(pLink);
    if(!pDel) {
        return;
    }

    for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
        m_Listeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pDel->TraverseDestLinks(Del_Link,pListener);
            pListener->OnLink(Del_Link,pDel);
        }
    }

    m_LinkList.Remove(pDel);
    pDel->Release();
}

void CLCConfig::DeleteDestLink(CLCLink* pLink, CLCDestLink* pDest)
{
    ULONG ulid;
    POSITION Pos;
    CLCConfigListener* pListener = NULL;
    CLCLink* pL = NULL;

    ASSERT(pLink);
    ASSERT(pDest);

    pL = m_LinkList.GetItem(pLink);
    if(!pL) {
        return;
    }

    for(Pos = m_Listeners.GetStartPosition();Pos != NULL;) {
        m_Listeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pListener->OnDestLink(Del_Link,pL,pDest);
        }
    }
    
    pL->DeleteDestLink(pDest);
    pL->Release();
}

void CLCConfig::ActualizeLinks()
{
    HRESULT hr = S_OK;
    CLCLink** ppLinks = NULL;
    CLCLink* pLink = NULL;
    CLCDestLink *pDest = NULL;
    CTargetInfo info;
    CString strTarget;

    try {
        hr = GetTargetList();
        if(FAILED(hr)) {
            throw hr;
        }

        if(!m_TargetInfo.GetCount()) {
            return;
        }


        ppLinks = m_LinkList.GetItemList();
        if(!ppLinks) {
            AfxThrowMemoryException();
        }

        for(int i= 0; ppLinks[i]; i++) {
            pLink = ppLinks[i];
            strTarget = pLink->GetTargetName();
            
            //lookup in target info map
            if(m_TargetInfo.Lookup(strTarget,info)) {
                //hit - actualize it with information
                *pLink = info;
                pLink->SetValid(true);
            }
            else {
                pLink->SetValid(false);
            }

            //now iterate through all dest links
            for(int j = 0; j< pLink->GetDestLinkCount(); j++) {
                pDest = pLink->GetDestLink(j);
                if(!pDest) {
                    ASSERT(pDest);
                    continue;
                }

                strTarget = pDest->GetTargetName();
                //lookup in target info map
                if(m_TargetInfo.Lookup(strTarget,info)) {
                    //hit - actualize it with information
                    *pDest = info;
                    pDest->SetValid(true);
                }
                else {
                    pDest->SetValid(false);
                }

                FC_RELEASE_PTR(pDest);
            }
        }

        m_LinkList.FreeItemList(ppLinks);
    }
    catch(HRESULT hrRes) {
        hr = hrRes;
    }
    catch(CMemoryException *e) {
        e->Delete();
    }
    catch(CException* e) {
        e->Delete();
    }

}

void CLCConfig::AddMessageListener(CLCMessageListener *p)
{
    CLCMessageListener *pTemp = NULL;
    ULONG ulid = (ULONG) p;

    if(m_MessageListeners.Lookup(ulid,pTemp)) {
	    return;
    }

    m_MessageListeners.SetAt(ulid,p);
}

void CLCConfig::RemoveMessageListener(CLCMessageListener *p)
{
    CLCMessageListener *pTemp = NULL;
    ULONG ulid = (ULONG) p;

    if(!m_MessageListeners.Lookup(ulid,pTemp)) {
	    return;
    }

    m_MessageListeners.RemoveKey(ulid);
}

void CLCConfig::IssueMessage(HRESULT hr, LPCTSTR pszMessage)
{
    POSITION Pos;
    ULONG ulid;
    CLCMessageListener *pListener;

    for(Pos = m_MessageListeners.GetStartPosition();Pos != NULL;) {
        m_MessageListeners.GetNextAssoc(Pos,ulid,pListener);
        if(pListener) {
            pListener->OnMessage(hr,pszMessage);
        }
    }
}

int CLCConfig::Check()
{
    int iErrors = 0, iWarnings = 0;
    NameList  TypeLinkNameList;

    try {
        int i,iCount;
        LPTSTR psz = NULL;
        HRESULT hr;
        CString strMsg;
        ULONG ulIP;
        CString strTargetType;
        bool bPCTarget = false;
        bool bECTarget = false;
        bool bCMZTarget = false;

        //Check number of links
        if(!GetLinkCount()) {
            hr = E_CELATED_NO_LINKS;
            psz = strMsg.GetBuffer(MAX_PATH);
            UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr);
            strMsg.ReleaseBuffer();
            psz = NULL;
            IssueMessage(hr,strMsg);
            iErrors++;
        }

        //Check number of data types.
        if(!m_DataTypes.size()) {
            hr = E_CELATED_NO_DATATYPES;
            psz = strMsg.GetBuffer(MAX_PATH);
            UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr);
            strMsg.ReleaseBuffer();
            psz = NULL;
            IssueMessage(hr,strMsg);
            iErrors++;
        }

        
        iCount = m_LinkList.GetCount();

        for(i= 0; i < iCount; i++) {
            CLCLink* pLink = m_LinkList[i];
            if(pLink) {
                bPCTarget = false;
                bECTarget = false;
				bCMZTarget = false;
                // Check if PC
                strTargetType = pLink->GetTargetType();
                if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_PC)) {
                    bPCTarget = true;
                }
                // Check if EC
                if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_EC)) 
                {
                    bECTarget = true;
                }
                // Check if CMZ
                if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_CMZ)) 
                {
                    bCMZTarget = true;
                }
                //Check data type assignment.
                CLCDataType* pType = NULL;
                pType = pLink->GetDataType();
                if(!pType) {
                    hr = E_CELATED_ASSIGNED_TYPE;
                    psz = strMsg.GetBuffer(MAX_PATH);
                    UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                        pLink->GetTargetName());
                    strMsg.ReleaseBuffer();
                    psz = NULL;
                    IssueMessage(hr,strMsg);
                    iErrors++;
                } else {
                    CString strTemp;
                    CString strLinkName;
                    int iCharCount;

                    strLinkName = pLink->GetTargetName();
                    iCharCount = strLinkName.GetLength();
                    pType->GetName(strTemp);
                    iCharCount += strTemp.GetLength();
                    strTemp.Format(_T("%s - %s"), pLink->GetTargetName(), strTemp); 
                    if(iCharCount > UTIL_MAX_ID_LENGTH - MAX_GEN_TEXT) {
                        hr = E_CELATED_NAMES_TOO_LONG;
                        psz = strMsg.GetBuffer(MAX_PATH);
                        UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                            strTemp);
                        strMsg.ReleaseBuffer();
                        psz = NULL;
                        IssueMessage(hr,strMsg);
                        iErrors++;
                    }
                    // check UsedBySender (only one combination DataType-Sender allowed)
                    POSITION pos = TypeLinkNameList.Find(strTemp);
                    if(pos) {
                        hr = E_CELATED_LINK_ALREADY_USED;
                        psz = strMsg.GetBuffer(MAX_PATH);
                        UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                            strLinkName);
                        strMsg.ReleaseBuffer();
                        psz = NULL;
                        IssueMessage(hr,strMsg);
                        iErrors++;
                    }
                    TypeLinkNameList.AddTail(strTemp);
                    pType->Release();
                }
                
                //check destination targets 
                if(!pLink->GetDestLinkCount()) {
                    //no of destination links == 0
                    hr = E_CELATED_NO_DESTTARGET;
                    psz = strMsg.GetBuffer(MAX_PATH);
                    UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                        pLink->GetTargetName());
                    strMsg.ReleaseBuffer();
                    psz = NULL;
                    IssueMessage(hr,strMsg);
                    iErrors++;
                }

                //Source target valid
                if(!pLink->IsValid()) {
                    //No
                    hr = E_CELATED_INVTARGET;
                    psz = strMsg.GetBuffer(MAX_PATH);
                    UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                        pLink->GetTargetName());
                    strMsg.ReleaseBuffer();
                    psz = NULL;
                    IssueMessage(hr,strMsg);
                    iErrors++;
                }

                //Check validity of target addresses.
                ulIP = pLink->GetIP();
                if(!ulIP || ulIP == ULONG_MAX ) {
                    //invalid host
                    hr = W_CELATED_INVHOST;
                    psz = strMsg.GetBuffer(MAX_PATH);
                    UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                        pLink->GetTargetName());
                    strMsg.ReleaseBuffer();
                    psz = NULL;
                    IssueMessage(hr,strMsg);
                    iWarnings++;
                }

                for(int j = 0; j< pLink->GetDestLinkCount(); j++) {
                    CLCDestLink* pDest = pLink->GetDestLink(j);
                    if(!pDest) {
                        ASSERT(pDest);
                        continue;
                    }

                    // Check if PC
                    strTargetType = pDest->GetTargetType();
                    if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_PC)) {
                        bPCTarget |= true;
                    }
                    // Check if EC
                    if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_EC))
                    {
                        bECTarget |= true;
                    }
					// Check if CMZ
                    if(!strTargetType.CompareNoCase(LATED_TARGET_TYPE_CMZ))
                    {
                        bCMZTarget |= true;
                    }
                    //destination target valid ?
                    if(!pDest->IsValid()) {
                        //No
                        hr = E_CELATED_INVTARGET;
                        psz = strMsg.GetBuffer(MAX_PATH);
                        UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                            pDest->GetTargetName());
                        strMsg.ReleaseBuffer();
                        psz = NULL;
                        IssueMessage(hr,strMsg);
                        iErrors++;
                    }

                    ulIP = pDest->GetIP();
                    if(!ulIP || ulIP == ULONG_MAX ) {
                        //invalid host
                        hr = W_CELATED_INVHOST;
                        psz = strMsg.GetBuffer(MAX_PATH);
                        UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                            pDest->GetTargetName());
                        strMsg.ReleaseBuffer();
                        psz = NULL;
                        IssueMessage(hr,strMsg);
                        iWarnings++;
                    }

                    FC_RELEASE_PTR(pDest);
                }

                // now check the data types
                if(bPCTarget) {
                    // PC restrictions
                    CLCDataType* pType = NULL;
                    pType = pLink->GetDataType();
                    if(pType) {
                        if(pType->FindMemberByType(BL_INT)) {
                            hr = E_CELATED_UNSUPPORTED_TYPE_PC;
                            psz = strMsg.GetBuffer(MAX_PATH);
                            UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                                pType->GetName());
                            strMsg.ReleaseBuffer();
                            psz = NULL;
                            IssueMessage(hr,strMsg);
                            iErrors++;
                        }
                        pType->Release();
                    }
                }
                if(bECTarget) 
				{
                    // EC restrictions
                    CLCDataType* pType = NULL;
                    pType = pLink->GetDataType();
                    if(pType) {
                        if((pType->FindMemberByType(BL_LWORD)) ||
                           (pType->FindMemberByType(BL_LINT)) ||
                           (pType->FindMemberByType(BL_REAL)) ||
                           (pType->FindMemberByType(BL_LREAL)) ||
                           (pType->FindMemberByType(BL_DATE)) ||
                           (pType->FindMemberByType(BL_DT)) ||
                           (pType->FindMemberByType(BL_WSTRING))
                            ) {
                            hr = E_CELATED_UNSUPPORTED_TYPE_EC;
                            psz = strMsg.GetBuffer(MAX_PATH);
                            UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                                pType->GetName());
                            strMsg.ReleaseBuffer();
                            psz = NULL;
                            IssueMessage(hr,strMsg);
                            iErrors++;
                        }
                        pType->Release();
                    }
				}
                if(bCMZTarget) 
				{
                    CLCDataType* pType = NULL;
                    pType = pLink->GetDataType();
                    if(pType) 
					{
                        if(pType->FindMemberByType(BL_WSTRING))
						{
                            hr = E_CELATED_UNSUPPORTED_TYPE_CMZ;
                            psz = strMsg.GetBuffer(MAX_PATH);
                            UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,
                                pType->GetName());
                            strMsg.ReleaseBuffer();
                            psz = NULL;
                            IssueMessage(hr,strMsg);
                            iErrors++;
                        }
                        pType->Release();
                    }
                }
                pLink->Release();
            }
        }

        hr = S_CELATED_GEN_INFO;
        psz = strMsg.GetBuffer(MAX_PATH);
        UTIL_FormatMessage(AfxGetResourceHandle(),psz,MAX_PATH,hr,iErrors,iWarnings);
        strMsg.ReleaseBuffer();
        psz = NULL;
        IssueMessage(hr,strMsg);

        TypeLinkNameList.RemoveAll();
    }
    catch(CMemoryException* e) {
        e->Delete();
        iErrors++;
    }
   
    return iErrors;
}


//------------------------------------------------------------------*
/**
 * Allocates a list with references to all data type objects declared
 * with this configuration. Iterate through the list using
 * a NULL reference as end criteria e.g. 
 * for(int i = 0, ppTypes[i]; i++).<br>
 * Call FreeDataTypeList() to free the list of references.
 *
 * @return			CLCDataType ** pointer to list of references to
 *                  data type descriptors.
 * @exception		CMemoryException
 * @see			    FreeDataTypeList()    
*/
CLCDataType ** CLCConfig::GetDatatypeList()
{
    CLCDataType** ppTypes = m_DataTypes.GetAssocList();
    if(!ppTypes) {
        AfxThrowMemoryException();
    }

    return ppTypes;
}

//------------------------------------------------------------------*
/**
 * Frees an data type list allocated with GetDatatypeList().
 * @param		CLCDataType **ppTypes pointer to list 
 *              of data type references. Shall not be NULL.
 * @see			GetDatatypeList()
*/
void CLCConfig::FreeDataTypeList(CLCDataType **ppTypes)
{
    m_DataTypes.FreeAssocList(ppTypes);
}


bool CLCConfig::ConvertToIP(CString & strAddress, CString & strIP, ULONG & ulIP)
{
    CSockAddr addr;
    bool bIP = false;
    CString str = strAddress;

    try {
        CString strOPCServer;
        CString strCommChannel;
        CString strConnectPar;
        HRESULT hr;

        // hack to resolve the {}-problem
        str.TrimLeft(TCHAR('{'));
        str.TrimRight(TCHAR('}'));
        hr = UTIL_ParseControlString(str, 
                strOPCServer.GetBuffer(UTIL_MAX_OPC_SERVER_LEN), 
                NULL, // custom connect guid not needed
                NULL, // OPC server instance name not needed
                strCommChannel.GetBuffer(UTIL_MAX_COMM_CHANNEL_LEN), 
                strConnectPar.GetBuffer(UTIL_MAX_CONNECT_PAR_LEN), 
                NULL, // protocol not needed
                NULL, // protocol parameter not needed
                NULL  // FirmwareVersion not needed
                );

        strOPCServer.ReleaseBuffer();
        strCommChannel.ReleaseBuffer();
        strConnectPar.ReleaseBuffer();

        try {
            //try with host name first.
            if(!strConnectPar.IsEmpty()) {
                unsigned long ulAddr = ::inet_addr(strConnectPar);
                if(ulAddr == INADDR_NONE) {
                    addr = CBlockingSocket::GetHostByName(strConnectPar);
                } else {
                    CSockAddr TempAddr(strConnectPar);
                    addr = TempAddr;
                }
            } else {
                //Remove unused chars.
                strOPCServer.TrimLeft();
                strOPCServer.TrimRight();
                strOPCServer.TrimLeft(_T("{["));
                strOPCServer.TrimRight(_T("]}"));
                int iIndex = strOPCServer.ReverseFind(_T(','));
                if(iIndex != -1) {
                    strOPCServer = strOPCServer.Mid(iIndex+1);
                    bIP = true;
                }
                addr = CBlockingSocket::GetHostByName(strOPCServer);
            }
        }
        catch(CBlockingSocketException *e) {
            delete e;
            bIP = true;
        }

        if(addr.IPAddr() != 0UL) { //host name 
            ulIP = addr.IPAddr();
            addr.DottedDecimal(strIP.GetBuffer(_MAX_PATH),_MAX_PATH);
            strIP.ReleaseBuffer();
            return true;
        }

        //any case here try to convert and assume bIP == true
        CSockAddr addr2(strConnectPar);
        addr2.DottedDecimal(strIP.GetBuffer(_MAX_PATH),_MAX_PATH);
        strIP.ReleaseBuffer();
        ulIP = addr2.IPAddr();
    }
    catch(CMemoryException* e) {
        e->Delete();
        return false;
    }

    return true;
}


// methods with generated objects
void CLCConfig::AddGenObj(const CLCGenObj& genObj)
{
    m_NewGenObjs.AddTail(genObj);
}

void CLCConfig::GenObjsExecute()
{
    try {
        HRESULT hr;
        if (!m_pResourceOp)
        {
            hr = m_pResourceOp.CoCreateInstance(CLSID_CEAddonActionComObj);
        }

        CString strPath = m_strPath;
        int i = strPath.ReverseFind('\\');
        int j = strPath.GetLength() - i;
        strPath.Delete(i, j);

        POSITION pos;
        pos = m_GenObjs.GetHeadPosition();
        while(pos)
        {
            CLCGenObj genObj = m_GenObjs.GetNext(pos);
            BOOL bFound = FALSE;
            POSITION posNew = m_NewGenObjs.GetHeadPosition();
            while(posNew)
            {
                CLCGenObj genNewObj = m_NewGenObjs.GetNext(posNew);
                if (genObj.Compare(genNewObj))
                {
                    bFound = TRUE;
                    break;
                }
            }
            if (!bFound)
            {
                genObj.Remove(m_pPM, strPath, m_pResourceOp);
            }
        }

        m_GenObjs.RemoveAll();

        pos = m_NewGenObjs.GetHeadPosition();
        while(pos)
        {
            CLCGenObj genObj = m_NewGenObjs.GetNext(pos);
            genObj.Execute(m_pPM, strPath, m_pResourceOp);
            m_GenObjs.AddTail(genObj);
        }

        m_NewGenObjs.RemoveAll();
    }
    catch(CMemoryException *e) {
        e->Delete();
    }
    catch(CException* e) {
        e->Delete();
    }
}

IMPLEMENT_SERIAL(CLCDataType, CXMLObject, LATED_SERIAL_VERSION)

CLCDataType::CLCDataType()
    : m_unDataID(0) , m_strName() , m_Members() , m_strAlias()
{
    AddRef();
    CLCConfig::AddObject(this);
    m_bPCUsed = false;
    m_bECUsed = false;
	m_bCMZUsed = false;
}

CLCDataType::CLCDataType(unsigned short unDataID) 
    : m_unDataID(unDataID) , m_strName() , m_Members() , m_strAlias()
{
    AddRef();
    CLCConfig::AddObject(this);
    m_bPCUsed = false;
    m_bECUsed = false;
	m_bCMZUsed = false;
}


CLCDataType::~CLCDataType()
{
    m_Members.RemoveAll();
}

void CLCDataType::OnFinalRelease()
{
    CLCConfig::RemoveObject(this);
}


CLCDataType::CLCDataType(const CLCDataType & other)
{
    AddRef();
    CopyTo(other);
    CLCConfig::AddObject(this);
    m_bPCUsed = false;
    m_bECUsed = false;
	m_bCMZUsed = false;
}

const CLCDataType& CLCDataType::operator=(const CLCDataType & other)
{
    CopyTo(other);
    return *this;
}

void CLCDataType::CopyTo(const CLCDataType & other)
{
    if(this == &other) {
        return;
    }
    
    m_unDataID = other.m_unDataID;
    m_strName = other.m_strName;
    m_strAlias = other.m_strAlias;
    m_Members.RemoveAll();

    CLCDataType & rOther = const_cast<CLCDataType &>(other);

    for(int i = 0;i< rOther.m_Members.GetCount(); i++) {
        CLCMember* pMember = rOther.m_Members[i];
        if(pMember) {
            m_Members.Add(pMember);
        }
    }
}

UINT CLCDataType::GetContextMenu()
{ 
    return IDR_POPUP_STD;
}

void CLCDataType::SerializeXML(CXMLArchive & ar)
{
    ULONG ul;
    BOOL bRet;

    if(ar.IsStoring()) {
        ar.SetAttribute(LATED_ATTR_NAME,m_strName);
        ar.SetAttribute(LATED_ATTR_ALIAS,m_strAlias);
        ul = m_unDataID;
        ar.SetAttribute(LATED_ATTR_DATAID,ul);
        //iterate over members and write members...
        for(int i= 0; i< m_Members.GetCount(); i++ ) {
            CLCMember* pMember = m_Members[i];
            if(pMember) {
                ar << pMember;
                pMember->Release();
            }
        }

    }
    else { //loading
        ar.GetAttribute(LATED_ATTR_NAME,m_strName);
        ar.GetAttribute(LATED_ATTR_ALIAS,m_strAlias);
        ar.GetAttribute(LATED_ATTR_DATAID,ul);
        //dataid is of datatype of port number.
        m_unDataID = (unsigned short)ul;

        bRet = ar.IsElementExisting(LATED_ELEM_LC_MEMBER);
        if(bRet) {
            for(bRet = ar.MoveDown( LATED_ELEM_LC_MEMBER );
                bRet; bRet= ar.MoveNext()) {

                CXMLObject * pObj = NULL;
                pObj = ar.ReadObject();
                CLCMember* pMember = dynamic_cast<CLCMember*>(pObj);
                if(pMember) {
                    m_Members.Add(pMember);
                    pMember = NULL;
                }
            }
            ar.MoveUp();
        }
    }
}

void CLCDataType::GetKey(CString & strKey)
{
    strKey = m_strName;
    strKey.MakeLower();
}

void CLCDataType::TraverseMembers(int id, CLCConfigListener *pListener)
{
    if(!pListener) {
        //no one listens, so return...
        return;
    }

    for(int i = 0;i< m_Members.GetCount(); i++) {
        CLCMember* pMember = m_Members[i];
        if(pMember) {
            pListener->OnMember(id,this,pMember);
            pMember->Release();
        }
    }
}

void CLCDataType::AddMember(CLCMember* pMember)
{
    if(!pMember) {
        ASSERT(pMember);
        return;
    }
    m_Members.Add(pMember);
}

void CLCDataType::RemoveMembers()
{
    m_Members.RemoveAll();
}

bool CLCDataType::FindMemberByType(BL_IEC_TYP TypeToFind) {
    for(int i = 0;i< m_Members.GetCount(); i++) {
        CLCMember* pMember = m_Members[i];
        if(pMember) {
            BL_IEC_TYP Type = pMember->GetType();
            pMember->Release();
            if(Type == TypeToFind) {
                return(true);
            }
        }
    }
    return(false);
}

size_t CLCDataType::GetDataLen()
{
    size_t size = 0;
    
    for(int i = 0;i< m_Members.GetCount(); i++) {
        CLCMember* pMember = m_Members[i];
        if(pMember) {
            size += pMember->GetDataLen();
            pMember->Release();
        }
    }

    return size;
}

IMPLEMENT_SERIAL(CLCMember, CXMLObject, LATED_SERIAL_VERSION)

CLCMember::CLCMember() : m_strName() , m_Type(BL_ANY)
    , m_strAlias() , m_strComment(), m_strInitVal()
{
    AddRef();
    m_bSubscript = false;
    m_size = 0;
    CLCConfig::AddObject(this);
}

CLCMember::CLCMember(const CLCMember & other)
{
    AddRef();
    CopyTo(other);
    CLCConfig::AddObject(this);
}

void CLCMember::OnFinalRelease()
{
    CLCConfig::RemoveObject(this);
}

const CLCMember& CLCMember::operator=(const CLCMember & other)
{
    CopyTo(other);
    return *this;
}

void CLCMember::CopyTo(const CLCMember & other)
{
    if(this == &other) {
        return;
    }
    
    m_strName = other.m_strName;
    m_Type = other.m_Type;
    m_strAlias = other.m_strAlias;
    m_strComment = other.m_strComment;
    m_strInitVal = other.m_strInitVal;
    m_size = other.m_size;
    m_bSubscript = other.m_bSubscript;
}

size_t CLCMember::GetDataLen()
{
    return m_size;
}

void CLCMember::SerializeXML(CXMLArchive & ar)
{
    CString str,strType;
    DWORD dwModifier = 0;
    CComBSTR sTypeName;

    if(ar.IsStoring()) {
        ar.SetAttribute(LATED_ATTR_NAME,m_strName);
        sTypeName = BL_DatatypeToTypename(m_Type,dwModifier);
        strType = sTypeName;

        if(m_bSubscript) {
            strType += LATED_BRACKET_S;
            str.Format("%d",m_size);
            strType += str;
            strType += LATED_BRACKET_E;
        }

        ar.SetAttribute(LATED_ATTR_TYPE,strType);
    }
    else { //loading
        ar.GetAttribute(LATED_ATTR_NAME,m_strName);
        ar.GetAttribute(LATED_ATTR_TYPE,strType);

        m_bSubscript = CLCConfig::GetSubscript(strType,str,m_size);
        if(m_bSubscript) {
            sTypeName = str;
        }
        else {
            sTypeName = strType;
        }

        BL_TypenameToDatatype(sTypeName,&m_Type,&dwModifier);
        if(!m_bSubscript) {
            m_size = CLCConfig::GetDataLenFromType(m_Type);            
        }
    }
}

bool CLCMember::GetToolTip(CString & str)
{
    CString strHelp;
    CComBSTR sTypeName;
    DWORD dwModifier = 0;
    
    str.Empty();
    sTypeName = BL_DatatypeToTypename(m_Type,dwModifier);
    strHelp = sTypeName;
    str.Format(IDS_LATED_MEMBER_TT,strHelp);
    return true;
}

UINT CLCMember::GetContextMenu()
{ 
    return 0;
}

void CLCMember::SetProperty(int mask, CString & strName, CString & strType,
                CString & strAlias, CString & strComment, CString & strInitVal)
{
    if(mask & CLCMember::name) {
        m_strName = strName;
    }

    if(mask & CLCMember::type) {
        DWORD dwModifier = 0;
        CComBSTR sTypeName;
        CString str;

        m_bSubscript = CLCConfig::GetSubscript(strType,str,m_size);
        if(m_bSubscript) {
            sTypeName = str;
        }
        else {
            sTypeName = strType;
        }

        BL_TypenameToDatatype(sTypeName,&m_Type,&dwModifier);
        if(!m_bSubscript) {
            m_size = CLCConfig::GetDataLenFromType(m_Type);            
        }
    }

    if(mask & CLCMember::alias) {
        m_strAlias = strAlias;
    }

    if(mask & CLCMember::comment) {
        m_strComment = strComment;
    }

    if(mask & CLCMember::initval) {
        m_strInitVal = strInitVal;
    }
}

void CLCMember::GetProperty(int mask, CString & strName, CString & strType,
                CString & strAlias, CString & strComment, CString & strInitVal)
{
    if(mask & CLCMember::name) {
        strName = m_strName;
    }

    if(mask & CLCMember::type) {
        DWORD dwModifier = 0;
        CComBSTR sTypeName;
        CString str;

        sTypeName = BL_DatatypeToTypename(m_Type,dwModifier);
        strType = sTypeName;

        if(m_bSubscript) {
            strType += LATED_BRACKET_S;
            str.Format("%d",m_size);
            strType += str;
            strType += LATED_BRACKET_E;
        }
    }

    if(mask & CLCMember::alias) {
        strAlias = m_strAlias;
    }

    if(mask & CLCMember::comment) {
        strComment = m_strComment;
    }

    if(mask & CLCMember::initval) {
        strInitVal = m_strInitVal;
    }
}




IMPLEMENT_SERIAL(CLCLink, CXMLObject, LATED_SERIAL_VERSION)

CLCLink::CLCLink() : m_strTarget() , m_strType(), m_strResource() ,
    m_DatatypeList() , m_DatatypeNameList() , m_DestLinks()
{
    m_ConnType = UNKNOWN;
    m_ulIP = 0UL;
    m_bValid = true;
    AddRef();

    CLCConfig::AddObject(this);
}

CLCLink::~CLCLink()
{
    m_DatatypeNameList.RemoveAll();
    m_DatatypeList.RemoveAll();
    m_DestLinks.RemoveAll();
}

void CLCLink::OnFinalRelease()
{
    CLCConfig::RemoveObject(this);
}

CLCLink::CLCLink(const CLCLink & other) : m_DestLinks()
{
    AddRef();
    CopyTo(other);
    CLCConfig::AddObject(this);
}

const CLCLink& CLCLink::operator=(const CLCLink & other)
{
    CopyTo(other);
    return *this;
}

const CLCLink& CLCLink::operator=(const CTargetInfo & info)
{
    m_strTarget = info.m_strName;
    m_strResource = info.m_strResource;
    m_strIP = info.m_strIP;
    m_strType = info.m_strType;
    m_ulIP = info.m_ulIP;
    m_bValid = info.m_bValid;
    return *this;
}

void CLCLink::ToTargetInfo(CTargetInfo & info)
{
    info.m_strName = m_strTarget;
    info.m_strIP = m_strIP;
    info.m_ulIP = m_ulIP;
    info.m_strResource = m_strResource;
    info.m_strType = m_strType;
    info.m_bValid = m_bValid;
}

void CLCLink::CopyTo(const CLCLink & other)
{
    int i,iCount;

    if(this == &other) {
        return;
    }

    m_ulIP = other.m_ulIP;
    m_strTarget = other.m_strTarget;
    m_strResource = other.m_strResource;
    m_strType = other.m_strType;
    m_ConnType = other.m_ConnType;
    m_strIP = other.m_strIP;
    m_bValid = other.m_bValid;

    m_DatatypeNameList.RemoveAll();
    m_DatatypeList.RemoveAll();
    m_DestLinks.RemoveAll();
    
    for(POSITION pos = other.m_DatatypeNameList.GetHeadPosition(); pos;) {
       CString str = other.m_DatatypeNameList.GetNext(pos);
       m_DatatypeNameList.AddTail(str);
    }
    
    CLCLink & rOther = const_cast<CLCLink &>(other);
    //copy of datatype list.
    CLCDataType** ppDatatypes = rOther.m_DatatypeList.GetItemList();
    if(!ppDatatypes) {
        AfxThrowMemoryException();
    }

    for(i= 0;ppDatatypes[i];i++) {
        ppDatatypes[i]->AddRef();
        m_DatatypeList.Add(ppDatatypes[i]);
    }
    
    rOther.m_DatatypeList.FreeItemList(ppDatatypes);

    iCount = rOther.m_DestLinks.GetCount();
    for(i=0;i<iCount;i++) {
        CLCDestLink* pDest = rOther.m_DestLinks[i];
        if(pDest) {
            m_DestLinks.Add(pDest);
        }
    }
}

void CLCLink::GetName(CString & strName)
{
    CString str;
    strName = m_strTarget;

    if(!m_bValid) {
        strName += _T(" (");
        str.LoadString(IDS_INVALID_LINK);
        strName += str;
        strName += _T(")");
        return;
    }

    CLCDataType* p = m_DatatypeList[0];
    if(p) {
        strName += _T(" (");
        p->GetName(str);
        strName += str;
        strName += _T(")");
        p->Release();
    }
}

int CLCLink::GetImageId()
{
    if(!m_strType.CompareNoCase(LATED_TARGET_TYPE_PC)) {
        return LATED_IMAGEID_LINKSRCPC;
    }

    if(!m_strType.CompareNoCase(LATED_TARGET_TYPE_EC)) {
        return LATED_IMAGEID_LINKSRCEC;
    }

	if(!m_strType.CompareNoCase(LATED_TARGET_TYPE_CMZ)) {
		return LATED_IMAGEID_LINKSRCCMZ;
	}

    return LATED_IMAGEID_LINKSRCPC;
}

bool CLCLink::GetToolTip(CString & str)
{
    CString strIPSrc;
    GetIP(strIPSrc);
    str.Empty();

    switch(m_ConnType) {
    case P2P:
        str.Format(IDS_LATED_P2P_TT,strIPSrc);
        break;
    case PUBLISH:
        str.Format(IDS_LATED_PUBLISH_TT,strIPSrc);        
        break;
    case SUBSCRIBE:
        return false;
    }

    return true;
}

UINT CLCLink::GetContextMenu()
{
    return IDR_POPUP_LINK;
}

void CLCLink::SerializeXML(CXMLArchive & ar)
{
    CString str;
    CLCDestLink* pDest = NULL;
    BOOL bRet = FALSE;
    CXMLObject* pObj = NULL;
    
    if(ar.IsStoring()) {
        //mandatory attributes first
        ar.SetAttribute(LATED_ATTR_IP,m_ulIP);
        ar.SetAttribute(LATED_ATTR_TARGET,m_strTarget);
        ar.SetAttribute(LATED_ATTR_TYPE,m_strType);

        str = CLCConfig::FromConnType(m_ConnType);
        ar.SetAttribute(LATED_ATTR_CONNTYPE,str);

        for(int i = 0; i< m_DatatypeList.GetCount(); i++) {
            CLCDataType * p = m_DatatypeList[i];
            if(!p) continue;

            p->GetName(str);
            p->Release();

            bRet = ar.MoveDown( LATED_ELEM_LC_DATA );
            if(bRet) {
                ar.SetAttribute(LATED_ATTR_NAME,str);
                ar.MoveUp();
            }
        }

        CLCDestLink** ppLinks = m_DestLinks.GetItemList();
        if(!ppLinks) {
            AfxThrowMemoryException();
        }

        for(i= 0; ppLinks[i]; i++) {
            ar << ppLinks[i];
        }

        m_DestLinks.FreeItemList(ppLinks);

    }
    else { //loading
        //mandatory attributes first
        ar.GetAttribute(LATED_ATTR_IP,m_ulIP);
        ar.GetAttribute(LATED_ATTR_TARGET,m_strTarget);
        ar.GetAttribute(LATED_ATTR_TYPE,m_strType);
        ar.GetAttribute(LATED_ATTR_CONNTYPE,str);
        
        m_ConnType = CLCConfig::ToConnType(str);

        CSockAddr addr(m_ulIP);
        addr.DottedDecimal(m_strIP.GetBuffer(LATED_IPSTR_LEN),LATED_IPSTR_LEN);
        m_strIP.ReleaseBuffer();

        bRet = ar.IsElementExisting(LATED_ELEM_LC_LINK_DEST);
        if(bRet) {
            for(bRet = ar.MoveDown( LATED_ELEM_LC_LINK_DEST );
                bRet; bRet = ar.MoveNext()) {
                pObj = ar.ReadObject();
                pDest = dynamic_cast<CLCDestLink*>(pObj);
                if(pDest) {
                    m_DestLinks.Add(pDest);
                    if(m_ConnType == PUBLISH) {
                        pDest->SetPublisherAddress(m_ulIP);
                    }
                    pDest = NULL;
                }
            }
            ar.MoveUp();
        }

        bRet = ar.IsElementExisting(LATED_ELEM_LC_DATA);
        if(bRet) {
            for(bRet = ar.MoveDown( LATED_ELEM_LC_DATA );
                bRet; bRet = ar.MoveNext()) {
            
                if(!ar.GetAttribute(LATED_ATTR_NAME,str)) {
                    continue;
                }
                
                m_DatatypeNameList.AddTail(str);
            }
            ar.MoveUp();
        }
    }
}

void CLCLink::ResolveDatatypes(CLCConfig *pConfig)
{
    //iterate through list of references
    for(POSITION pos = m_DatatypeNameList.GetHeadPosition(); pos;) {
        CString str = m_DatatypeNameList.GetNext(pos);

        //lookup datatype within configuration
        CLCDataType* pDatatype = pConfig->GetDatatype(str);
        //it's addref'd
        if(pDatatype) { //found
            m_DatatypeList.Add(pDatatype);
        }
    }
    //now clear the datatype name list
    m_DatatypeNameList.RemoveAll();
}

bool CLCLink::IsSubscriber(CLCLink* pPublisher)
{
    if(m_ConnType != SUBSCRIBE) {
        return false;
    }

    if(!m_strTarget.CompareNoCase(pPublisher->m_strTarget)) {
        return true;
    }

    return false;
}

void CLCLink::TraverseDestLinks(int id,CLCConfigListener* pListener)
{
    if(!pListener) {
        return;
    }

    CLCDestLink** ppLinks = m_DestLinks.GetItemList();
    if(!ppLinks) {
        AfxThrowMemoryException();
    }

    for(int i= 0; ppLinks[i]; i++) {
        pListener->OnDestLink(id,this,ppLinks[i]);
    }

    m_DestLinks.FreeItemList(ppLinks);
}

void CLCLink::GetIP(CString & str)
{
    CSockAddr addr(m_ulIP);
    str.Empty();
    addr.DottedDecimal(str.GetBuffer(LATED_IPSTR_LEN),LATED_IPSTR_LEN);
    str.ReleaseBuffer();
}

void CLCLink::AddDestLink(CLCDestLink* pDest)
{
    ASSERT(pDest);
    if(!pDest) {
        return;
    }
    m_DestLinks.Add(pDest);
}

void CLCLink::DeleteDestLink(CLCDestLink* pDest)
{
    ASSERT(pDest);
    if(!pDest) {
        return;
    }
    m_DestLinks.Remove(pDest);
}

bool CLCLink::ExistsDestOfType(LPCTSTR szTargetType) {
    for(int i = 0; i < GetDestLinkCount(); i++) {
        CString strTargetType = GetDestLink(i)->GetTargetType();
        if(!strTargetType.CompareNoCase(szTargetType)) {
            return(true);
        }
    }
    return(false);
}

void CLCLink::AssignDataType(CLCDataType* p)
{
    if(!p) {
        m_DatatypeList.RemoveAll();
    }
    else {
        p->AddRef();
        m_DatatypeList.RemoveAll();
        m_DatatypeList.Add(p);
        p->Release();
    }
}

IMPLEMENT_SERIAL(CLCDestLink, CXMLObject, LATED_SERIAL_VERSION)

CLCDestLink::CLCDestLink() : m_strType() , m_strTarget() 
    , m_strIP() , m_strResource()
{
    m_ConnType = UNKNOWN;
    m_ulIP = 0UL;
    m_bValid = true;
    AddRef();

    CLCConfig::AddObject(this);

}

CLCDestLink::CLCDestLink(const CLCDestLink & other)
{
    AddRef();
    CopyTo(other);
    CLCConfig::AddObject(this);
}

void CLCDestLink::OnFinalRelease()
{
    CLCConfig::RemoveObject(this);
}

const CLCDestLink& CLCDestLink::operator=(const CLCDestLink & other)
{
    CopyTo(other);
    return *this;
}

const CLCDestLink& CLCDestLink::operator=(const CTargetInfo & info)
{
    m_strTarget = info.m_strName;
    m_strIP = info.m_strIP;
    m_strType = info.m_strType;
    m_ulIP = info.m_ulIP;
    m_strResource = info.m_strResource;
    m_bValid = info.m_bValid;
    return *this;
}

void CLCDestLink::ToTargetInfo(CTargetInfo & info)
{
    info.m_strName = m_strTarget;
    info.m_strIP = m_strIP;
    info.m_ulIP = m_ulIP;
    info.m_strType = m_strType;
    info.m_strResource = m_strResource;
    info.m_bValid = m_bValid;
}

void CLCDestLink::CopyTo(const CLCDestLink & other)
{
    if(this == &other) {
        return;
    }

    m_ulIP = other.m_ulIP;
    m_strTarget = other.m_strTarget;
    m_strType = other.m_strType;
    m_ConnType = other.m_ConnType;
    m_strIP = other.m_strIP;
    m_strResource = other.m_strResource;
    m_bValid = other.m_bValid;
}

void CLCDestLink::GetName(CString & strName)
{
    CString str;
    strName = m_strTarget;
    if(!m_bValid) {
        strName += _T(" (");
        str.LoadString(IDS_INVALID_LINK);
        strName += str;
        strName += _T(")");
    }
}

int CLCDestLink::GetImageId()
{
    if(!m_strType.CompareNoCase(LATED_TARGET_TYPE_PC)) {
        return LATED_IMAGEID_LINKDSTPC;
    }

    if(!m_strType.CompareNoCase(LATED_TARGET_TYPE_EC)) {
        return LATED_IMAGEID_LINKDSTEC;
    }

	if(!m_strType.CompareNoCase(LATED_TARGET_TYPE_CMZ)) {
		return LATED_IMAGEID_LINKDSTCMZ;
	}

    return LATED_IMAGEID_LINKDSTPC;
}


bool CLCDestLink::GetToolTip(CString & str)
{
    CSockAddr addr(m_ulIP);
    CString strIPSrc;

    str.Empty();

    addr.DottedDecimal(strIPSrc.GetBuffer(LATED_IPSTR_LEN),LATED_IPSTR_LEN);
    strIPSrc.ReleaseBuffer();

    switch(m_ConnType) {
    case P2P:
        str.Format(IDS_LATED_P2P_TT,strIPSrc);
        break;
    case PUBLISH:
        return false;
    case SUBSCRIBE:
        str.Format(IDS_LATED_SUBSCRIBE_TT,strIPSrc);        
        break;
    }

    return true;
}

UINT CLCDestLink::GetContextMenu()
{ 
    return IDR_POPUP_DESTLINK;
}

void CLCDestLink::SerializeXML(CXMLArchive & ar)
{
    CString str;
    
    
    if(ar.IsStoring()) {
        //mandatory attributes first
        ar.SetAttribute(LATED_ATTR_IP,m_ulIP);
        ar.SetAttribute(LATED_ATTR_TARGET,m_strTarget);
        ar.SetAttribute(LATED_ATTR_TYPE,m_strType);

        str = CLCConfig::FromConnType(m_ConnType);
        ar.SetAttribute(LATED_ATTR_CONNTYPE,str);
    }
    else { //loading
        //mandatory attributes first
        ar.GetAttribute(LATED_ATTR_IP,m_ulIP);
        ar.GetAttribute(LATED_ATTR_TARGET,m_strTarget);
        ar.GetAttribute(LATED_ATTR_TYPE,m_strType);
        ar.GetAttribute(LATED_ATTR_CONNTYPE,str);
        m_ConnType = CLCConfig::ToConnType(str);

        CSockAddr addr(m_ulIP);
        addr.DottedDecimal(m_strIP.GetBuffer(LATED_IPSTR_LEN),LATED_IPSTR_LEN);
        m_strIP.ReleaseBuffer();
    }
}

void CLCDestLink::SetPublisherAddress(ULONG ulIP)
{
    if(m_ConnType == SUBSCRIBE) {
        m_ulIP = ulIP;
    }
}

void CLCDestLink::GetIP(CString & str)
{
    CSockAddr addr(m_ulIP);
    str.Empty();
    addr.DottedDecimal(str.GetBuffer(LATED_IPSTR_LEN),LATED_IPSTR_LEN);
    str.ReleaseBuffer();
}



/****************************************************************************
    CGenObj
 ****************************************************************************/

IMPLEMENT_SERIAL(CLCGenObj, CXMLObject, LATED_SERIAL_VERSION)


//------------------------------------------------------------------*
/**
 * CLCGenObj
 *
 */
CLCGenObj::CLCGenObj()
{
    m_eType = GENTYPE_UNDEF;
    m_ePos  = GENPOS_UNDEF;
}


//------------------------------------------------------------------*
/**
 * ~CLCGenObj
 *
 */
CLCGenObj:: ~CLCGenObj()
{
}


CLCGenObj::CLCGenObj(const CLCGenObj& other)
{
    if(this == &other) 
    {
        return;
    }

    m_eType = other.m_eType;
    m_strFile = other.m_strFile;
    m_strName = other.m_strName;
    m_strClass = other.m_strClass;
    m_strTask = other.m_strTask;
    m_strInterval = other.m_strInterval;
    m_strPriority = other.m_strPriority;
    m_ePos = other.m_ePos;
}

const CLCGenObj& CLCGenObj::operator=(const CLCGenObj& other)
{
    if(this != &other) 
    {
        m_eType = other.m_eType;
        m_strFile = other.m_strFile;
        m_strName = other.m_strName;
        m_strClass = other.m_strClass;
        m_strTask = other.m_strTask;
        m_strInterval = other.m_strInterval;
        m_strPriority = other.m_strPriority;
        m_ePos = other.m_ePos;
    }

    return *this;
}


void CLCGenObj::Clear()
{
    m_eType = GENTYPE_UNDEF;
    m_strFile.Empty();
    m_strName.Empty();
    m_strClass.Empty();    
    m_strClass.Empty();
    m_strInterval.Empty();
    m_strPriority.Empty(); 
    m_ePos = GENPOS_UNDEF;
}

void CLCGenObj::InitFile(const CString& strFile)
{
    Clear();
    m_eType = GENTYPE_FILE;
    m_strFile = strFile;
}

void CLCGenObj::InitLinkImp(const CString& strResource, const CString& strGVL)
{
    Clear();
    m_eType = GENTYPE_LINK_IMP;
    m_strFile = strResource;
    m_strName = strGVL;
}

void CLCGenObj::InitLinkProg(const CString& strResource, const CString& strProgInst, const CString& strTask, 
                             const CString& strProgClass, BOOL bPosFirst)
{
    Clear();
    m_eType = GENTYPE_LINK_PROG;
    m_strFile = strResource;
    m_strName = strProgInst;
    m_strClass = strProgClass;
    m_strTask = strTask;
    m_ePos = bPosFirst?GENPOS_FIRST:GENPOS_LAST;
}

void CLCGenObj::InitLinkTask(const CString& strResource, const CString& strTaskName,
                             const CString& strInterval, const CString& strPriority,
                             BOOL bPosFirst)
{
    Clear();
    m_eType = GENTYPE_LINK_TASK;
    m_strFile = strResource;
    m_strName = strTaskName;
    m_strInterval = strInterval;
    m_strPriority = strPriority;
    m_ePos = bPosFirst?GENPOS_FIRST:GENPOS_LAST;
}



//------------------------------------------------------------------*
/**
 * SerializeXML
 *
 */
void CLCGenObj::SerializeXML(CXMLArchive & ar)
{
    CString str;
    if(ar.IsStoring()) 
    {
        switch(m_eType)
        {
        case GENTYPE_UNDEF:
            return;
        case GENTYPE_FILE:
            str = LATED_GENTYPE_FILE;
            break;
        case GENTYPE_LINK_IMP:
            str = LATED_GENTYPE_LINK_IMP;
            break;
        case GENTYPE_LINK_TASK:
            str = LATED_GENTYPE_LINK_TASK;
            break;
        case GENTYPE_LINK_PROG:
            str = LATED_GENTYPE_LINK_TASK;
            break;
        }
        ar.SetAttribute(LATED_ATTR_GENTYPE, str);
        ar.SetAttribute(LATED_ATTR_FILE, m_strFile);
        ar.SetAttribute(LATED_ATTR_NAME, m_strName);
        ar.SetAttribute(LATED_ATTR_CLASS, m_strClass);
        ar.SetAttribute(LATED_ATTR_TASK, m_strTask);
        ar.SetAttribute(LATED_ATTR_INTERVAL, m_strInterval);
        ar.SetAttribute(LATED_ATTR_PRIORITY, m_strPriority);

        str.Empty();
        switch(m_ePos)
        {
        case GENPOS_FIRST:
            str = LATED_GENPOS_FIRST;
            break;
        case GENPOS_LAST:
            str = LATED_GENPOS_LAST;
            break;
        }
        if (!str.IsEmpty())
        {
            ar.SetAttribute(LATED_ATTR_GENPOS, str);
        }
    }
    else 
    { //loading
        m_strFile.Empty();
        m_strName.Empty();
        ar.GetAttribute(LATED_ATTR_GENTYPE, str);
        m_eType = GENTYPE_UNDEF;
        if (str.Compare(LATED_GENTYPE_FILE)==0)
        {
            m_eType = GENTYPE_FILE;
        }
        else if (str.Compare(LATED_GENTYPE_LINK_IMP)==0)
        {
            m_eType = GENTYPE_LINK_IMP;
        }
        else if (str.Compare(LATED_GENTYPE_LINK_TASK)==0)
        {
            m_eType = GENTYPE_LINK_TASK;
        }
        else if (str.Compare(LATED_GENTYPE_LINK_TASK)==0)
        {
            m_eType = GENTYPE_LINK_TASK;
        }

        ar.GetAttribute(LATED_ATTR_FILE, m_strFile);
        ar.GetAttribute(LATED_ATTR_NAME, m_strName);
        ar.GetAttribute(LATED_ATTR_CLASS, m_strClass);
        ar.GetAttribute(LATED_ATTR_INTERVAL, m_strInterval);
        ar.GetAttribute(LATED_ATTR_PRIORITY, m_strPriority);
        ar.GetAttribute(LATED_ATTR_TASK, m_strTask);

        str.Empty();
        ar.GetAttribute(LATED_ATTR_GENPOS, str);
        m_ePos = GENPOS_UNDEF;
        if (str.Compare(LATED_GENPOS_FIRST)==0)
        {
            m_ePos = GENPOS_FIRST;
        }
        else if (str.Compare(LATED_GENPOS_LAST)==0)
        {
            m_ePos = GENPOS_LAST;
        }
    }
}


//------------------------------------------------------------------*
/**
 * Execute
 *
 */
void CLCGenObj::Execute(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    if (m_eType == GENTYPE_FILE)
    {
        CreateLinkFile(pPM, strProjPath, rpResourceOp);
    }
    else if (m_eType == GENTYPE_LINK_IMP)
    {
        CreateLinkImp(pPM, strProjPath, rpResourceOp);
    }
    else if (m_eType == GENTYPE_LINK_TASK)
    {
        CreateLinkTask(pPM, strProjPath, rpResourceOp);
    }
    else if (m_eType == GENTYPE_LINK_PROG)
    {
        CreateLinkProg(pPM, strProjPath, rpResourceOp);
    }
}


//------------------------------------------------------------------*
/**
 * Remove
 *
 */
void CLCGenObj::Remove(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    if (m_eType == GENTYPE_FILE)
    {
        RemoveLinkFile(pPM, strProjPath, rpResourceOp);
    }
    else if (m_eType == GENTYPE_LINK_IMP)
    {
        RemoveLinkImp(pPM, strProjPath, rpResourceOp);
    }
    else if (m_eType == GENTYPE_LINK_TASK)
    {
        RemoveLinkTask(pPM, strProjPath, rpResourceOp);
    }
    else if (m_eType == GENTYPE_LINK_PROG)
    {
        RemoveLinkProg(pPM, strProjPath, rpResourceOp);
    }
}

BOOL CLCGenObj::Compare(const CLCGenObj& other)
{
    if (this->m_eType != other.m_eType)
    {
        return FALSE;
    }

    if (m_eType == GENTYPE_FILE)
    {
        if (this->m_strFile.CompareNoCase(other.m_strFile)==0)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if (m_eType == GENTYPE_LINK_IMP)
    {
        if (this->m_strFile.CompareNoCase(other.m_strFile)==0 &&
            this->m_strName.CompareNoCase(other.m_strName)==0 )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }    
    }
    else if (m_eType == GENTYPE_LINK_TASK)
    {
        if (this->m_strFile.CompareNoCase(other.m_strFile)==0 &&
            this->m_strName.CompareNoCase(other.m_strName)==0 &&
            this->m_strInterval.CompareNoCase(other.m_strInterval)==0 &&
            this->m_strPriority.CompareNoCase(other.m_strPriority)==0 &&
            this->m_ePos == other.m_ePos)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }    
    }
    else if (m_eType == GENTYPE_LINK_PROG)
    {
        if (this->m_strFile.CompareNoCase(other.m_strFile)==0 &&
            this->m_strName.CompareNoCase(other.m_strName)==0 &&
            this->m_strTask.CompareNoCase(other.m_strTask)==0 &&
            this->m_strClass.CompareNoCase(other.m_strClass)==0 &&
            this->m_ePos == other.m_ePos)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }    
    }

    return FALSE;
}


//------------------------------------------------------------------*
/**
 * CreateLinkFile
 *
 */
void CLCGenObj::CreateLinkFile(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
     HRESULT hr;
     CComBSTR sFileName(m_strFile);
     hr = pPM->AddFile(sFileName, FV_FILE_SOURCE, FALSE);
}

//------------------------------------------------------------------*
/**
 * RemoveLinkFile
 *
 */
void CLCGenObj::RemoveLinkFile(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
     HRESULT hr;
     CComBSTR sFileName(m_strFile);
     hr = pPM->RemoveFile(sFileName, FV_FILE_SOURCE, FALSE);

     CString strLongFileName;
     strLongFileName.Format(_T("%s\\%s"), strProjPath, m_strFile);
     try
     {
        CFile::Remove(strLongFileName);
     }
     catch(CFileException* e) 
     {
        #ifdef _DEBUG
            afxDump << "File could not be opened "
                << e->m_cause << "\n";
        #endif
        e->Delete();
    }
}

//------------------------------------------------------------------*
/**
 * CreateLinkImp
 *
 */
void CLCGenObj::CreateLinkImp(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    CComQIPtr<ICEProjInfo> pProjInfo(pPM);
    if (!rpResourceOp || !pProjInfo)
    {
        return;
    }

    HRESULT hr;
    CComBSTR sResourceName(m_strFile);
    CComBSTR sGVL(m_strName);
    hr = rpResourceOp->InsertGVLImport(pProjInfo,
                                       sResourceName,
                                       sGVL);
}

//------------------------------------------------------------------*
/**
 * RemoveLinkImp
 *
 */
void CLCGenObj::RemoveLinkImp(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    CComQIPtr<ICEProjInfo> pProjInfo(pPM);
    if (!rpResourceOp || !pProjInfo)
    {
        return;
    }

    HRESULT hr;
    CComBSTR sResourceName(m_strFile);
    CComBSTR sGVL(m_strName);
    hr = rpResourceOp->RemoveGVLImport(pProjInfo,
                                       sResourceName,
                                       sGVL);
}

//------------------------------------------------------------------*
/**
 * CreateLinkTask
 *
 */
void CLCGenObj::CreateLinkTask(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    CComQIPtr<ICEProjInfo> pProjInfo(pPM);
    if (!rpResourceOp || !pProjInfo)
    {
        return;
    }

    HRESULT hr;
    CComBSTR sResourceName(m_strFile);
    CComBSTR sTaskName(m_strName);
    CComBSTR sInterval(m_strInterval);
    CComBSTR sPriority(m_strPriority);
    BOOL     bAutostart = TRUE;
    BOOL bPosFirst = FALSE;
    if (m_ePos==GENPOS_FIRST)
    {
        bPosFirst = GENPOS_LAST;
    }

    hr = rpResourceOp->InsertTask(pProjInfo,
                                  sResourceName,
                                  sTaskName,
                                  sInterval,
                                  sPriority,
                                  bAutostart,
								  FALSE,	// NFTASKHANDLING 19.05.05 SIS
                                  NULL,
                                  bPosFirst);
}

//------------------------------------------------------------------*
/**
 * RemoveLinkTask
 *
 */
void CLCGenObj::RemoveLinkTask(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    CComQIPtr<ICEProjInfo> pProjInfo(pPM);
    if (!rpResourceOp || !pProjInfo)
    {
        return;
    }

    HRESULT hr;
    CComBSTR sResourceName(m_strFile);
    CComBSTR sTaskName(m_strName);

    hr = rpResourceOp->RemoveTask(pProjInfo,
                                  sResourceName,
                                  sTaskName);
}

//------------------------------------------------------------------*
/**
 * CreateLinkProg
 *
 */
void CLCGenObj::CreateLinkProg(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    CComQIPtr<ICEProjInfo> pProjInfo(pPM);
    if (!rpResourceOp || !pProjInfo)
    {
        return;
    }

     HRESULT hr;
     CComBSTR sResourceName(m_strFile);
     CComBSTR sTaskName(m_strTask);
     CComBSTR sProgClass(m_strClass);
     CComBSTR sProgInstName(m_strName);
     BOOL bPosFirst = FALSE;
     if (m_ePos==GENPOS_FIRST)
     {
         bPosFirst = GENPOS_LAST;
     }
     
     hr = rpResourceOp->InsertProgramInstance(pProjInfo,
                                              sResourceName,
                                              sTaskName,
                                              sProgClass,
                                              sProgInstName,
                                              bPosFirst);
}

//------------------------------------------------------------------*
/**
 * RemoveLinkProg
 *
 */
void CLCGenObj::RemoveLinkProg(ICEProjectManager* pPM, const CString& strProjPath, CComQIPtr<ICEIECResourceOp>& rpResourceOp)
{
    CComQIPtr<ICEProjInfo> pProjInfo(pPM);
    if (!rpResourceOp || !pProjInfo)
    {
        return;
    }

    HRESULT hr;
    CComBSTR sResourceName(m_strFile);
    CComBSTR sProgInstName(m_strName);
    
    hr = rpResourceOp->RemoveProgramInstance(pProjInfo,
                                             sResourceName,
                                             sProgInstName);
}
