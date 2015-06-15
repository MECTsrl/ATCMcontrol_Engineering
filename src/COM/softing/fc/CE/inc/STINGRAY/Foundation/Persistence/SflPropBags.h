
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0279 */
/* at Fri Jan 28 19:37:08 2000
 */
/* Compiler settings for \Stingray\SFL\include\foundation\persistence\SflPropBags.idl:
    Os (OptLev=s), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __SflPropBags_h__
#define __SflPropBags_h__

/* Forward Declarations */ 

#ifndef __IPersistenceStrategy_FWD_DEFINED__
#define __IPersistenceStrategy_FWD_DEFINED__
typedef interface IPersistenceStrategy IPersistenceStrategy;
#endif 	/* __IPersistenceStrategy_FWD_DEFINED__ */


#ifndef __RegistryPropertyBag_FWD_DEFINED__
#define __RegistryPropertyBag_FWD_DEFINED__

#ifdef __cplusplus
typedef class RegistryPropertyBag RegistryPropertyBag;
#else
typedef struct RegistryPropertyBag RegistryPropertyBag;
#endif /* __cplusplus */

#endif 	/* __RegistryPropertyBag_FWD_DEFINED__ */


#ifndef __XMLPropertyBag_FWD_DEFINED__
#define __XMLPropertyBag_FWD_DEFINED__

#ifdef __cplusplus
typedef class XMLPropertyBag XMLPropertyBag;
#else
typedef struct XMLPropertyBag XMLPropertyBag;
#endif /* __cplusplus */

#endif 	/* __XMLPropertyBag_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __SflPropBags_LIBRARY_DEFINED__
#define __SflPropBags_LIBRARY_DEFINED__

/* library SflPropBags */
/* [helpfile][helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SflPropBags;

#ifndef __IPersistenceStrategy_INTERFACE_DEFINED__
#define __IPersistenceStrategy_INTERFACE_DEFINED__

/* interface IPersistenceStrategy */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPersistenceStrategy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("54A28AB0-426E-11D2-9D20-00C04F91E286")
    IPersistenceStrategy : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ VARIANT InitParam) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Commit( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ BSTR PropertyName,
            /* [in] */ IUnknown __RPC_FAR *pPersistObject) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR PropertyName,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *pPersistObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPersistenceStrategyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPersistenceStrategy __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPersistenceStrategy __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [in] */ VARIANT InitParam);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Commit )( 
            IPersistenceStrategy __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [in] */ BSTR PropertyName,
            /* [in] */ IUnknown __RPC_FAR *pPersistObject);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IPersistenceStrategy __RPC_FAR * This,
            /* [in] */ BSTR PropertyName,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *pPersistObject);
        
        END_INTERFACE
    } IPersistenceStrategyVtbl;

    interface IPersistenceStrategy
    {
        CONST_VTBL struct IPersistenceStrategyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPersistenceStrategy_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPersistenceStrategy_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPersistenceStrategy_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPersistenceStrategy_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPersistenceStrategy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPersistenceStrategy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPersistenceStrategy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPersistenceStrategy_Init(This,InitParam)	\
    (This)->lpVtbl -> Init(This,InitParam)

#define IPersistenceStrategy_Commit(This)	\
    (This)->lpVtbl -> Commit(This)

#define IPersistenceStrategy_Save(This,PropertyName,pPersistObject)	\
    (This)->lpVtbl -> Save(This,PropertyName,pPersistObject)

#define IPersistenceStrategy_Load(This,PropertyName,pPersistObject)	\
    (This)->lpVtbl -> Load(This,PropertyName,pPersistObject)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPersistenceStrategy_Init_Proxy( 
    IPersistenceStrategy __RPC_FAR * This,
    /* [in] */ VARIANT InitParam);


void __RPC_STUB IPersistenceStrategy_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPersistenceStrategy_Commit_Proxy( 
    IPersistenceStrategy __RPC_FAR * This);


void __RPC_STUB IPersistenceStrategy_Commit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPersistenceStrategy_Save_Proxy( 
    IPersistenceStrategy __RPC_FAR * This,
    /* [in] */ BSTR PropertyName,
    /* [in] */ IUnknown __RPC_FAR *pPersistObject);


void __RPC_STUB IPersistenceStrategy_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPersistenceStrategy_Load_Proxy( 
    IPersistenceStrategy __RPC_FAR * This,
    /* [in] */ BSTR PropertyName,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *pPersistObject);


void __RPC_STUB IPersistenceStrategy_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPersistenceStrategy_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_RegistryPropertyBag;

#ifdef __cplusplus

class DECLSPEC_UUID("54A28AB1-426E-11D2-9D20-00C04F91E286")
RegistryPropertyBag;
#endif

EXTERN_C const CLSID CLSID_XMLPropertyBag;

#ifdef __cplusplus

class DECLSPEC_UUID("54A28AB4-426E-11D2-9D20-00C04F91E286")
XMLPropertyBag;
#endif
#endif /* __SflPropBags_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


