              

#include "stdafx.h"

#include "ErrorCodes.h"
#include "KadManager.h"
#include "KadManagerComObj.h"
#include "ComObj.h"
#include "TypeComObj.h"
#include "fc_xml.h"

CKadAccess kadAccess;

//------------------------------------------------------------------*
/**
 * creates a new object that handles the access to the KAD data
 * stored in a XML file.
 *
 * @see    KAD description - section ATCM KAD in 4C_Interfaces.doc         
*/
CKadAccess::CKadAccess()
{
	m_pTreeRoot = NULL;
    m_bInitialized = FALSE;
}

//------------------------------------------------------------------*
/**
 * deletes the object that handles the access to the KAD data.
 * stored in a XML file.
*/
CKadAccess::~CKadAccess()
{
	if (m_pTreeRoot != NULL) {
		m_pTreeRoot->Release();
	}
}

//------------------------------------------------------------------*
/**
 * initializes the KAD access handler.
 * the initialization should be done when the object is used for the 
 * first time, not when it is created (because one static KAD access
 * object is used the initialization would be performed too early then)
*/
HRESULT CKadAccess::Init()
{
	HRESULT hrResult;
	IXMLDOMDocument *pDocument;

    INTEGRITY_DOMAIN

    if (m_bInitialized) {
        hrResult = S_OK;
    } else {
        RemergeKAD();
        m_bInitialized = TRUE;
        pDocument = NULL;
	    hrResult = XML_LoadDocument(_bstr_t("4ceng.kad"),&pDocument);
	    if (hrResult == S_OK) {
		    hrResult = GetFirstSubNode(L"KAD4CE",pDocument,&m_pTreeRoot);
        }
	    if (hrResult != S_OK) {
            if (pDocument != NULL) {
                pDocument->Release();
            }
	    }
    }
    return hrResult;
}

HRESULT CKadAccess::StringToGuid(BSTR sGuidString, GUID *pGuid)
// converts a GUID string into a GUID and deletes the string
{
    HRESULT hrResult;

    hrResult = CLSIDFromString(sGuidString,pGuid);
    SysFreeString(sGuidString);
    return hrResult;
}

HRESULT CKadAccess::InitAndQueryInterface(IUnknown *pUnknown, IXMLDOMNode *pNode,
    IID interfaceId, void **ppInterface)
// initializes the reference to a XML node of an object and returns the
// requested interface pointer
{
    HRESULT hrResult;
    IXmlNodeRef *pXmlNodeRef;

    try {
        *ppInterface = NULL;
        pXmlNodeRef = NULL;
        hrResult = pUnknown->QueryInterface(IID_IXmlNodeRef,
            (void **) &pXmlNodeRef);
        if (hrResult == S_OK) {
            pXmlNodeRef->SetXmlNode(pNode);
            pXmlNodeRef->Release();
            hrResult = pUnknown->QueryInterface(interfaceId,ppInterface);
        }
    }
    catch (...) {
        hrResult = E_XML_EXCEPTION_OCCURED;
    }
    pUnknown->Release();
    return hrResult;
}

HRESULT CKadAccess::GetFirstSubNode(BSTR sNodeName, IXMLDOMNode *pParentNode,
    IXMLDOMNode **ppChildNode)
// returns the first sub-node with the given name
{
    HRESULT hrResult;

    if(pParentNode == NULL)
    {
        pParentNode = m_pTreeRoot;
    }

    hrResult = XML_GetFirstSubNode(sNodeName,pParentNode,ppChildNode);
    switch (hrResult) {
        case S_OK:
            break;
        case S_XML_NODE_NOT_FOUND:
            hrResult = S_FALSE;
            break;
        default:
            hrResult = E_FAILED_TO_ACCESS_XML;
    }
    return hrResult;
}

HRESULT CKadAccess::GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
	BSTR *psAttributeValue)
// returns the value of an attribute
{
    return XML_GetAttribute(sAttributeName,pNode,psAttributeValue);
}

HRESULT CKadAccess::GetSubNode(BSTR sSubNodeName, IXMLDOMNode **ppSubNode)
// returns the node specified by the given node name
{
    HRESULT hrResult;

    *ppSubNode = NULL;
    hrResult = Init();
	if (hrResult == S_OK) {
        hrResult = GetFirstSubNode(sSubNodeName,m_pTreeRoot,ppSubNode);
    }
    return hrResult;
}

HRESULT CKadAccess::GetNextSiblingNode(IXMLDOMNode *pNode, BSTR sNodeName,
    IXMLDOMNode **ppSiblingNode)
// gets the next sibling node with the specified node name
{
    HRESULT hrResult;

    hrResult = XML_GetNextSiblingNode(pNode,sNodeName,ppSiblingNode);
    switch (hrResult) {
        case S_OK:
            break;
        case S_XML_NODE_NOT_FOUND:
            hrResult = S_FALSE;
            break;
        default:
            hrResult = E_FAILED_TO_ACCESS_XML;
    }
    return hrResult;
}

HRESULT CKadAccess::GetSubNodeByAttribute(IXMLDOMNode *pNode, BSTR sSubNodeName,
    BSTR sAttributeName, BSTR sAttributeValue, IXMLDOMNode **ppSubNode)
// gets a sub node that has the specified attribute value
{
    BOOL bDone;
    BSTR sValue;
	HRESULT hrResult;
    IXMLDOMNode *pSubNode,*pSiblingNode;

    *ppSubNode = NULL;
    hrResult = GetFirstSubNode(sSubNodeName,pNode,&pSubNode);
    bDone = (hrResult != S_OK);
    while (! bDone) {
        hrResult = GetAttribute(sAttributeName,pSubNode,&sValue);
        if (hrResult == S_OK) {
            if (wcscmp(sAttributeValue,sValue) == 0) {
                bDone = TRUE;    
                *ppSubNode = pSubNode;
                pSubNode = NULL;
            } else {
                hrResult = GetNextSiblingNode(pSubNode,sSubNodeName,
                    &pSiblingNode);
                pSubNode->Release();
                if (hrResult == S_OK) {
                    pSubNode = pSiblingNode;
                } else {
                    pSubNode = NULL;
                }
            }
            SysFreeString(sValue);
        }
        if (hrResult != S_OK) {
            bDone = TRUE;
        }
    }
    if (pSubNode != NULL) {
        pSubNode->Release();
    }
    return hrResult;
}

HRESULT CKadAccess::GetSubSubNodeByAttribute(BSTR sSubNodeName, 
    BSTR sSubSubNodeName, BSTR sAttributeName, BSTR sAttributeValue,
    IXMLDOMNode **ppSubSubNode)
// gets a sub node of a sub node that has the specified attribute value
{
	HRESULT hrResult;
    IXMLDOMNode *pSubNode;

    *ppSubSubNode = NULL;
    hrResult = Init();
	if (hrResult == S_OK) {
        hrResult = GetFirstSubNode(sSubNodeName,m_pTreeRoot,&pSubNode);
    }
    if (hrResult == S_OK) {
        hrResult = GetSubNodeByAttribute(pSubNode,sSubSubNodeName,
            sAttributeName,sAttributeValue,ppSubSubNode);
        pSubNode->Release();
    }
    return hrResult;
}
 
HRESULT CKadAccess::GetSubNodeAttribute(BSTR sNodeName, BSTR sAttributeName,
  BSTR *psAttributeValue)
// returns the value of a string attribute of the specified node
{
	BSTR sValue;
	HRESULT hrResult;
	IXMLDOMNode *pSubNode;

    pSubNode = NULL;
    hrResult = Init();
	if (hrResult == S_OK) {
    	hrResult = GetFirstSubNode(sNodeName,m_pTreeRoot,&pSubNode);
    }
	if (hrResult == S_OK) {
		hrResult = GetAttribute(sAttributeName,pSubNode,&sValue);
    }
	if (hrResult == S_OK) {
		*psAttributeValue = sValue;
	}
    if (pSubNode != NULL) {
        pSubNode->Release();
    }
	return hrResult;
}

HRESULT CKadAccess::GetSubNodeAttribute(BSTR sNodeName, BSTR sAttributeName,
  GUID *pAttributeValue)
// returns the value of a GUID attribute of the specified node
{
    BSTR sValue;
	HRESULT hrResult;

    hrResult = GetSubNodeAttribute(sNodeName,sAttributeName,&sValue);
    if (hrResult == S_OK) {
        hrResult = StringToGuid(sValue,pAttributeValue);
    }
	return hrResult;
}

HRESULT CKadAccess::GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
	GUID *pAttributeValue) 
// gets the value of a GUID attribute at the given node
{
    BSTR sValue;
	HRESULT hrResult;

    KADMANAGER_ASSERT(pNode != NULL);
    KADMANAGER_ASSERT(pAttributeValue != NULL);

    hrResult = GetAttribute(sAttributeName,pNode,&sValue);
    if (hrResult == S_OK) {
        hrResult = StringToGuid(sValue,pAttributeValue);
    }
	return hrResult;
}

HRESULT CKadAccess::GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
	BOOL *pbAttributeValue)
// gets the value of a boolean attribute at the given node
{
    BSTR sValue;
	HRESULT hrResult;

    KADMANAGER_ASSERT(pNode != NULL);
    KADMANAGER_ASSERT(pbAttributeValue != NULL);

    hrResult = GetAttribute(sAttributeName,pNode,&sValue);
    if (hrResult == S_OK) {
        *pbAttributeValue = (wcscmp(sValue,L"0") != 0);
        SysFreeString(sValue);
    }
	return hrResult;
}

HRESULT CKadAccess::GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
	WORD *pAttributeValue) 
// gets the value of a FACILITYCODE attribute at the given node
{
    BSTR sValue;
	HRESULT hrResult;

    KADMANAGER_ASSERT(pNode != NULL);
    KADMANAGER_ASSERT(pAttributeValue != NULL);

    hrResult = GetAttribute(sAttributeName,pNode,&sValue);
    if (hrResult == S_OK) 
    {
        //converting the BSTR sValue (must be HEX) to WORD
        WORD AttributeValue;
        swscanf(sValue, L"%x", &AttributeValue);
        ::SysFreeString(sValue);
        if(AttributeValue == 0)
        {
            return hrResult = E_FAIL;
        }
        *pAttributeValue = AttributeValue;
    }
	return hrResult;
}

HRESULT CKadAccess::GetSubNodeType(BSTR nodeName, IType **ppIType)
// gets a type interface corresponding to the type attribute of a sub-node
{
    HRESULT hrResult;
    BSTR sType;

    if (ppIType == NULL) {
        hrResult = E_POINTER;
    } else {
        *ppIType = NULL;
        hrResult = GetSubNodeAttribute(nodeName,L"TYPE",&sType);
        if (hrResult == S_OK) {
            hrResult = TFGetSubSubNodeByAttribute<CTypeComObj,IType>(
                IID_IType,L"TYPES",L"TYPE",L"ID",sType,ppIType);            
            SysFreeString(sType);
        }
    }
    return hrResult;
}

HRESULT CKadAccess::GetNodeType(IXMLDOMNode *pNode, IType **ppIType)
// gets a type interface corresponding to the type attribute of a node
{
    HRESULT hrResult;
    BSTR sType;

    if (ppIType == NULL) {
        hrResult = E_POINTER;
    } else {
        *ppIType = NULL;
        hrResult = GetAttribute(L"TYPE",pNode,&sType);
        if (hrResult == S_OK) {
            hrResult = TFGetSubSubNodeByAttribute<CTypeComObj,IType>(
                IID_IType,L"TYPES",L"TYPE",L"ID",sType,ppIType);            
            SysFreeString(sType);
        }
    }
    return hrResult;
}



// ***************************************************************
// some special things: use KADMerger.dll to merge KADs each time
// the engineering is started.

extern "C" __declspec(dllimport) long __stdcall Get4CPath(LPSTR pszFile);
extern "C" __declspec(dllimport) long __stdcall MergeKAD_KnieFK(LPSTR pszProjectPath, 
                                                      LPSTR pszKADFileName, 
                                                      LPSTR pszAddOnFileName, 
                                                      BOOL bOverwrite, 
                                                      LPSTR pszLogFile, 
                                                      BOOL bRemove);



HRESULT CKadAccess::RemergeKAD()
{
    int i;
    char pszInstallDir[MAX_PATH];
    char pszKadName[MAX_PATH];
    char pszOrigKadName[MAX_PATH];
    char pszBackKadName[MAX_PATH];
    char pszAddKAD[MAX_PATH];
    char pszAddKADFullPath[MAX_PATH];
    char pszLogFile[MAX_PATH];
    char pszFindFilePattern[MAX_PATH];
    BOOL bError = FALSE;

    // hack fill with something: KAD merge Dll test for strlen of parameter
    for(i=0; i<MAX_PATH; i++)
    {
        pszInstallDir[i] = 1;
    }
    pszInstallDir[MAX_PATH-1] = 0;
    if (Get4CPath(pszInstallDir)!=TRUE)
    {
        //printf("Error: Looking up ATCMControl installation path!\n");
        return E_FAIL;
    }
    
    // check for nomerge.kad file
    char pszNoMerge[MAX_PATH];
    strcpy(pszNoMerge, pszInstallDir);
    strcat(pszNoMerge, _T("Engineering\\bin\\nomerge.kad"));

    FILE *f = NULL;
    f = fopen(pszNoMerge, _T("r") );
    if (f!=NULL)
    {
        fclose(f);
        return S_OK;
    }




    // ************************ 4ceng.kad  *************************
    strcpy(pszOrigKadName, pszInstallDir);
    strcat(pszOrigKadName, "Engineering\\bin\\4ceng.kad");
    
    strcpy(pszBackKadName, pszOrigKadName);
    strcat(pszBackKadName, ".back");

    DeleteFile(pszBackKadName);
    if (!MoveFile(pszOrigKadName, pszBackKadName))
    {
        //printf("Could not create backup of 4ceng.kad\n");
    }

    //printf("Merging 4ceng.kad\n");

    strcpy(pszKadName, "Engineering\\bin\\4ceng.kad");
    strcpy(pszLogFile, "KadMerge.log");


    HANDLE hFind;
    WIN32_FIND_DATA findFileData;
    BOOL bFound = TRUE;

    strcpy(pszFindFilePattern, pszInstallDir);
    strcat(pszFindFilePattern, "Engineering\\bin\\*_4ceng.kad");


    // start with Kernel_IEC_4ceng.kad and skip this later
    strcpy(pszAddKAD, "Kernel_IEC_4ceng.kad");
    strcpy(pszAddKADFullPath, pszInstallDir);
    strcat(pszAddKADFullPath, "Engineering\\bin\\");
    strcat(pszAddKADFullPath, pszAddKAD);
    //printf("Merging KAD: %s\n", pszAddKAD);
    if (MergeKAD_KnieFK(pszInstallDir, 
                         pszKadName, 
                         pszAddKADFullPath, 
                         TRUE, 
                         pszLogFile, 
                         FALSE)  != TRUE)
    {
        //printf("Error: Merging KAD %s!\n", pszAddKAD);
        bError = TRUE;
    }
  
    hFind = FindFirstFile(pszFindFilePattern, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        while (bFound)
        {
            strcpy(pszAddKAD, findFileData.cFileName);

            if (stricmp(pszAddKAD, "Kernel_IEC_4ceng.kad")!=0)
            {
                strcpy(pszAddKADFullPath, pszInstallDir);
                strcat(pszAddKADFullPath, "Engineering\\bin\\");
                strcat(pszAddKADFullPath, pszAddKAD);
                //printf("Merging KAD: %s\n", pszAddKAD);
                if (MergeKAD_KnieFK(pszInstallDir, 
                                     pszKadName, 
                                     pszAddKADFullPath, 
                                     TRUE, 
                                     pszLogFile, 
                                     FALSE)  != TRUE)
                {
                    //printf("Error: Merging KAD %s!\n", pszAddKAD);
                    bError = TRUE;
                }
            }
            bFound = FindNextFile(hFind, &findFileData);
        }
        FindClose(hFind);
    }



    // ************************ 4cg.kad  *************************
    strcpy(pszOrigKadName, pszInstallDir);
    strcat(pszOrigKadName, "Engineering\\bin\\4cg.kad");
    
    strcpy(pszBackKadName, pszOrigKadName);
    strcat(pszBackKadName, ".back");

    DeleteFile(pszBackKadName);
    if (!MoveFile(pszOrigKadName, pszBackKadName))
    {
        printf("Could not create backup of 4ceng.kad\n");
    }

    printf("\nRemerging 4cg.kad\n");

    strcpy(pszKadName, "Engineering\\bin\\4cg.kad");
    strcpy(pszLogFile, "KadMerge_CG.log");


    bFound = TRUE;

    strcpy(pszFindFilePattern, pszInstallDir);
    strcat(pszFindFilePattern, "Engineering\\bin\\*_4cg.kad");
  
    hFind = FindFirstFile(pszFindFilePattern, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        while (bFound)
        {
            strcpy(pszAddKAD, findFileData.cFileName);

            strcpy(pszAddKADFullPath, pszInstallDir);
            strcat(pszAddKADFullPath, "Engineering\\bin\\");
            strcat(pszAddKADFullPath, pszAddKAD);
            //printf("Merging KAD: %s\n", pszAddKAD);
            if (MergeKAD_KnieFK(pszInstallDir, 
                                 pszKadName, 
                                 pszAddKADFullPath, 
                                 TRUE, 
                                 pszLogFile, 
                                 FALSE)  != TRUE)
            {
                //printf("Error: Merging KAD %s!\n", pszAddKAD);
                bError = TRUE;
            }
            bFound = FindNextFile(hFind, &findFileData);
        }
        FindClose(hFind);
    }

    //printf("\n");
    if (bError)
    {
        //printf("*************\n"
        //       " Errors during KAD remerging,\n"
        //       " the old KAD files are still available (4cg|4ceng).kad.back,\n"
        //       " please rename to original file names.\n\n");
    }
    else
    {
        //printf("KADs successfully remerged!\n");
    }
    //printf("\n");

    return S_OK;
}


