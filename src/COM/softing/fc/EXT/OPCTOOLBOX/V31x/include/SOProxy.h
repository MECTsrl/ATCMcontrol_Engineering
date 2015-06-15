
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0280 */
/* at Fri Mar 15 09:33:04 2002
 */
/* Compiler settings for SOProxy.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
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

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __SOProxy_h__
#define __SOProxy_h__

/* Forward Declarations */ 

#ifndef __ISoftingOPCWatch_FWD_DEFINED__
#define __ISoftingOPCWatch_FWD_DEFINED__
typedef interface ISoftingOPCWatch ISoftingOPCWatch;
#endif 	/* __ISoftingOPCWatch_FWD_DEFINED__ */


#ifndef __ISoftingOPCWatch_FWD_DEFINED__
#define __ISoftingOPCWatch_FWD_DEFINED__
typedef interface ISoftingOPCWatch ISoftingOPCWatch;
#endif 	/* __ISoftingOPCWatch_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ISoftingOPCWatch_INTERFACE_DEFINED__
#define __ISoftingOPCWatch_INTERFACE_DEFINED__

/* interface ISoftingOPCWatch */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ISoftingOPCWatch;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8C727A71-AA9D-4415-B49F-87EFD8504DBC")
    ISoftingOPCWatch : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetChildren( 
            /* [string][in] */ LPCWSTR parent,
            /* [in] */ BOOL withAttributes,
            /* [string][out] */ LPWSTR __RPC_FAR *children) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CollectChanges( 
            /* [string][in] */ LPCWSTR objects,
            /* [in] */ DWORD doCollect) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetChanges( 
            /* [string][out] */ LPWSTR __RPC_FAR *changes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatistics( 
            /* [string][out] */ LPWSTR __RPC_FAR *statistics) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetObject( 
            /* [string][in] */ LPCWSTR objectDescription,
            /* [string][out] */ LPWSTR __RPC_FAR *obj) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoftingOPCWatchVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISoftingOPCWatch __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISoftingOPCWatch __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISoftingOPCWatch __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetChildren )( 
            ISoftingOPCWatch __RPC_FAR * This,
            /* [string][in] */ LPCWSTR parent,
            /* [in] */ BOOL withAttributes,
            /* [string][out] */ LPWSTR __RPC_FAR *children);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CollectChanges )( 
            ISoftingOPCWatch __RPC_FAR * This,
            /* [string][in] */ LPCWSTR objects,
            /* [in] */ DWORD doCollect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetChanges )( 
            ISoftingOPCWatch __RPC_FAR * This,
            /* [string][out] */ LPWSTR __RPC_FAR *changes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatistics )( 
            ISoftingOPCWatch __RPC_FAR * This,
            /* [string][out] */ LPWSTR __RPC_FAR *statistics);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetObject )( 
            ISoftingOPCWatch __RPC_FAR * This,
            /* [string][in] */ LPCWSTR objectDescription,
            /* [string][out] */ LPWSTR __RPC_FAR *obj);
        
        END_INTERFACE
    } ISoftingOPCWatchVtbl;

    interface ISoftingOPCWatch
    {
        CONST_VTBL struct ISoftingOPCWatchVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoftingOPCWatch_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoftingOPCWatch_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoftingOPCWatch_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoftingOPCWatch_GetChildren(This,parent,withAttributes,children)	\
    (This)->lpVtbl -> GetChildren(This,parent,withAttributes,children)

#define ISoftingOPCWatch_CollectChanges(This,objects,doCollect)	\
    (This)->lpVtbl -> CollectChanges(This,objects,doCollect)

#define ISoftingOPCWatch_GetChanges(This,changes)	\
    (This)->lpVtbl -> GetChanges(This,changes)

#define ISoftingOPCWatch_GetStatistics(This,statistics)	\
    (This)->lpVtbl -> GetStatistics(This,statistics)

#define ISoftingOPCWatch_GetObject(This,objectDescription,obj)	\
    (This)->lpVtbl -> GetObject(This,objectDescription,obj)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISoftingOPCWatch_GetChildren_Proxy( 
    ISoftingOPCWatch __RPC_FAR * This,
    /* [string][in] */ LPCWSTR parent,
    /* [in] */ BOOL withAttributes,
    /* [string][out] */ LPWSTR __RPC_FAR *children);


void __RPC_STUB ISoftingOPCWatch_GetChildren_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISoftingOPCWatch_CollectChanges_Proxy( 
    ISoftingOPCWatch __RPC_FAR * This,
    /* [string][in] */ LPCWSTR objects,
    /* [in] */ DWORD doCollect);


void __RPC_STUB ISoftingOPCWatch_CollectChanges_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISoftingOPCWatch_GetChanges_Proxy( 
    ISoftingOPCWatch __RPC_FAR * This,
    /* [string][out] */ LPWSTR __RPC_FAR *changes);


void __RPC_STUB ISoftingOPCWatch_GetChanges_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISoftingOPCWatch_GetStatistics_Proxy( 
    ISoftingOPCWatch __RPC_FAR * This,
    /* [string][out] */ LPWSTR __RPC_FAR *statistics);


void __RPC_STUB ISoftingOPCWatch_GetStatistics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISoftingOPCWatch_GetObject_Proxy( 
    ISoftingOPCWatch __RPC_FAR * This,
    /* [string][in] */ LPCWSTR objectDescription,
    /* [string][out] */ LPWSTR __RPC_FAR *obj);


void __RPC_STUB ISoftingOPCWatch_GetObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoftingOPCWatch_INTERFACE_DEFINED__ */



#ifndef __SOProxy_LIBRARY_DEFINED__
#define __SOProxy_LIBRARY_DEFINED__

/* library SOProxy */
/* [helpstring][version][uuid] */ 



EXTERN_C const IID LIBID_SOProxy;
#endif /* __SOProxy_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


