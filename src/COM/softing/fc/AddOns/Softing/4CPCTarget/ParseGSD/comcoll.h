#ifndef __COMCOLL_H_
#define __COMCOLL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class CComInterfaceColl
 *  IDispatch based collection interface template - used to implement
 *  standard OLE Automation compliant collections. The enumerator 
 *  exposes IEnumVARIANT interface
*/
template <class T, const IID* piid,class InterfaceType,
const GUID* plibid, WORD wMajor = 1,WORD wMinor = 0>
class CComInterfaceColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
    public IDispatchImpl<T, piid, plibid,wMajor,wMinor>
{
public:
    typedef CComEnum<IEnumVARIANT,&IID_IEnumVARIANT, 
        VARIANT, _Copy<VARIANT>,CComObjectThreadModel> CComEnumVariant;

    CComInterfaceColl()
    {
        m_size = 0;
        m_pvt = NULL;
    }

    virtual ~CComInterfaceColl() 
    {
        Cleanup();
    }

    void FinalRelease()
    {
        Cleanup();
    }

//--------------------------
// =Interface map
BEGIN_COM_MAP(CComInterfaceColl)
COM_INTERFACE_ENTRY(IDispatch)
COM_INTERFACE_ENTRY(T)
END_COM_MAP()

    STDMETHOD(Initialize)(size_t size, InterfaceType** ppArray = NULL)
    {    
        size_t i;
        InterfaceType *p = NULL;
        IUnknown* pUnk = NULL;
        HRESULT hr = S_OK;

        if(m_pvt) {
            //already initialized
            return S_FALSE;
        }

        m_size = size;
        //allocate interface ptr array.
        m_pvt = new VARIANT [m_size+1];
        if(!m_pvt) {
            return E_OUTOFMEMORY;
        }

        memset(m_pvt,0,sizeof(VARIANT) * (m_size+1));

        for(i = 0;i<m_size+1;i++) {
            ::VariantInit(&m_pvt[i]);
        }
        
        //array to set into collection can be NULL, e.g. if Insert is used.
        if(!ppArray) {
            return hr;
        }

        for(i= 0;i<m_size;i++) {
            p = ppArray[i];
            _ASSERTE(p);
            //QI makes addref, ::VariantClear() will release
            hr = p->QueryInterface(IID_IUnknown,(void**)&pUnk);
            if(FAILED(hr)) {
                return hr;
            }

            m_pvt[i].vt = VT_UNKNOWN;
            m_pvt[i].punkVal = pUnk;
        }

        return hr;
    }

    STDMETHOD(Insert)(long Index, InterfaceType* pInterface)
    {
        IUnknown* pUnk = NULL;
        IUnknown* pTemp = NULL;

        HRESULT hr = S_OK;

		if (pInterface == NULL)
			return E_POINTER;

        if(Index < 0 || Index > (long)m_size) {
            return E_INVALIDARG;
        }

        //this add's a reference
        hr = pInterface->QueryInterface(IID_IUnknown,(void**)&pUnk);
        if(FAILED(hr)) {
            return hr;
        }

        pTemp = m_pvt[Index].punkVal; //store old entry
        m_pvt[Index].vt = VT_UNKNOWN;
        m_pvt[Index].punkVal = pUnk;  //set new interface ptr to entry

        if(pTemp) { //old entry occupied ?
            //yes, release old entry
            pTemp->Release();
        }

        return hr;
    }

	STDMETHOD(get_Count)(long* pcount)
	{
		if (pcount == NULL)
			return E_POINTER;
		*pcount = m_size;
		return S_OK;
	}

	STDMETHOD(get_Item)(long Index, InterfaceType ** ppvar)
	{
        IUnknown* pUnk = NULL;
        HRESULT hr = E_FAIL;

		//Index is 1-based
		if (ppvar == NULL)
			return E_POINTER;

        *ppvar = NULL;
        Index--;

        if(Index < 0 || Index >= (long)m_size) {
            return E_INVALIDARG;
        }

        pUnk = m_pvt[Index].punkVal;
        if(pUnk) {
            hr = pUnk->QueryInterface(__uuidof(*ppvar),(void**)ppvar);
        }
		return hr;
	}

	STDMETHOD(get__newEnum)(IUnknown** ppUnk)
	{
		if (ppUnk == NULL)
			return E_POINTER;
		*ppUnk = NULL;
		HRESULT hRes = S_OK;
		CComObject<CComEnumVariant>* p;
		hRes = CComObject<CComEnumVariant>::CreateInstance(&p);
		if (SUCCEEDED(hRes))
		{
			hRes = p->Init(&m_pvt[0],&m_pvt[m_size],0,AtlFlagCopy);
			if (hRes == S_OK)
				hRes = p->QueryInterface(IID_IUnknown, (void**)ppUnk);
            
		}
		if (hRes != S_OK)
			delete p;
		return hRes;
	}

private:
//operations
    void Cleanup()
    {
        if(m_pvt) {
            for(size_t i= 0;i<m_size;i++) {
                ::VariantClear(&m_pvt[i]);
            }
            
            delete [] m_pvt;
            m_pvt = NULL;
        }
    }

protected:
    size_t m_size;
    VARIANT * m_pvt;
};

//------------------------------------------------------------------*
/**
 *  class CComInterfaceColl2
 *  IDispatch based collection interface template - used to implement
 *  standard OLE Automation compliant collections. The enumerator 
 *  exposes IEnumUnknown interface
*/
template <class T, const IID* piid,class InterfaceType,
const GUID* plibid, WORD wMajor = 1,WORD wMinor = 0>
class CComInterfaceColl2 : 
	public CComObjectRootEx<CComMultiThreadModel>,
    public IDispatchImpl<T, piid, plibid,wMajor,wMinor>
{
public:
    typedef CComEnum<IEnumUnknown,&IID_IEnumUnknown, 
        IUnknown*, _CopyInterface<IUnknown>,CComObjectThreadModel> CComEnumUnk;

    CComInterfaceColl2()
    {
        m_size = 0;
        m_ppArray = NULL;
    }

    virtual ~CComInterfaceColl2() 
    {
        Cleanup();
    }

    void FinalRelease()
    {
        Cleanup();
    }

//--------------------------
// =Interface map
BEGIN_COM_MAP(CComInterfaceColl2)
COM_INTERFACE_ENTRY(IDispatch)
COM_INTERFACE_ENTRY(T)
END_COM_MAP()

    STDMETHOD(Initialize)(size_t size, InterfaceType** ppArray = NULL)
    {    
        m_size = size;
        InterfaceType *p = NULL;
        IUnknown* pUnk = NULL;
        HRESULT hr = S_OK;

        //allocate interface ptr array.
        m_ppArray = new IUnknown* [m_size+1];
        if(!m_ppArray) {
            return E_OUTOFMEMORY;
        }

        memset(m_ppArray,0,sizeof(IUnknown*) * (m_size+1));

        //array to set into collection can be NULL, e.g. if Insert is used.
        if(!ppArray) {
            return hr;
        }

        for(size_t i= 0;i<m_size;i++) {
            p = ppArray[i];
            _ASSERTE(p);
            hr = p->QueryInterface(IID_IUnknown,(void**)&pUnk);
            if(FAILED(hr)) {
                return hr;
            }

            m_ppArray[i] = pUnk;
        }

        return hr;
    }

    STDMETHOD(Insert)(long Index, InterfaceType* pInterface)
    {
        IUnknown* pUnk = NULL;
        IUnknown* pTemp = NULL;

        HRESULT hr = S_OK;

		if (pInterface == NULL)
			return E_POINTER;

        if(Index < 0 || Index > (long)m_size) {
            return E_INVALIDARG;
        }

        //this add's a reference
        hr = pInterface->QueryInterface(IID_IUnknown,(void**)&pUnk);
        if(FAILED(hr)) {
            return hr;
        }

        pTemp = m_ppArray[Index]; //store old entry
        m_ppArray[Index] = pUnk;  //set new interface ptr to entry

        if(pTemp) { //old entry occupied ?
            //yes, release old entry
            pTemp->Release();
        }

        return hr;
    }

	STDMETHOD(get_Count)(long* pcount)
	{
		if (pcount == NULL)
			return E_POINTER;
		*pcount = m_size;
		return S_OK;
	}

	STDMETHOD(get_Item)(long Index, InterfaceType ** ppvar)
	{
        IUnknown* pUnk = NULL;
        HRESULT hr = E_FAIL;

		//Index is 1-based
		if (ppvar == NULL)
			return E_POINTER;

        *ppvar = NULL;
        Index--;

        if(Index < 0 || Index >= (long)m_size) {
            return E_INVALIDARG;
        }

        pUnk = m_ppArray[Index];
        if(pUnk) {
            hr = pUnk->QueryInterface(__uuidof(*ppvar),(void**)ppvar);
        }
		return hr;
	}

	STDMETHOD(get__newEnum)(IUnknown** ppUnk)
	{
		if (ppUnk == NULL)
			return E_POINTER;
		*ppUnk = NULL;
		HRESULT hRes = S_OK;
		CComObject<CComEnumUnk>* p;
		hRes = CComObject<CComEnumUnk>::CreateInstance(&p);
		if (SUCCEEDED(hRes))
		{
			hRes = p->Init(m_ppArray,&m_ppArray[m_size],0,AtlFlagCopy);
			if (hRes == S_OK)
				hRes = p->QueryInterface(IID_IUnknown, (void**)ppUnk);
            
		}
		if (hRes != S_OK)
			delete p;
		return hRes;
	}

private:
//operations
    void Cleanup()
    {
        if(m_ppArray) {
            for(size_t i= 0;i<m_size;i++) {
                if(m_ppArray[i]) {
                    m_ppArray[i]->Release();
                    m_ppArray[i] = NULL;
                }
            }
            
            delete [] m_ppArray;
            m_ppArray = NULL;
        }
    }

protected:
    size_t m_size;
    IUnknown ** m_ppArray;
};

#endif // __COMCOLL_H_
