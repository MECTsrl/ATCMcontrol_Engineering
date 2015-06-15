/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 24 16:29:21 2015
 */
/* Compiler settings for ..\..\..\..\TargetBase\Kernel\inc\CSC_OnlineServer.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __CSC_OnlineServer_h__
#define __CSC_OnlineServer_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IConnection_FWD_DEFINED__
#define __IConnection_FWD_DEFINED__
typedef interface IConnection IConnection;
#endif 	/* __IConnection_FWD_DEFINED__ */


#ifndef __IControlMgt_FWD_DEFINED__
#define __IControlMgt_FWD_DEFINED__
typedef interface IControlMgt IControlMgt;
#endif 	/* __IControlMgt_FWD_DEFINED__ */


#ifndef __IMessages_FWD_DEFINED__
#define __IMessages_FWD_DEFINED__
typedef interface IMessages IMessages;
#endif 	/* __IMessages_FWD_DEFINED__ */


#ifndef __ILicenseMgt_FWD_DEFINED__
#define __ILicenseMgt_FWD_DEFINED__
typedef interface ILicenseMgt ILicenseMgt;
#endif 	/* __ILicenseMgt_FWD_DEFINED__ */


#ifndef ___ICERemoteMessageEvent_FWD_DEFINED__
#define ___ICERemoteMessageEvent_FWD_DEFINED__
typedef interface _ICERemoteMessageEvent _ICERemoteMessageEvent;
#endif 	/* ___ICERemoteMessageEvent_FWD_DEFINED__ */


#ifndef __ICommissioningMgt_FWD_DEFINED__
#define __ICommissioningMgt_FWD_DEFINED__
typedef interface ICommissioningMgt ICommissioningMgt;
#endif 	/* __ICommissioningMgt_FWD_DEFINED__ */


#ifndef ___FCCommissioningMgtNotification_FWD_DEFINED__
#define ___FCCommissioningMgtNotification_FWD_DEFINED__
typedef interface _FCCommissioningMgtNotification _FCCommissioningMgtNotification;
#endif 	/* ___FCCommissioningMgtNotification_FWD_DEFINED__ */


#ifndef __ITransfer_FWD_DEFINED__
#define __ITransfer_FWD_DEFINED__
typedef interface ITransfer ITransfer;
#endif 	/* __ITransfer_FWD_DEFINED__ */


#ifndef __IBreakpoint_FWD_DEFINED__
#define __IBreakpoint_FWD_DEFINED__
typedef interface IBreakpoint IBreakpoint;
#endif 	/* __IBreakpoint_FWD_DEFINED__ */


#ifndef ___FCRemoteBreakpointNotification_FWD_DEFINED__
#define ___FCRemoteBreakpointNotification_FWD_DEFINED__
typedef interface _FCRemoteBreakpointNotification _FCRemoteBreakpointNotification;
#endif 	/* ___FCRemoteBreakpointNotification_FWD_DEFINED__ */


#ifndef __IForceManagement_FWD_DEFINED__
#define __IForceManagement_FWD_DEFINED__
typedef interface IForceManagement IForceManagement;
#endif 	/* __IForceManagement_FWD_DEFINED__ */


#ifndef __ICustomConnect_FWD_DEFINED__
#define __ICustomConnect_FWD_DEFINED__
typedef interface ICustomConnect ICustomConnect;
#endif 	/* __ICustomConnect_FWD_DEFINED__ */


#ifndef __IConnection_FWD_DEFINED__
#define __IConnection_FWD_DEFINED__
typedef interface IConnection IConnection;
#endif 	/* __IConnection_FWD_DEFINED__ */


#ifndef __IControlMgt_FWD_DEFINED__
#define __IControlMgt_FWD_DEFINED__
typedef interface IControlMgt IControlMgt;
#endif 	/* __IControlMgt_FWD_DEFINED__ */


#ifndef __IMessages_FWD_DEFINED__
#define __IMessages_FWD_DEFINED__
typedef interface IMessages IMessages;
#endif 	/* __IMessages_FWD_DEFINED__ */


#ifndef __ILicenseMgt_FWD_DEFINED__
#define __ILicenseMgt_FWD_DEFINED__
typedef interface ILicenseMgt ILicenseMgt;
#endif 	/* __ILicenseMgt_FWD_DEFINED__ */


#ifndef ___ICERemoteMessageEvent_FWD_DEFINED__
#define ___ICERemoteMessageEvent_FWD_DEFINED__
typedef interface _ICERemoteMessageEvent _ICERemoteMessageEvent;
#endif 	/* ___ICERemoteMessageEvent_FWD_DEFINED__ */


#ifndef __ICommissioningMgt_FWD_DEFINED__
#define __ICommissioningMgt_FWD_DEFINED__
typedef interface ICommissioningMgt ICommissioningMgt;
#endif 	/* __ICommissioningMgt_FWD_DEFINED__ */


#ifndef ___FCCommissioningMgtNotification_FWD_DEFINED__
#define ___FCCommissioningMgtNotification_FWD_DEFINED__
typedef interface _FCCommissioningMgtNotification _FCCommissioningMgtNotification;
#endif 	/* ___FCCommissioningMgtNotification_FWD_DEFINED__ */


#ifndef __ITransfer_FWD_DEFINED__
#define __ITransfer_FWD_DEFINED__
typedef interface ITransfer ITransfer;
#endif 	/* __ITransfer_FWD_DEFINED__ */


#ifndef __IBreakpoint_FWD_DEFINED__
#define __IBreakpoint_FWD_DEFINED__
typedef interface IBreakpoint IBreakpoint;
#endif 	/* __IBreakpoint_FWD_DEFINED__ */


#ifndef __IForceManagement_FWD_DEFINED__
#define __IForceManagement_FWD_DEFINED__
typedef interface IForceManagement IForceManagement;
#endif 	/* __IForceManagement_FWD_DEFINED__ */


#ifndef ___FCRemoteBreakpointNotification_FWD_DEFINED__
#define ___FCRemoteBreakpointNotification_FWD_DEFINED__
typedef interface _FCRemoteBreakpointNotification _FCRemoteBreakpointNotification;
#endif 	/* ___FCRemoteBreakpointNotification_FWD_DEFINED__ */


#ifndef __ICustomConnect_FWD_DEFINED__
#define __ICustomConnect_FWD_DEFINED__
typedef interface ICustomConnect ICustomConnect;
#endif 	/* __ICustomConnect_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_CSC_OnlineServer_0000 */
/* [local] */ 

#ifndef CSC_ONLINESERVER_IDL
#define CSC_ONLINESERVER_IDL
//+-------------------------------------------------------------------------
//
//  ATCMControl 4CSC
//  ATCM.
//
//--------------------------------------------------------------------------
#ifndef __CSC_TRANSFER_TARGET__DEFINED__
#define __CSC_TRANSFER_TARGET__DEFINED__
//  enumerator for transfer target: either W3-Server or Control
typedef 
enum tagCSC_TRANSFER_TARGET
    {	ProgramDomains	= 0,
	VisualDomains	= ProgramDomains + 1
    }	CSC_TRANSFER_TARGET;

#endif // __CSC_TRANSFER_TARGET__DEFINED__














extern RPC_IF_HANDLE __MIDL_itf_CSC_OnlineServer_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CSC_OnlineServer_0000_v0_0_s_ifspec;

#ifndef __IConnection_INTERFACE_DEFINED__
#define __IConnection_INTERFACE_DEFINED__

/* interface IConnection */
/* [unique][helpstring][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("08C2E9E3-AB62-47b1-956C-9DC1479988C7")
    IConnection : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckConnection( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConnection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConnection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConnection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConnection __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConnection __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConnection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConnection __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckConnection )( 
            IConnection __RPC_FAR * This);
        
        END_INTERFACE
    } IConnectionVtbl;

    interface IConnection
    {
        CONST_VTBL struct IConnectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConnection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConnection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConnection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConnection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConnection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConnection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConnection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IConnection_CheckConnection(This)	\
    (This)->lpVtbl -> CheckConnection(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnection_CheckConnection_Proxy( 
    IConnection __RPC_FAR * This);


void __RPC_STUB IConnection_CheckConnection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConnection_INTERFACE_DEFINED__ */


#ifndef __IControlMgt_INTERFACE_DEFINED__
#define __IControlMgt_INTERFACE_DEFINED__

/* interface IControlMgt */
/* [unique][helpstring][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IControlMgt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("092CA1FD-2A47-4f66-8E34-8563882D4872")
    IControlMgt : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [string][in] */ BSTR sUser,
            /* [string][in] */ BSTR sPassword,
            /* [retval][out] */ long __RPC_FAR *plKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Logout( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ColdStartResource( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WarmStartResource( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PauseResource( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ContinueResource( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartTaskOnResource( 
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTask) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopTaskOnResource( 
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTask) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartAllTasksOnResource( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopAllTasksOnResource( 
            /* [in] */ long lKey) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IControlMgtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IControlMgt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IControlMgt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IControlMgt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Login )( 
            IControlMgt __RPC_FAR * This,
            /* [string][in] */ BSTR sUser,
            /* [string][in] */ BSTR sPassword,
            /* [retval][out] */ long __RPC_FAR *plKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Logout )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ColdStartResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WarmStartResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PauseResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ContinueResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartTaskOnResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTask);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopTaskOnResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTask);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartAllTasksOnResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopAllTasksOnResource )( 
            IControlMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        END_INTERFACE
    } IControlMgtVtbl;

    interface IControlMgt
    {
        CONST_VTBL struct IControlMgtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IControlMgt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IControlMgt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IControlMgt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IControlMgt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IControlMgt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IControlMgt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IControlMgt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IControlMgt_Login(This,sUser,sPassword,plKey)	\
    (This)->lpVtbl -> Login(This,sUser,sPassword,plKey)

#define IControlMgt_Logout(This,lKey)	\
    (This)->lpVtbl -> Logout(This,lKey)

#define IControlMgt_ColdStartResource(This,lKey)	\
    (This)->lpVtbl -> ColdStartResource(This,lKey)

#define IControlMgt_WarmStartResource(This,lKey)	\
    (This)->lpVtbl -> WarmStartResource(This,lKey)

#define IControlMgt_PauseResource(This,lKey)	\
    (This)->lpVtbl -> PauseResource(This,lKey)

#define IControlMgt_ContinueResource(This,lKey)	\
    (This)->lpVtbl -> ContinueResource(This,lKey)

#define IControlMgt_StartTaskOnResource(This,lKey,sTask)	\
    (This)->lpVtbl -> StartTaskOnResource(This,lKey,sTask)

#define IControlMgt_StopTaskOnResource(This,lKey,sTask)	\
    (This)->lpVtbl -> StopTaskOnResource(This,lKey,sTask)

#define IControlMgt_StartAllTasksOnResource(This,lKey)	\
    (This)->lpVtbl -> StartAllTasksOnResource(This,lKey)

#define IControlMgt_StopAllTasksOnResource(This,lKey)	\
    (This)->lpVtbl -> StopAllTasksOnResource(This,lKey)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_Login_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [string][in] */ BSTR sUser,
    /* [string][in] */ BSTR sPassword,
    /* [retval][out] */ long __RPC_FAR *plKey);


void __RPC_STUB IControlMgt_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_Logout_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IControlMgt_Logout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_ColdStartResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IControlMgt_ColdStartResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_WarmStartResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IControlMgt_WarmStartResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_PauseResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IControlMgt_PauseResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_ContinueResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IControlMgt_ContinueResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StartTaskOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTask);


void __RPC_STUB IControlMgt_StartTaskOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StopTaskOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTask);


void __RPC_STUB IControlMgt_StopTaskOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StartAllTasksOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IControlMgt_StartAllTasksOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StopAllTasksOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IControlMgt_StopAllTasksOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IControlMgt_INTERFACE_DEFINED__ */


#ifndef __IMessages_INTERFACE_DEFINED__
#define __IMessages_INTERFACE_DEFINED__

/* interface IMessages */
/* [unique][helpstring][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IMessages;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("74D30907-795D-4c5d-AD77-FE3EA0551544")
    IMessages : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMinMaxHistoryIDs( 
            /* [in] */ long lKey,
            /* [out] */ long __RPC_FAR *pFirst,
            /* [out] */ long __RPC_FAR *pLast) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetHistoryMessage( 
            /* [in] */ long lKey,
            /* [in] */ long idHistory,
            /* [out] */ BSTR __RPC_FAR *sMessage,
            /* [out] */ DATE __RPC_FAR *dt) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaxHistoryLength( 
            /* [in] */ long lKey,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaxHistoryLength( 
            /* [in] */ long lKey,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearHistoryBuffer( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Disconnect( 
            /* [in] */ long lKey) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessagesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMessages __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMessages __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMessages __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMessages __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMessages __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMessages __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMessages __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMinMaxHistoryIDs )( 
            IMessages __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [out] */ long __RPC_FAR *pFirst,
            /* [out] */ long __RPC_FAR *pLast);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHistoryMessage )( 
            IMessages __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ long idHistory,
            /* [out] */ BSTR __RPC_FAR *sMessage,
            /* [out] */ DATE __RPC_FAR *dt);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxHistoryLength )( 
            IMessages __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxHistoryLength )( 
            IMessages __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearHistoryBuffer )( 
            IMessages __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IMessages __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IMessages __RPC_FAR * This,
            /* [in] */ long lKey);
        
        END_INTERFACE
    } IMessagesVtbl;

    interface IMessages
    {
        CONST_VTBL struct IMessagesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessages_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMessages_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMessages_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMessages_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMessages_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMessages_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMessages_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMessages_GetMinMaxHistoryIDs(This,lKey,pFirst,pLast)	\
    (This)->lpVtbl -> GetMinMaxHistoryIDs(This,lKey,pFirst,pLast)

#define IMessages_GetHistoryMessage(This,lKey,idHistory,sMessage,dt)	\
    (This)->lpVtbl -> GetHistoryMessage(This,lKey,idHistory,sMessage,dt)

#define IMessages_get_MaxHistoryLength(This,lKey,pVal)	\
    (This)->lpVtbl -> get_MaxHistoryLength(This,lKey,pVal)

#define IMessages_put_MaxHistoryLength(This,lKey,newVal)	\
    (This)->lpVtbl -> put_MaxHistoryLength(This,lKey,newVal)

#define IMessages_ClearHistoryBuffer(This,lKey)	\
    (This)->lpVtbl -> ClearHistoryBuffer(This,lKey)

#define IMessages_Connect(This,lKey)	\
    (This)->lpVtbl -> Connect(This,lKey)

#define IMessages_Disconnect(This,lKey)	\
    (This)->lpVtbl -> Disconnect(This,lKey)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_GetMinMaxHistoryIDs_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [out] */ long __RPC_FAR *pFirst,
    /* [out] */ long __RPC_FAR *pLast);


void __RPC_STUB IMessages_GetMinMaxHistoryIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_GetHistoryMessage_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ long idHistory,
    /* [out] */ BSTR __RPC_FAR *sMessage,
    /* [out] */ DATE __RPC_FAR *dt);


void __RPC_STUB IMessages_GetHistoryMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMessages_get_MaxHistoryLength_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMessages_get_MaxHistoryLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMessages_put_MaxHistoryLength_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ long newVal);


void __RPC_STUB IMessages_put_MaxHistoryLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_ClearHistoryBuffer_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IMessages_ClearHistoryBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_Connect_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IMessages_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_Disconnect_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IMessages_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMessages_INTERFACE_DEFINED__ */


#ifndef __ILicenseMgt_INTERFACE_DEFINED__
#define __ILicenseMgt_INTERFACE_DEFINED__

/* interface ILicenseMgt */
/* [unique][helpstring][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_ILicenseMgt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7B3E7997-0DD5-41a2-8CB2-87443F0846D9")
    ILicenseMgt : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SerialNo( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EnableCode( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ScrambledLicInfo( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILicenseMgtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILicenseMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILicenseMgt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILicenseMgt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ILicenseMgt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ILicenseMgt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ILicenseMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ILicenseMgt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SerialNo )( 
            ILicenseMgt __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EnableCode )( 
            ILicenseMgt __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScrambledLicInfo )( 
            ILicenseMgt __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } ILicenseMgtVtbl;

    interface ILicenseMgt
    {
        CONST_VTBL struct ILicenseMgtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILicenseMgt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILicenseMgt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILicenseMgt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILicenseMgt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILicenseMgt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILicenseMgt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILicenseMgt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILicenseMgt_get_SerialNo(This,pVal)	\
    (This)->lpVtbl -> get_SerialNo(This,pVal)

#define ILicenseMgt_put_EnableCode(This,newVal)	\
    (This)->lpVtbl -> put_EnableCode(This,newVal)

#define ILicenseMgt_get_ScrambledLicInfo(This,pVal)	\
    (This)->lpVtbl -> get_ScrambledLicInfo(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILicenseMgt_get_SerialNo_Proxy( 
    ILicenseMgt __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ILicenseMgt_get_SerialNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ILicenseMgt_put_EnableCode_Proxy( 
    ILicenseMgt __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ILicenseMgt_put_EnableCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILicenseMgt_get_ScrambledLicInfo_Proxy( 
    ILicenseMgt __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ILicenseMgt_get_ScrambledLicInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILicenseMgt_INTERFACE_DEFINED__ */


#ifndef ___ICERemoteMessageEvent_INTERFACE_DEFINED__
#define ___ICERemoteMessageEvent_INTERFACE_DEFINED__

/* interface _ICERemoteMessageEvent */
/* [object][helpstring][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID__ICERemoteMessageEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0F01D071-00FD-4686-BEC3-81E8A77B2575")
    _ICERemoteMessageEvent : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnNewMessage( 
            /* [in] */ BSTR sMessage) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _ICERemoteMessageEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ICERemoteMessageEvent __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ICERemoteMessageEvent __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ICERemoteMessageEvent __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ICERemoteMessageEvent __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ICERemoteMessageEvent __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ICERemoteMessageEvent __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ICERemoteMessageEvent __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnNewMessage )( 
            _ICERemoteMessageEvent __RPC_FAR * This,
            /* [in] */ BSTR sMessage);
        
        END_INTERFACE
    } _ICERemoteMessageEventVtbl;

    interface _ICERemoteMessageEvent
    {
        CONST_VTBL struct _ICERemoteMessageEventVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICERemoteMessageEvent_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ICERemoteMessageEvent_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ICERemoteMessageEvent_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ICERemoteMessageEvent_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ICERemoteMessageEvent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ICERemoteMessageEvent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ICERemoteMessageEvent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define _ICERemoteMessageEvent_OnNewMessage(This,sMessage)	\
    (This)->lpVtbl -> OnNewMessage(This,sMessage)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _ICERemoteMessageEvent_OnNewMessage_Proxy( 
    _ICERemoteMessageEvent __RPC_FAR * This,
    /* [in] */ BSTR sMessage);


void __RPC_STUB _ICERemoteMessageEvent_OnNewMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___ICERemoteMessageEvent_INTERFACE_DEFINED__ */


#ifndef __ICommissioningMgt_INTERFACE_DEFINED__
#define __ICommissioningMgt_INTERFACE_DEFINED__

/* interface ICommissioningMgt */
/* [unique][helpstring][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_ICommissioningMgt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BC62E932-1E49-4d0a-BA75-C413B9F6CB30")
    ICommissioningMgt : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentUser( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [string][in] */ BSTR sUser,
            /* [string][in] */ BSTR sPassword,
            /* [string][in] */ BSTR sEngMachineName,
            /* [out] */ long __RPC_FAR *plKey,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pUnkSession) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Logout( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ForceLogout( 
            /* [string][in] */ BSTR sUserName,
            /* [string][in] */ BSTR sPassword,
            /* [string][in] */ BSTR sEngMachineName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICommissioningMgtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICommissioningMgt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICommissioningMgt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentUser )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Login )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [string][in] */ BSTR sUser,
            /* [string][in] */ BSTR sPassword,
            /* [string][in] */ BSTR sEngMachineName,
            /* [out] */ long __RPC_FAR *plKey,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *pUnkSession);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Logout )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForceLogout )( 
            ICommissioningMgt __RPC_FAR * This,
            /* [string][in] */ BSTR sUserName,
            /* [string][in] */ BSTR sPassword,
            /* [string][in] */ BSTR sEngMachineName);
        
        END_INTERFACE
    } ICommissioningMgtVtbl;

    interface ICommissioningMgt
    {
        CONST_VTBL struct ICommissioningMgtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICommissioningMgt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICommissioningMgt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICommissioningMgt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICommissioningMgt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICommissioningMgt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICommissioningMgt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICommissioningMgt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICommissioningMgt_get_CurrentUser(This,pVal)	\
    (This)->lpVtbl -> get_CurrentUser(This,pVal)

#define ICommissioningMgt_Login(This,sUser,sPassword,sEngMachineName,plKey,pUnkSession)	\
    (This)->lpVtbl -> Login(This,sUser,sPassword,sEngMachineName,plKey,pUnkSession)

#define ICommissioningMgt_Logout(This,lKey)	\
    (This)->lpVtbl -> Logout(This,lKey)

#define ICommissioningMgt_ForceLogout(This,sUserName,sPassword,sEngMachineName)	\
    (This)->lpVtbl -> ForceLogout(This,sUserName,sPassword,sEngMachineName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_get_CurrentUser_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICommissioningMgt_get_CurrentUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_Login_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [string][in] */ BSTR sUser,
    /* [string][in] */ BSTR sPassword,
    /* [string][in] */ BSTR sEngMachineName,
    /* [out] */ long __RPC_FAR *plKey,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *pUnkSession);


void __RPC_STUB ICommissioningMgt_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_Logout_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB ICommissioningMgt_Logout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_ForceLogout_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [string][in] */ BSTR sUserName,
    /* [string][in] */ BSTR sPassword,
    /* [string][in] */ BSTR sEngMachineName);


void __RPC_STUB ICommissioningMgt_ForceLogout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICommissioningMgt_INTERFACE_DEFINED__ */


#ifndef ___FCCommissioningMgtNotification_INTERFACE_DEFINED__
#define ___FCCommissioningMgtNotification_INTERFACE_DEFINED__

/* interface _FCCommissioningMgtNotification */
/* [object][helpstring][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID__FCCommissioningMgtNotification;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4E348888-2A49-4fef-9934-E2D57C5D638C")
    _FCCommissioningMgtNotification : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnUserLoggedin( 
            /* [string][in] */ BSTR sUserName,
            /* [string][in] */ BSTR sEngMachineName,
            BOOL bLoggingIn) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _FCCommissioningMgtNotificationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _FCCommissioningMgtNotification __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _FCCommissioningMgtNotification __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _FCCommissioningMgtNotification __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _FCCommissioningMgtNotification __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _FCCommissioningMgtNotification __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _FCCommissioningMgtNotification __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _FCCommissioningMgtNotification __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUserLoggedin )( 
            _FCCommissioningMgtNotification __RPC_FAR * This,
            /* [string][in] */ BSTR sUserName,
            /* [string][in] */ BSTR sEngMachineName,
            BOOL bLoggingIn);
        
        END_INTERFACE
    } _FCCommissioningMgtNotificationVtbl;

    interface _FCCommissioningMgtNotification
    {
        CONST_VTBL struct _FCCommissioningMgtNotificationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _FCCommissioningMgtNotification_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _FCCommissioningMgtNotification_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _FCCommissioningMgtNotification_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _FCCommissioningMgtNotification_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _FCCommissioningMgtNotification_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _FCCommissioningMgtNotification_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _FCCommissioningMgtNotification_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define _FCCommissioningMgtNotification_OnUserLoggedin(This,sUserName,sEngMachineName,bLoggingIn)	\
    (This)->lpVtbl -> OnUserLoggedin(This,sUserName,sEngMachineName,bLoggingIn)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _FCCommissioningMgtNotification_OnUserLoggedin_Proxy( 
    _FCCommissioningMgtNotification __RPC_FAR * This,
    /* [string][in] */ BSTR sUserName,
    /* [string][in] */ BSTR sEngMachineName,
    BOOL bLoggingIn);


void __RPC_STUB _FCCommissioningMgtNotification_OnUserLoggedin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___FCCommissioningMgtNotification_INTERFACE_DEFINED__ */


#ifndef __ITransfer_INTERFACE_DEFINED__
#define __ITransfer_INTERFACE_DEFINED__

/* interface ITransfer */
/* [unique][helpstring][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_ITransfer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5C5D0124-1D7D-4bf2-87A7-226D609238EE")
    ITransfer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartOnlineChange( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CommitOnlineChange( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sResource,
            /* [string][in] */ BSTR sProjectName,
            /* [string][in] */ BSTR sProjectGUID,
            /* [string][in] */ BSTR sDownloadGUID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CancelOnlineChange( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DownloadDomain( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sDomainName,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UploadDomain( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sDomainName,
            /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteAllDomains( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteDomain( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sDomainName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDomainList( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *listDomainNames) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DLoadAttribute( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sAtributeName,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearResource( 
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FlashResource( 
            /* [in] */ long lKey) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITransferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITransfer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITransfer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITransfer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartOnlineChange )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CommitOnlineChange )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sResource,
            /* [string][in] */ BSTR sProjectName,
            /* [string][in] */ BSTR sProjectGUID,
            /* [string][in] */ BSTR sDownloadGUID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelOnlineChange )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DownloadDomain )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sDomainName,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UploadDomain )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sDomainName,
            /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteAllDomains )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteDomain )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sDomainName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDomainList )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *listDomainNames);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DLoadAttribute )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src,
            /* [string][in] */ BSTR sAtributeName,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearResource )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ CSC_TRANSFER_TARGET src);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FlashResource )( 
            ITransfer __RPC_FAR * This,
            /* [in] */ long lKey);
        
        END_INTERFACE
    } ITransferVtbl;

    interface ITransfer
    {
        CONST_VTBL struct ITransferVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITransfer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITransfer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITransfer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITransfer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITransfer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITransfer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITransfer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITransfer_StartOnlineChange(This,lKey,src)	\
    (This)->lpVtbl -> StartOnlineChange(This,lKey,src)

#define ITransfer_CommitOnlineChange(This,lKey,src,sResource,sProjectName,sProjectGUID,sDownloadGUID)	\
    (This)->lpVtbl -> CommitOnlineChange(This,lKey,src,sResource,sProjectName,sProjectGUID,sDownloadGUID)

#define ITransfer_CancelOnlineChange(This,lKey,src)	\
    (This)->lpVtbl -> CancelOnlineChange(This,lKey,src)

#define ITransfer_DownloadDomain(This,lKey,src,sDomainName,content)	\
    (This)->lpVtbl -> DownloadDomain(This,lKey,src,sDomainName,content)

#define ITransfer_UploadDomain(This,lKey,src,sDomainName,content)	\
    (This)->lpVtbl -> UploadDomain(This,lKey,src,sDomainName,content)

#define ITransfer_DeleteAllDomains(This,lKey,src)	\
    (This)->lpVtbl -> DeleteAllDomains(This,lKey,src)

#define ITransfer_DeleteDomain(This,lKey,src,sDomainName)	\
    (This)->lpVtbl -> DeleteDomain(This,lKey,src,sDomainName)

#define ITransfer_GetDomainList(This,lKey,src,listDomainNames)	\
    (This)->lpVtbl -> GetDomainList(This,lKey,src,listDomainNames)

#define ITransfer_get_DLoadAttribute(This,lKey,src,sAtributeName,pVal)	\
    (This)->lpVtbl -> get_DLoadAttribute(This,lKey,src,sAtributeName,pVal)

#define ITransfer_ClearResource(This,lKey,src)	\
    (This)->lpVtbl -> ClearResource(This,lKey,src)

#define ITransfer_FlashResource(This,lKey)	\
    (This)->lpVtbl -> FlashResource(This,lKey)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_StartOnlineChange_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src);


void __RPC_STUB ITransfer_StartOnlineChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_CommitOnlineChange_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sResource,
    /* [string][in] */ BSTR sProjectName,
    /* [string][in] */ BSTR sProjectGUID,
    /* [string][in] */ BSTR sDownloadGUID);


void __RPC_STUB ITransfer_CommitOnlineChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_CancelOnlineChange_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src);


void __RPC_STUB ITransfer_CancelOnlineChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_DownloadDomain_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sDomainName,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content);


void __RPC_STUB ITransfer_DownloadDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_UploadDomain_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sDomainName,
    /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content);


void __RPC_STUB ITransfer_UploadDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_DeleteAllDomains_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src);


void __RPC_STUB ITransfer_DeleteAllDomains_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_DeleteDomain_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sDomainName);


void __RPC_STUB ITransfer_DeleteDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_GetDomainList_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *listDomainNames);


void __RPC_STUB ITransfer_GetDomainList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITransfer_get_DLoadAttribute_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sAtributeName,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB ITransfer_get_DLoadAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_ClearResource_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src);


void __RPC_STUB ITransfer_ClearResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_FlashResource_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB ITransfer_FlashResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITransfer_INTERFACE_DEFINED__ */


#ifndef __IBreakpoint_INTERFACE_DEFINED__
#define __IBreakpoint_INTERFACE_DEFINED__

/* interface IBreakpoint */
/* [object][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IBreakpoint;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("779A4012-481C-45fe-9324-3172A65BE7A9")
    IBreakpoint : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetBreakpoint( 
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sInstanceName,
            /* [in] */ long lBreakpointNumber,
            /* [retval][out] */ long __RPC_FAR *hBP) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearBreakpoint( 
            /* [in] */ long lKey,
            /* [in] */ long hBP) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearAllBreakpoints( 
            /* [in] */ long lKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ContinueTask( 
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SingleStep( 
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StepOver( 
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StepOut( 
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBreakpointVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBreakpoint __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBreakpoint __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IBreakpoint __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBreakpoint )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sInstanceName,
            /* [in] */ long lBreakpointNumber,
            /* [retval][out] */ long __RPC_FAR *hBP);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearBreakpoint )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ long hBP);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearAllBreakpoints )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ long lKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ContinueTask )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SingleStep )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StepOver )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StepOut )( 
            IBreakpoint __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [string][in] */ BSTR sTaskName);
        
        END_INTERFACE
    } IBreakpointVtbl;

    interface IBreakpoint
    {
        CONST_VTBL struct IBreakpointVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBreakpoint_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBreakpoint_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBreakpoint_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBreakpoint_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IBreakpoint_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IBreakpoint_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IBreakpoint_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IBreakpoint_SetBreakpoint(This,lKey,sInstanceName,lBreakpointNumber,hBP)	\
    (This)->lpVtbl -> SetBreakpoint(This,lKey,sInstanceName,lBreakpointNumber,hBP)

#define IBreakpoint_ClearBreakpoint(This,lKey,hBP)	\
    (This)->lpVtbl -> ClearBreakpoint(This,lKey,hBP)

#define IBreakpoint_ClearAllBreakpoints(This,lKey)	\
    (This)->lpVtbl -> ClearAllBreakpoints(This,lKey)

#define IBreakpoint_ContinueTask(This,lKey,sTaskName)	\
    (This)->lpVtbl -> ContinueTask(This,lKey,sTaskName)

#define IBreakpoint_SingleStep(This,lKey,sTaskName)	\
    (This)->lpVtbl -> SingleStep(This,lKey,sTaskName)

#define IBreakpoint_StepOver(This,lKey,sTaskName)	\
    (This)->lpVtbl -> StepOver(This,lKey,sTaskName)

#define IBreakpoint_StepOut(This,lKey,sTaskName)	\
    (This)->lpVtbl -> StepOut(This,lKey,sTaskName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_SetBreakpoint_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sInstanceName,
    /* [in] */ long lBreakpointNumber,
    /* [retval][out] */ long __RPC_FAR *hBP);


void __RPC_STUB IBreakpoint_SetBreakpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_ClearBreakpoint_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ long hBP);


void __RPC_STUB IBreakpoint_ClearBreakpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_ClearAllBreakpoints_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IBreakpoint_ClearAllBreakpoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_ContinueTask_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName);


void __RPC_STUB IBreakpoint_ContinueTask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_SingleStep_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName);


void __RPC_STUB IBreakpoint_SingleStep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_StepOver_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName);


void __RPC_STUB IBreakpoint_StepOver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_StepOut_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName);


void __RPC_STUB IBreakpoint_StepOut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBreakpoint_INTERFACE_DEFINED__ */


#ifndef ___FCRemoteBreakpointNotification_INTERFACE_DEFINED__
#define ___FCRemoteBreakpointNotification_INTERFACE_DEFINED__

/* interface _FCRemoteBreakpointNotification */
/* [object][helpstring][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID__FCRemoteBreakpointNotification;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("147E5B29-26A5-40d1-A70F-57274B518F4F")
    _FCRemoteBreakpointNotification : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnBreakpointReached( 
            /* [in] */ BSTR sTaskName,
            /* [in] */ long bpQuality,
            /* [in] */ SAFEARRAY __RPC_FAR * arrsInstances,
            /* [in] */ SAFEARRAY __RPC_FAR * arrlBreakpointNumber) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnConfigurationChanged( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _FCRemoteBreakpointNotificationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnBreakpointReached )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This,
            /* [in] */ BSTR sTaskName,
            /* [in] */ long bpQuality,
            /* [in] */ SAFEARRAY __RPC_FAR * arrsInstances,
            /* [in] */ SAFEARRAY __RPC_FAR * arrlBreakpointNumber);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnConfigurationChanged )( 
            _FCRemoteBreakpointNotification __RPC_FAR * This);
        
        END_INTERFACE
    } _FCRemoteBreakpointNotificationVtbl;

    interface _FCRemoteBreakpointNotification
    {
        CONST_VTBL struct _FCRemoteBreakpointNotificationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _FCRemoteBreakpointNotification_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _FCRemoteBreakpointNotification_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _FCRemoteBreakpointNotification_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _FCRemoteBreakpointNotification_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _FCRemoteBreakpointNotification_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _FCRemoteBreakpointNotification_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _FCRemoteBreakpointNotification_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define _FCRemoteBreakpointNotification_OnBreakpointReached(This,sTaskName,bpQuality,arrsInstances,arrlBreakpointNumber)	\
    (This)->lpVtbl -> OnBreakpointReached(This,sTaskName,bpQuality,arrsInstances,arrlBreakpointNumber)

#define _FCRemoteBreakpointNotification_OnConfigurationChanged(This)	\
    (This)->lpVtbl -> OnConfigurationChanged(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _FCRemoteBreakpointNotification_OnBreakpointReached_Proxy( 
    _FCRemoteBreakpointNotification __RPC_FAR * This,
    /* [in] */ BSTR sTaskName,
    /* [in] */ long bpQuality,
    /* [in] */ SAFEARRAY __RPC_FAR * arrsInstances,
    /* [in] */ SAFEARRAY __RPC_FAR * arrlBreakpointNumber);


void __RPC_STUB _FCRemoteBreakpointNotification_OnBreakpointReached_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _FCRemoteBreakpointNotification_OnConfigurationChanged_Proxy( 
    _FCRemoteBreakpointNotification __RPC_FAR * This);


void __RPC_STUB _FCRemoteBreakpointNotification_OnConfigurationChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___FCRemoteBreakpointNotification_INTERFACE_DEFINED__ */


#ifndef __IForceManagement_INTERFACE_DEFINED__
#define __IForceManagement_INTERFACE_DEFINED__

/* interface IForceManagement */
/* [object][helpstring][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IForceManagement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52282971-AD8A-4b97-8DC7-2547C822BE4E")
    IForceManagement : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetForceList( 
            /* [in] */ long lKey,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psItemList,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psValues) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddForceList( 
            /* [in] */ long lKey,
            /* [in] */ SAFEARRAY __RPC_FAR * sItemList,
            /* [in] */ SAFEARRAY __RPC_FAR * sValues) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveForceList( 
            /* [in] */ long lKey,
            /* [in] */ SAFEARRAY __RPC_FAR * sItemList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearForceList( 
            /* [in] */ long lKey) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IForceManagementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IForceManagement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IForceManagement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IForceManagement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetForceList )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psItemList,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psValues);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddForceList )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ SAFEARRAY __RPC_FAR * sItemList,
            /* [in] */ SAFEARRAY __RPC_FAR * sValues);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveForceList )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ long lKey,
            /* [in] */ SAFEARRAY __RPC_FAR * sItemList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearForceList )( 
            IForceManagement __RPC_FAR * This,
            /* [in] */ long lKey);
        
        END_INTERFACE
    } IForceManagementVtbl;

    interface IForceManagement
    {
        CONST_VTBL struct IForceManagementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IForceManagement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IForceManagement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IForceManagement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IForceManagement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IForceManagement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IForceManagement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IForceManagement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IForceManagement_GetForceList(This,lKey,psItemList,psValues)	\
    (This)->lpVtbl -> GetForceList(This,lKey,psItemList,psValues)

#define IForceManagement_AddForceList(This,lKey,sItemList,sValues)	\
    (This)->lpVtbl -> AddForceList(This,lKey,sItemList,sValues)

#define IForceManagement_RemoveForceList(This,lKey,sItemList)	\
    (This)->lpVtbl -> RemoveForceList(This,lKey,sItemList)

#define IForceManagement_ClearForceList(This,lKey)	\
    (This)->lpVtbl -> ClearForceList(This,lKey)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_GetForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psItemList,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psValues);


void __RPC_STUB IForceManagement_GetForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_AddForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ SAFEARRAY __RPC_FAR * sItemList,
    /* [in] */ SAFEARRAY __RPC_FAR * sValues);


void __RPC_STUB IForceManagement_AddForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_RemoveForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ SAFEARRAY __RPC_FAR * sItemList);


void __RPC_STUB IForceManagement_RemoveForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_ClearForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey);


void __RPC_STUB IForceManagement_ClearForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IForceManagement_INTERFACE_DEFINED__ */


#ifndef __ICustomConnect_INTERFACE_DEFINED__
#define __ICustomConnect_INTERFACE_DEFINED__

/* interface ICustomConnect */
/* [object][oleautomation][helpstring][uuid] */ 


EXTERN_C const IID IID_ICustomConnect;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("20B477C8-4957-43e4-B07F-02B47C3B34A3")
    ICustomConnect : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateVarSession( 
            BSTR sInit,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppOPCServer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateCommissioningMgt( 
            BSTR sInit,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppCommissionMgt) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateControlManagement( 
            BSTR sInit,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppControlMgt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICustomConnectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICustomConnect __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICustomConnect __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICustomConnect __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICustomConnect __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICustomConnect __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICustomConnect __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICustomConnect __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateVarSession )( 
            ICustomConnect __RPC_FAR * This,
            BSTR sInit,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppOPCServer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateCommissioningMgt )( 
            ICustomConnect __RPC_FAR * This,
            BSTR sInit,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppCommissionMgt);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateControlManagement )( 
            ICustomConnect __RPC_FAR * This,
            BSTR sInit,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppControlMgt);
        
        END_INTERFACE
    } ICustomConnectVtbl;

    interface ICustomConnect
    {
        CONST_VTBL struct ICustomConnectVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICustomConnect_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICustomConnect_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICustomConnect_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICustomConnect_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICustomConnect_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICustomConnect_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICustomConnect_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICustomConnect_CreateVarSession(This,sInit,ppOPCServer)	\
    (This)->lpVtbl -> CreateVarSession(This,sInit,ppOPCServer)

#define ICustomConnect_CreateCommissioningMgt(This,sInit,ppCommissionMgt)	\
    (This)->lpVtbl -> CreateCommissioningMgt(This,sInit,ppCommissionMgt)

#define ICustomConnect_CreateControlManagement(This,sInit,ppControlMgt)	\
    (This)->lpVtbl -> CreateControlManagement(This,sInit,ppControlMgt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICustomConnect_CreateVarSession_Proxy( 
    ICustomConnect __RPC_FAR * This,
    BSTR sInit,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppOPCServer);


void __RPC_STUB ICustomConnect_CreateVarSession_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICustomConnect_CreateCommissioningMgt_Proxy( 
    ICustomConnect __RPC_FAR * This,
    BSTR sInit,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppCommissionMgt);


void __RPC_STUB ICustomConnect_CreateCommissioningMgt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICustomConnect_CreateControlManagement_Proxy( 
    ICustomConnect __RPC_FAR * This,
    BSTR sInit,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppControlMgt);


void __RPC_STUB ICustomConnect_CreateControlManagement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICustomConnect_INTERFACE_DEFINED__ */



#ifndef __CSC_OnlineServer_LIBRARY_DEFINED__
#define __CSC_OnlineServer_LIBRARY_DEFINED__

/* library CSC_OnlineServer */
/* [helpstring][version][uuid] */ 














EXTERN_C const IID LIBID_CSC_OnlineServer;
#endif /* __CSC_OnlineServer_LIBRARY_DEFINED__ */

/* interface __MIDL_itf_CSC_OnlineServer_0220 */
/* [local] */ 

#endif //CSC_ONLINESERVER_IDL


extern RPC_IF_HANDLE __MIDL_itf_CSC_OnlineServer_0220_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CSC_OnlineServer_0220_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long __RPC_FAR *, unsigned long            , LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long __RPC_FAR *, LPSAFEARRAY __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
