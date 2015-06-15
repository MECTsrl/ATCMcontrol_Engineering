#include "stdafx.h"

#include <atlbase.h>

#include "WizardList.h"
#include "CEProjWiz.h"

#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"

/////////////////////////////////////////////////////////////////////////

CWizardListEntry::CWizardListEntry(CLSID classId, CString sName, 
    CString sDescription)
// creates a new wizard list entry describing one project wizard
{
    m_classId = classId;
    m_sName = sName;
    m_sDescription = sDescription;
}

CLSID CWizardListEntry::GetClassId()
// returns the class ID of the project wizard
{
    return m_classId;
}

CString CWizardListEntry::GetName()
// returns the name of the project wizard
{
    return m_sName;
}

CString CWizardListEntry::GetDescription()
// returns a textual description of the project wizard
{
    return m_sDescription;
}

/////////////////////////////////////////////////////////////////////////

CWizardList::CWizardList()
// creates a new project wizard list 
{
    position = NULL;
}

CWizardList::~CWizardList()
// deletes a project wizard list 
{
    CWizardListEntry *pEntry;

    position = GetHeadPosition();
    while (position != NULL) {
        pEntry = (CWizardListEntry *) CPtrList::GetNext(position);
        delete pEntry;
    }
}

void CWizardList::Init()
// fills the wizard list with all the COM objects implementing a project wizard
{
    SearchWizards();
}

HRESULT CWizardList::AddWizardToList(CLSID classId)
// adds a new wizard to the list
{
    HRESULT hrResult;
    BSTR sName = NULL;
    BSTR sDescription = NULL;
    ICEProjectWizard *pProjectWizard;

    hrResult = CoCreateInstance(classId,0,CLSCTX_ALL,IID_ICEProjectWizard,
        (void**) &pProjectWizard);
    if (SUCCEEDED(hrResult)) {
        hrResult = pProjectWizard->GetDescription(LANG_ENGLISH,&sName,
            &sDescription);
        if (hrResult == S_OK) {
            AddTail(new CWizardListEntry(classId,CString(sName),
                CString(sDescription)));
            SysFreeString(sName);
            SysFreeString(sDescription);
        }
        pProjectWizard->Release();
    }
    return hrResult;
}

void CWizardList::SearchWizards()
// searches all wizards by searching for COM objects implementing
// the project wizard category
{
    HRESULT hr;
    CLSID classId;
    ULONG ulNumberIds;
    IEnumCLSID *pEnumClsid;
    ICatInformation *pCatInformation;
    CATID requiredCategoryIds[1];

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,0,CLSCTX_ALL,
        IID_ICatInformation,(void**) &pCatInformation);
    if (SUCCEEDED(hr)) {
        requiredCategoryIds[0] = CATID_4CPrjWizards;
        hr = pCatInformation->EnumClassesOfCategories(
            sizeof(requiredCategoryIds)/sizeof(*requiredCategoryIds),
            requiredCategoryIds,0,NULL,&pEnumClsid);
        if (SUCCEEDED(hr)) {
            hr = pEnumClsid->Reset();
            while (hr == S_OK) {
                hr = pEnumClsid->Next(1,&classId,&ulNumberIds);
                if (hr == S_OK) {
                    if (ulNumberIds == 1) {
                        HRESULT hrAdd = AddWizardToList(classId);
                    } else {
                        hr = S_FALSE;
                    }
                }
            }
            pEnumClsid->Release();
        }
        pCatInformation->Release();
    }
}

CWizardListEntry *CWizardList::GetFirst()
// gets the first wizard of the list
{
    position = GetHeadPosition();
    return GetNext();
}

CWizardListEntry *CWizardList::GetNext()
// gets the next wizard of the list
{
    CWizardListEntry *pEntry;

    if (position != NULL) {
        pEntry = (CWizardListEntry *) CPtrList::GetNext(position);
    } else {
        pEntry = NULL;
    }
    return pEntry;
}

CWizardListEntry *CWizardList::GetEntry(CString sName)
// returns the entry of the wizard list with the given name
{
    BOOL bFound;
    CWizardListEntry *pEntry;

    bFound = FALSE;
    pEntry = GetFirst();
    while ((pEntry != NULL) && (! bFound)) {
        if (pEntry->GetName().Compare(sName) == 0) {
            bFound = TRUE;
        } else {
            pEntry = GetNext();
        }
    }
    return pEntry;
}

/////////////////////////////////////////////////////////////////////////


