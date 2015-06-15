
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "dp_slot.h"
#include "gsdmessage.h"
//----  Local Defines:   -------------------------------------------*


CDP_Slot::CDP_Slot()
: m_strName(), m_ModRefList(), m_Modules()
{
    m_lSlotNo = DEFAULT;
    m_lDefRef = DEFAULT;
    m_pDefaultModule = NULL;
}

CDP_Slot::~CDP_Slot()
{}

void CDP_Slot::FinalRelease()
{
    FC_RELEASE_PTR(m_pDefaultModule);
    m_Modules.RemoveAll();
    m_ModRefList.RemoveAll();
}

void CDP_Slot::AddModuleReferences(CList<long,long> * pList)
{
    m_ModRefList.RemoveAll();
    m_ModRefList.AddTail(pList);
}

void CDP_Slot::SlotAssignment(CStationCallback* pCB)
{
    CString strMsg;
    //start with default module
    IModule * pModule = pCB->LookupModule(m_lDefRef); //should be addref'd
    if(!pModule) {
        /* error message - Could not find module with reference '%1'
           for assignment to slot '%2'
        */
        GSD_FormatMessage(strMsg, E_GSD_MODULE_ASSIGNMENT,(int)m_lDefRef,(LPCTSTR)m_strName);
        pCB->Message(E_GSD_MODULE_ASSIGNMENT,strMsg);
    }

    m_pDefaultModule = pModule;

    //now process the list of mountable modules
    POSITION pos = NULL;
    long lModRef = 0;
    for(pos = m_ModRefList.GetHeadPosition(); pos != NULL;) {
        lModRef = m_ModRefList.GetNext(pos);

        IModule * pModule = pCB->LookupModule(lModRef); //should be addref'd
        if(!pModule) {
            /*  error message - Could not find module with reference '%1'
                for assignment to slot '%2'
            */
            GSD_FormatMessage(strMsg, E_GSD_MODULE_ASSIGNMENT,(int)lModRef,(LPCTSTR)m_strName);
            pCB->Message(E_GSD_MODULE_ASSIGNMENT,strMsg);
            continue;
        }
        //here add to list of modules.
        m_Modules.Add(pModule);
    }
}

STDMETHODIMP CDP_Slot::get_SlotNumber(LONG * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lSlotNo;
	return S_OK;
}

STDMETHODIMP CDP_Slot::get_SlotName(BSTR * psVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(psVal,E_POINTER);
    return ::AllocSysString(psVal,m_strName);
}

STDMETHODIMP CDP_Slot::get_DefaultModule(IModule ** pp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
	*pp = NULL;
    if(m_pDefaultModule) {
        *pp = m_pDefaultModule;
        (*pp)->AddRef();
    }
	return S_OK;
}

STDMETHODIMP CDP_Slot::get_ModuleCollection(IModuleCollection ** pp)
{
    int iCount,i;
    CComObject<ModuleCollection> *pc = NULL;
    IModule *pModule = NULL;
    HRESULT hr = S_OK,hrLoop;
    
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<ModuleCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_Modules.GetCount();

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            IModule * pModule = m_Modules[i]; //makes an addref.
            if(!pModule) {
                _ASSERTE(pModule);
                continue;
            }

            hrLoop = pc->Insert(i,pModule);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pModule);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

