
// CEBackparseComObj.cpp : Implementation of CCEBackparseComObj
#include "stdafx.h"

#include <atlbase.h>

#include "CEBackparserCO.h"
#include "CEBackparseComObj.h"

#include "DLLInterface.h"

/////////////////////////////////////////////////////////////////////////////
// CCEBackparseComObj

STDMETHODIMP CCEBackparseComObj::doBackparse(BSTR sFileName, ICEProjInfo *pICEProjInfo, BSTR *psXmlText)
{
    HRESULT hr = S_OK;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
    FC_PCHECK_HR(sFileName,E_POINTER);
    FC_PCHECK_HR(pICEProjInfo,E_POINTER);
    FC_PCHECK_HR(psXmlText,E_POINTER);

    try{
        CComBSTR s;
        CString str;
        BackparserProjectInfo info;
        // convert types
        string FileName;
        string XmlText;
        
        pICEProjInfo->getProjectPath(&s);
        str = s;
        info.m_strProjectPath = str;

        str.Empty();
        s.Empty();

        pICEProjInfo->getProjectName(&s);
        str = s;
        info.m_strProjectName = str;

        str.Empty();
        s.Empty();

        FileName = CString(sFileName);
        ::doBackparse(FileName, XmlText,info);

        // convert types
        CString cs(XmlText.data());
        *psXmlText = cs.AllocSysString();
    }
    catch(HRESULT hrRes) {
        hr = hrRes;
    }
    catch(CMemoryException* e) {
        hr = E_OUTOFMEMORY;
        e->Delete();
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    return hr;
}
