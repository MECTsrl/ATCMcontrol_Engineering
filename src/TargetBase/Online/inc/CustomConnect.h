/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CustomConnect.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CustomConnect.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CustomConnect.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef __CUSTOM_CONNECT_H_
#define __CUSTOM_CONNECT_H_

//------------------------------------------------------------------*
/**
*  class CCustomConnect
*
*  @see    ....
*/
class ATL_NO_VTABLE CCustomConnect : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCustomConnect, &CLSID_CustomConnect>,
	public IDispatchImpl<ICustomConnect, &IID_ICustomConnect, &LIBID_CSC_OnlineServer, 2, 0>
{
// =Construction
public:
	                        CCustomConnect();
                                // default ctor
    virtual                 ~CCustomConnect();
                                // dtor

private:
                            CCustomConnect(CCustomConnect const & other);
                                // copy ctor
    CCustomConnect const &    operator = (CCustomConnect const & other);
                                // assignment


//--------------------------
// =Interface map
BEGIN_COM_MAP(CCustomConnect)
	COM_INTERFACE_ENTRY(ICustomConnect)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// =Access
public:

// =Operations
public:
DECLARE_REGISTRY_RESOURCEID(IDR_CUSTOM_CONNECT)

// =Helpers
//--------------------------
// =Interface
// ICustomConnect
public:
	STDMETHOD(CreateVarSession)(/*[in, string]*/ BSTR sInit, /*[out]*/ IUnknown **ppOPCServer);
	STDMETHOD(CreateCommissioningMgt)(/*[in, string]*/ BSTR sInit, /*[out]*/ IUnknown **ppCommissionMgt);
	STDMETHOD(CreateControlManagement)(/*[in, string]*/ BSTR sInit, /*[out]*/ IUnknown **ppControlMgt);

// =Attributes
protected:

private:
    static CCriticalSection         cs;

};

#endif

/* ---------------------------------------------------------------------------- */
