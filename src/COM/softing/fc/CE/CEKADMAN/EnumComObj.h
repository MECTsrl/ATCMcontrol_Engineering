#ifndef ENUM_COM_OBJ_H
#define ENUM_COM_OBJ_H

#include "ComObj.h"
#include "EnumData.h"
#include "ErrorCodes.h"
#include "KadManager.h"

template <class enumInterface, class enumElementInterface, class enumElementClass> 
class ATL_NO_VTABLE TCEnumComObj :
    public CComObjectRootEx<CComSingleThreadModel>,
    public enumInterface,
    public IInitEnum
// this template class implements an enumeration object 
{
    private:
        CEnumData m_enumData;
        BEGIN_COM_MAP(TCEnumComObj)
            COM_INTERFACE_ENTRY(enumInterface)
            COM_INTERFACE_ENTRY(IInitEnum)
        END_COM_MAP()
    public:
        DECLARE_REGISTRY_RESOURCEID(registryResourceId)
        DECLARE_PROTECT_FINAL_CONSTRUCT()
// IEnum...
  	    STDMETHOD(Next)(ULONG celt, enumElementInterface **rgelt, ULONG *pceltFetched);
	    STDMETHOD(Skip)(ULONG celt);
	    STDMETHOD(Reset)();
	    STDMETHOD(Clone)(enumInterface **ppenum);
// IInitEnum
        STDMETHOD(InitEnum)(GUID enumElementInterfaceId, IUnknown *pNode, 
    		BSTR sSubNodeName);
};

template <class enumInterface, class enumElementInterface, class enumElementClass> 
STDMETHODIMP TCEnumComObj<enumInterface,enumElementInterface,enumElementClass>::
    InitEnum(GUID enumElementInterfaceId,IUnknown *pNode, BSTR sSubNodeName)
// this function initializes the enumeration object
{
    m_enumData.Init(enumElementInterfaceId,(IXMLDOMNode *) pNode,
        sSubNodeName);
    return S_OK;
}

template <class enumInterface, class enumElementInterface, class enumElementClass> 
STDMETHODIMP TCEnumComObj<enumInterface,enumElementInterface,enumElementClass>::
    Next(ULONG celt, enumElementInterface **rgelt, ULONG *pceltFetched)
// this function fills a given table with the enumerated objects
{
    ULONG ulCount,ulFetched;
    HRESULT hrResult;
    IXMLDOMNode *pNextNode;
    enumElementInterface *pEnumElement;

    if ((rgelt == NULL) || (pceltFetched == NULL)) {
		hrResult = E_POINTER;
    } else if((pceltFetched == NULL) && (celt != 1)) {
        hrResult = E_INVALIDARG;
    } else {
        hrResult = S_OK;
        *pceltFetched = 0;
        for (ulCount=0; ulCount<celt; ulCount++) {
            rgelt[ulCount] = NULL;
        }
        ulFetched = 0;
        while ((ulFetched < celt) && (hrResult == S_OK)) {
            hrResult = m_enumData.GetNextNode(&pNextNode);
            if (hrResult == S_OK) {
                hrResult = TFCreateAndInitReturnObject<
                        enumElementClass,enumElementInterface>(
                    m_enumData.GetElementInterfaceId(),pNextNode,
                    &pEnumElement,FALSE);
                if (hrResult == S_OK) {
                    rgelt[ulFetched] = pEnumElement;
                    ulFetched++;
                }
                pNextNode->Release();
            }
        }
        switch (hrResult) {
            case S_OK:
            case S_FALSE:
                *pceltFetched = ulFetched;
                hrResult = (ulFetched == celt) ? S_OK : S_FALSE ;
                break;
            default:
                for (ulCount=0; ulCount<celt; ulCount++) {
                    if (rgelt[ulFetched] != NULL) {
                        rgelt[ulFetched]->Release();
                        rgelt[ulFetched] = NULL;
                    }
                }
        }
    }
    return hrResult;
}

template <class enumInterface, class enumElementInterface, class enumElementClass> 
STDMETHODIMP TCEnumComObj<enumInterface,enumElementInterface,enumElementClass>::
    Skip(ULONG celt)
// this function skips a given number of entries, it is not implemented yet
{
	return E_NOTIMPL;
}

template <class enumInterface, class enumElementInterface, class enumElementClass> 
STDMETHODIMP TCEnumComObj<enumInterface,enumElementInterface,enumElementClass>::
    Reset()
// this function resets the enumerator
{
    m_enumData.Reset();
    return S_OK;
}

template <class enumInterface, class enumElementInterface, class enumElementClass> 
STDMETHODIMP TCEnumComObj<enumInterface,enumElementInterface,enumElementClass>::
    Clone(enumInterface **ppenum)
// this function clones the enumerator, it is not implemented yet
{
    HRESULT hrResult;

    if (ppenum == NULL) {
        hrResult = E_POINTER;
    } else {
        hrResult = E_NOTIMPL;
    }
	return hrResult;
}

#endif