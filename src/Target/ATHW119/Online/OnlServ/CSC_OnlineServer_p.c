/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 24 16:29:21 2015
 */
/* Compiler settings for ..\..\..\..\TargetBase\Kernel\inc\CSC_OnlineServer.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "CSC_OnlineServer.h"

#define TYPE_FORMAT_STRING_SIZE   1075                              
#define PROC_FORMAT_STRING_SIZE   299                               

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Standard interface: __MIDL_itf_CSC_OnlineServer_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IConnection, ver. 0.0,
   GUID={0x08C2E9E3,0xAB62,0x47b1,{0x95,0x6C,0x9D,0xC1,0x47,0x99,0x88,0xC7}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IConnection_CheckConnection_Proxy( 
    IConnection __RPC_FAR * This)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IConnection_CheckConnection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IConnection*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> CheckConnection((IConnection *) ((CStdStubBuffer *)This)->pvServerObject);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(8) _IConnectionProxyVtbl = 
{
    &IID_IConnection,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    IConnection_CheckConnection_Proxy
};


static const PRPC_STUB_FUNCTION IConnection_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    IConnection_CheckConnection_Stub
};

CInterfaceStubVtbl _IConnectionStubVtbl =
{
    &IID_IConnection,
    0,
    8,
    &IConnection_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IControlMgt, ver. 0.0,
   GUID={0x092CA1FD,0x2A47,0x4f66,{0x8E,0x34,0x85,0x63,0x88,0x2D,0x48,0x72}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_Login_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [string][in] */ BSTR sUser,
    /* [string][in] */ BSTR sPassword,
    /* [retval][out] */ long __RPC_FAR *plKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!plKey)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U + 15U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sUser,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sPassword,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sUser,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sPassword,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2] );
            
            *plKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[36],
                         ( void __RPC_FAR * )plKey);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    long _M0;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sPassword;
    void __RPC_FAR *_p_sUser;
    long __RPC_FAR *plKey;
    BSTR sPassword;
    BSTR sUser;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sUser = &sUser;
    MIDL_memset(
               _p_sUser,
               0,
               sizeof( BSTR  ));
    _p_sPassword = &sPassword;
    MIDL_memset(
               _p_sPassword,
               0,
               sizeof( BSTR  ));
    ( long __RPC_FAR * )plKey = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sUser,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sPassword,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        plKey = &_M0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> Login(
         (IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,
         sUser,
         sPassword,
         plKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U + 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( long __RPC_FAR * )_StubMsg.Buffer)++ = *plKey;
        
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sUser,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sPassword,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_Logout_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_Logout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> Logout((IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_ColdStartResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_ColdStartResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ColdStartResource((IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_WarmStartResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      10);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_WarmStartResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> WarmStartResource((IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_PauseResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      11);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_PauseResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> PauseResource((IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_ContinueResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      12);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_ContinueResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ContinueResource((IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StartTaskOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTask)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      13);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sTask,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sTask,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[20] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_StartTaskOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sTask;
    long lKey;
    BSTR sTask;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sTask = &sTask;
    MIDL_memset(
               _p_sTask,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[20] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sTask,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> StartTaskOnResource(
                       (IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,
                       lKey,
                       sTask);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sTask,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StopTaskOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTask)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      14);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sTask,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sTask,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[20] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_StopTaskOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sTask;
    long lKey;
    BSTR sTask;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sTask = &sTask;
    MIDL_memset(
               _p_sTask,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[20] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sTask,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> StopTaskOnResource(
                      (IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,
                      lKey,
                      sTask);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sTask,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StartAllTasksOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      15);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_StartAllTasksOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> StartAllTasksOnResource((IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IControlMgt_StopAllTasksOnResource_Proxy( 
    IControlMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      16);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IControlMgt_StopAllTasksOnResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IControlMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> StopAllTasksOnResource((IControlMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(17) _IControlMgtProxyVtbl = 
{
    &IID_IControlMgt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    IControlMgt_Login_Proxy ,
    IControlMgt_Logout_Proxy ,
    IControlMgt_ColdStartResource_Proxy ,
    IControlMgt_WarmStartResource_Proxy ,
    IControlMgt_PauseResource_Proxy ,
    IControlMgt_ContinueResource_Proxy ,
    IControlMgt_StartTaskOnResource_Proxy ,
    IControlMgt_StopTaskOnResource_Proxy ,
    IControlMgt_StartAllTasksOnResource_Proxy ,
    IControlMgt_StopAllTasksOnResource_Proxy
};


static const PRPC_STUB_FUNCTION IControlMgt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    IControlMgt_Login_Stub,
    IControlMgt_Logout_Stub,
    IControlMgt_ColdStartResource_Stub,
    IControlMgt_WarmStartResource_Stub,
    IControlMgt_PauseResource_Stub,
    IControlMgt_ContinueResource_Stub,
    IControlMgt_StartTaskOnResource_Stub,
    IControlMgt_StopTaskOnResource_Stub,
    IControlMgt_StartAllTasksOnResource_Stub,
    IControlMgt_StopAllTasksOnResource_Stub
};

CInterfaceStubVtbl _IControlMgtStubVtbl =
{
    &IID_IControlMgt,
    0,
    17,
    &IControlMgt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMessages, ver. 0.0,
   GUID={0x74D30907,0x795D,0x4c5d,{0xAD,0x77,0xFE,0x3E,0xA0,0x55,0x15,0x44}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_GetMinMaxHistoryIDs_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [out] */ long __RPC_FAR *pFirst,
    /* [out] */ long __RPC_FAR *pLast)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!pFirst)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        if(!pLast)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[28] );
            
            *pFirst = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
            
            *pLast = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[36],
                         ( void __RPC_FAR * )pFirst);
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[36],
                         ( void __RPC_FAR * )pLast);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IMessages_GetMinMaxHistoryIDs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    long _M1;
    long _M2;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    long __RPC_FAR *pFirst;
    long __RPC_FAR *pLast;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( long __RPC_FAR * )pFirst = 0;
    ( long __RPC_FAR * )pLast = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[28] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        pFirst = &_M1;
        pLast = &_M2;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IMessages*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> GetMinMaxHistoryIDs(
                       (IMessages *) ((CStdStubBuffer *)This)->pvServerObject,
                       lKey,
                       pFirst,
                       pLast);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U + 4U + 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( long __RPC_FAR * )_StubMsg.Buffer)++ = *pFirst;
        
        *(( long __RPC_FAR * )_StubMsg.Buffer)++ = *pLast;
        
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_GetHistoryMessage_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ long idHistory,
    /* [out] */ BSTR __RPC_FAR *sMessage,
    /* [out] */ DATE __RPC_FAR *dt)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(sMessage)
        {
        MIDL_memset(
               sMessage,
               0,
               sizeof( BSTR  ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        if(!sMessage)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        if(!dt)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = idHistory;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[40] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&sMessage,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 7) & ~ 0x7);
            *dt = *(( DATE __RPC_FAR * )_StubMsg.Buffer)++;
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[40],
                         ( void __RPC_FAR * )sMessage);
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[58],
                         ( void __RPC_FAR * )dt);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IMessages_GetHistoryMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    BSTR _M3;
    DATE _M4;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    DATE __RPC_FAR *dt;
    long idHistory;
    long lKey;
    BSTR __RPC_FAR *sMessage;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( BSTR __RPC_FAR * )sMessage = 0;
    ( DATE __RPC_FAR * )dt = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[40] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        idHistory = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        sMessage = &_M3;
        MIDL_memset(
               sMessage,
               0,
               sizeof( BSTR  ));
        dt = &_M4;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IMessages*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> GetHistoryMessage(
                     (IMessages *) ((CStdStubBuffer *)This)->pvServerObject,
                     lKey,
                     idHistory,
                     sMessage,
                     dt);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 8U + 23U + 8U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)sMessage,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)sMessage,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 7) & ~ 0x7);
        *(( DATE __RPC_FAR * )_StubMsg.Buffer)++ = *dt;
        
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)sMessage,
                        &__MIDL_TypeFormatString.Format[40] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMessages_get_MaxHistoryLength_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [retval][out] */ long __RPC_FAR *pVal)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        if(!pVal)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[54] );
            
            *pVal = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[36],
                         ( void __RPC_FAR * )pVal);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IMessages_get_MaxHistoryLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    long _M5;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    long __RPC_FAR *pVal;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( long __RPC_FAR * )pVal = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[54] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        pVal = &_M5;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IMessages*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> get_MaxHistoryLength(
                        (IMessages *) ((CStdStubBuffer *)This)->pvServerObject,
                        lKey,
                        pVal);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U + 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( long __RPC_FAR * )_StubMsg.Buffer)++ = *pVal;
        
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMessages_put_MaxHistoryLength_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ long newVal)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      10);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = newVal;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[62] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IMessages_put_MaxHistoryLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    long newVal;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[62] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        newVal = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IMessages*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> put_MaxHistoryLength(
                        (IMessages *) ((CStdStubBuffer *)This)->pvServerObject,
                        lKey,
                        newVal);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_ClearHistoryBuffer_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      11);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IMessages_ClearHistoryBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IMessages*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ClearHistoryBuffer((IMessages *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_Connect_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      12);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IMessages_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IMessages*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> Connect((IMessages *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMessages_Disconnect_Proxy( 
    IMessages __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      13);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IMessages_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IMessages*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> Disconnect((IMessages *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(14) _IMessagesProxyVtbl = 
{
    &IID_IMessages,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    IMessages_GetMinMaxHistoryIDs_Proxy ,
    IMessages_GetHistoryMessage_Proxy ,
    IMessages_get_MaxHistoryLength_Proxy ,
    IMessages_put_MaxHistoryLength_Proxy ,
    IMessages_ClearHistoryBuffer_Proxy ,
    IMessages_Connect_Proxy ,
    IMessages_Disconnect_Proxy
};


static const PRPC_STUB_FUNCTION IMessages_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    IMessages_GetMinMaxHistoryIDs_Stub,
    IMessages_GetHistoryMessage_Stub,
    IMessages_get_MaxHistoryLength_Stub,
    IMessages_put_MaxHistoryLength_Stub,
    IMessages_ClearHistoryBuffer_Stub,
    IMessages_Connect_Stub,
    IMessages_Disconnect_Stub
};

CInterfaceStubVtbl _IMessagesStubVtbl =
{
    &IID_IMessages,
    0,
    14,
    &IMessages_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ILicenseMgt, ver. 0.0,
   GUID={0x7B3E7997,0x0DD5,0x41a2,{0x8C,0xB2,0x87,0x44,0x3F,0x08,0x46,0xD9}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILicenseMgt_get_SerialNo_Proxy( 
    ILicenseMgt __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(pVal)
        {
        MIDL_memset(
               pVal,
               0,
               sizeof( BSTR  ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!pVal)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[68] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&pVal,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[40],
                         ( void __RPC_FAR * )pVal);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ILicenseMgt_get_SerialNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    BSTR _M6;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    BSTR __RPC_FAR *pVal;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( BSTR __RPC_FAR * )pVal = 0;
    RpcTryFinally
        {
        pVal = &_M6;
        MIDL_memset(
               pVal,
               0,
               sizeof( BSTR  ));
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ILicenseMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> get_SerialNo((ILicenseMgt *) ((CStdStubBuffer *)This)->pvServerObject,pVal);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 8U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)pVal,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)pVal,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)pVal,
                        &__MIDL_TypeFormatString.Format[40] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ILicenseMgt_put_EnableCode_Proxy( 
    ILicenseMgt __RPC_FAR * This,
    /* [in] */ BSTR newVal)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&newVal,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&newVal,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[74] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ILicenseMgt_put_EnableCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_newVal;
    BSTR newVal;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_newVal = &newVal;
    MIDL_memset(
               _p_newVal,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[74] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_newVal,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ILicenseMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> put_EnableCode((ILicenseMgt *) ((CStdStubBuffer *)This)->pvServerObject,newVal);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&newVal,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILicenseMgt_get_ScrambledLicInfo_Proxy( 
    ILicenseMgt __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(pVal)
        {
        MIDL_memset(
               pVal,
               0,
               sizeof( BSTR  ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        if(!pVal)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[68] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&pVal,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[40],
                         ( void __RPC_FAR * )pVal);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ILicenseMgt_get_ScrambledLicInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    BSTR _M7;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    BSTR __RPC_FAR *pVal;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( BSTR __RPC_FAR * )pVal = 0;
    RpcTryFinally
        {
        pVal = &_M7;
        MIDL_memset(
               pVal,
               0,
               sizeof( BSTR  ));
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ILicenseMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> get_ScrambledLicInfo((ILicenseMgt *) ((CStdStubBuffer *)This)->pvServerObject,pVal);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 8U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)pVal,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)pVal,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)pVal,
                        &__MIDL_TypeFormatString.Format[40] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(10) _ILicenseMgtProxyVtbl = 
{
    &IID_ILicenseMgt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    ILicenseMgt_get_SerialNo_Proxy ,
    ILicenseMgt_put_EnableCode_Proxy ,
    ILicenseMgt_get_ScrambledLicInfo_Proxy
};


static const PRPC_STUB_FUNCTION ILicenseMgt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    ILicenseMgt_get_SerialNo_Stub,
    ILicenseMgt_put_EnableCode_Stub,
    ILicenseMgt_get_ScrambledLicInfo_Stub
};

CInterfaceStubVtbl _ILicenseMgtStubVtbl =
{
    &IID_ILicenseMgt,
    0,
    10,
    &ILicenseMgt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: _ICERemoteMessageEvent, ver. 0.0,
   GUID={0x0F01D071,0x00FD,0x4686,{0xBE,0xC3,0x81,0xE8,0xA7,0x7B,0x25,0x75}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _ICERemoteMessageEvent_OnNewMessage_Proxy( 
    _ICERemoteMessageEvent __RPC_FAR * This,
    /* [in] */ BSTR sMessage)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sMessage,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sMessage,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[74] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB _ICERemoteMessageEvent_OnNewMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sMessage;
    BSTR sMessage;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sMessage = &sMessage;
    MIDL_memset(
               _p_sMessage,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[74] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sMessage,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((_ICERemoteMessageEvent*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> OnNewMessage((_ICERemoteMessageEvent *) ((CStdStubBuffer *)This)->pvServerObject,sMessage);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sMessage,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(8) __ICERemoteMessageEventProxyVtbl = 
{
    &IID__ICERemoteMessageEvent,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    _ICERemoteMessageEvent_OnNewMessage_Proxy
};


static const PRPC_STUB_FUNCTION _ICERemoteMessageEvent_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    _ICERemoteMessageEvent_OnNewMessage_Stub
};

CInterfaceStubVtbl __ICERemoteMessageEventStubVtbl =
{
    &IID__ICERemoteMessageEvent,
    0,
    8,
    &_ICERemoteMessageEvent_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ICommissioningMgt, ver. 0.0,
   GUID={0xBC62E932,0x1E49,0x4d0a,{0xBA,0x75,0xC4,0x13,0xB9,0xF6,0xCB,0x30}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_get_CurrentUser_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(pVal)
        {
        MIDL_memset(
               pVal,
               0,
               sizeof( BSTR  ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!pVal)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[68] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&pVal,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[40],
                         ( void __RPC_FAR * )pVal);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ICommissioningMgt_get_CurrentUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    BSTR _M8;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    BSTR __RPC_FAR *pVal;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( BSTR __RPC_FAR * )pVal = 0;
    RpcTryFinally
        {
        pVal = &_M8;
        MIDL_memset(
               pVal,
               0,
               sizeof( BSTR  ));
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ICommissioningMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> get_CurrentUser((ICommissioningMgt *) ((CStdStubBuffer *)This)->pvServerObject,pVal);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 8U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)pVal,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)pVal,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[48] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)pVal,
                        &__MIDL_TypeFormatString.Format[40] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_Login_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [string][in] */ BSTR sUser,
    /* [string][in] */ BSTR sPassword,
    /* [string][in] */ BSTR sEngMachineName,
    /* [out] */ long __RPC_FAR *plKey,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *pUnkSession)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(pUnkSession)
        {
        MIDL_memset(
               pUnkSession,
               0,
               sizeof( LPUNKNOWN __RPC_FAR * ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        if(!plKey)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        if(!pUnkSession)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U + 15U + 15U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sUser,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sPassword,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sEngMachineName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sUser,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sPassword,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sEngMachineName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[80] );
            
            *plKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
            
            NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&pUnkSession,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62],
                                  (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[36],
                         ( void __RPC_FAR * )plKey);
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[62],
                         ( void __RPC_FAR * )pUnkSession);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ICommissioningMgt_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    LPUNKNOWN _M10;
    long _M9;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sEngMachineName;
    void __RPC_FAR *_p_sPassword;
    void __RPC_FAR *_p_sUser;
    LPUNKNOWN __RPC_FAR *pUnkSession;
    long __RPC_FAR *plKey;
    BSTR sEngMachineName;
    BSTR sPassword;
    BSTR sUser;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sUser = &sUser;
    MIDL_memset(
               _p_sUser,
               0,
               sizeof( BSTR  ));
    _p_sPassword = &sPassword;
    MIDL_memset(
               _p_sPassword,
               0,
               sizeof( BSTR  ));
    _p_sEngMachineName = &sEngMachineName;
    MIDL_memset(
               _p_sEngMachineName,
               0,
               sizeof( BSTR  ));
    ( long __RPC_FAR * )plKey = 0;
    ( LPUNKNOWN __RPC_FAR * )pUnkSession = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[80] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sUser,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sPassword,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sEngMachineName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        plKey = &_M9;
        pUnkSession = &_M10;
        _M10 = 0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ICommissioningMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> Login(
         (ICommissioningMgt *) ((CStdStubBuffer *)This)->pvServerObject,
         sUser,
         sPassword,
         sEngMachineName,
         plKey,
         pUnkSession);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U + 0U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)pUnkSession,
                              (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( long __RPC_FAR * )_StubMsg.Buffer)++ = *plKey;
        
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)pUnkSession,
                            (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sUser,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sPassword,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sEngMachineName,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)pUnkSession,
                        &__MIDL_TypeFormatString.Format[62] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_Logout_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ICommissioningMgt_Logout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ICommissioningMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> Logout((ICommissioningMgt *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICommissioningMgt_ForceLogout_Proxy( 
    ICommissioningMgt __RPC_FAR * This,
    /* [string][in] */ BSTR sUserName,
    /* [string][in] */ BSTR sPassword,
    /* [string][in] */ BSTR sEngMachineName)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      10);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U + 15U + 15U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sUserName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sPassword,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sEngMachineName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sUserName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sPassword,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sEngMachineName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[102] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ICommissioningMgt_ForceLogout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sEngMachineName;
    void __RPC_FAR *_p_sPassword;
    void __RPC_FAR *_p_sUserName;
    BSTR sEngMachineName;
    BSTR sPassword;
    BSTR sUserName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sUserName = &sUserName;
    MIDL_memset(
               _p_sUserName,
               0,
               sizeof( BSTR  ));
    _p_sPassword = &sPassword;
    MIDL_memset(
               _p_sPassword,
               0,
               sizeof( BSTR  ));
    _p_sEngMachineName = &sEngMachineName;
    MIDL_memset(
               _p_sEngMachineName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[102] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sUserName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sPassword,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sEngMachineName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ICommissioningMgt*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ForceLogout(
               (ICommissioningMgt *) ((CStdStubBuffer *)This)->pvServerObject,
               sUserName,
               sPassword,
               sEngMachineName);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sUserName,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sPassword,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sEngMachineName,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(11) _ICommissioningMgtProxyVtbl = 
{
    &IID_ICommissioningMgt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    ICommissioningMgt_get_CurrentUser_Proxy ,
    ICommissioningMgt_Login_Proxy ,
    ICommissioningMgt_Logout_Proxy ,
    ICommissioningMgt_ForceLogout_Proxy
};


static const PRPC_STUB_FUNCTION ICommissioningMgt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    ICommissioningMgt_get_CurrentUser_Stub,
    ICommissioningMgt_Login_Stub,
    ICommissioningMgt_Logout_Stub,
    ICommissioningMgt_ForceLogout_Stub
};

CInterfaceStubVtbl _ICommissioningMgtStubVtbl =
{
    &IID_ICommissioningMgt,
    0,
    11,
    &ICommissioningMgt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: _FCCommissioningMgtNotification, ver. 0.0,
   GUID={0x4E348888,0x2A49,0x4fef,{0x99,0x34,0xE2,0xD5,0x7C,0x5D,0x63,0x8C}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _FCCommissioningMgtNotification_OnUserLoggedin_Proxy( 
    _FCCommissioningMgtNotification __RPC_FAR * This,
    /* [string][in] */ BSTR sUserName,
    /* [string][in] */ BSTR sEngMachineName,
    BOOL bLoggingIn)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U + 15U + 11U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sUserName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sEngMachineName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sUserName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sEngMachineName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            *(( BOOL __RPC_FAR * )_StubMsg.Buffer)++ = bLoggingIn;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[116] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB _FCCommissioningMgtNotification_OnUserLoggedin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sEngMachineName;
    void __RPC_FAR *_p_sUserName;
    BOOL bLoggingIn;
    BSTR sEngMachineName;
    BSTR sUserName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sUserName = &sUserName;
    MIDL_memset(
               _p_sUserName,
               0,
               sizeof( BSTR  ));
    _p_sEngMachineName = &sEngMachineName;
    MIDL_memset(
               _p_sEngMachineName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[116] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sUserName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sEngMachineName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        bLoggingIn = *(( BOOL __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((_FCCommissioningMgtNotification*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> OnUserLoggedin(
                  (_FCCommissioningMgtNotification *) ((CStdStubBuffer *)This)->pvServerObject,
                  sUserName,
                  sEngMachineName,
                  bLoggingIn);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sUserName,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sEngMachineName,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(8) __FCCommissioningMgtNotificationProxyVtbl = 
{
    &IID__FCCommissioningMgtNotification,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    _FCCommissioningMgtNotification_OnUserLoggedin_Proxy
};


static const PRPC_STUB_FUNCTION _FCCommissioningMgtNotification_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    _FCCommissioningMgtNotification_OnUserLoggedin_Stub
};

CInterfaceStubVtbl __FCCommissioningMgtNotificationStubVtbl =
{
    &IID__FCCommissioningMgtNotification,
    0,
    8,
    &_FCCommissioningMgtNotification_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ITransfer, ver. 0.0,
   GUID={0x5C5D0124,0x1D7D,0x4bf2,{0x87,0xA7,0x22,0x6D,0x60,0x92,0x38,0xEE}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_StartOnlineChange_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_StartOnlineChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> StartOnlineChange(
                     (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                     lKey,
                     src);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_CommitOnlineChange_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sResource,
    /* [string][in] */ BSTR sProjectName,
    /* [string][in] */ BSTR sProjectGUID,
    /* [string][in] */ BSTR sDownloadGUID)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U + 14U + 15U + 15U + 15U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sResource,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sProjectName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sProjectGUID,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sDownloadGUID,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sResource,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sProjectName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sProjectGUID,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sDownloadGUID,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[134] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_CommitOnlineChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sDownloadGUID;
    void __RPC_FAR *_p_sProjectGUID;
    void __RPC_FAR *_p_sProjectName;
    void __RPC_FAR *_p_sResource;
    long lKey;
    BSTR sDownloadGUID;
    BSTR sProjectGUID;
    BSTR sProjectName;
    BSTR sResource;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sResource = &sResource;
    MIDL_memset(
               _p_sResource,
               0,
               sizeof( BSTR  ));
    _p_sProjectName = &sProjectName;
    MIDL_memset(
               _p_sProjectName,
               0,
               sizeof( BSTR  ));
    _p_sProjectGUID = &sProjectGUID;
    MIDL_memset(
               _p_sProjectGUID,
               0,
               sizeof( BSTR  ));
    _p_sDownloadGUID = &sDownloadGUID;
    MIDL_memset(
               _p_sDownloadGUID,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[134] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sResource,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sProjectName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sProjectGUID,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sDownloadGUID,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> CommitOnlineChange(
                      (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                      lKey,
                      src,
                      sResource,
                      sProjectName,
                      sProjectGUID,
                      sDownloadGUID);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sResource,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sProjectName,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sProjectGUID,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sDownloadGUID,
                            &__MIDL_TypeFormatString.Format[26] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_CancelOnlineChange_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_CancelOnlineChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> CancelOnlineChange(
                      (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                      lKey,
                      src);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_DownloadDomain_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sDomainName,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      10);
        
        
        
        if(!content)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U + 14U + 0U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sDomainName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)content,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1000] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sDomainName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)content,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1000] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[156] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_DownloadDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sDomainName;
    SAFEARRAY __RPC_FAR * __RPC_FAR *content;
    long lKey;
    BSTR sDomainName;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sDomainName = &sDomainName;
    MIDL_memset(
               _p_sDomainName,
               0,
               sizeof( BSTR  ));
    ( SAFEARRAY __RPC_FAR * __RPC_FAR * )content = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[156] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sDomainName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[26],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&content,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1000],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> DownloadDomain(
                  (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                  lKey,
                  src,
                  sDomainName,
                  content);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sDomainName,
                            &__MIDL_TypeFormatString.Format[26] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)content,
                        &__MIDL_TypeFormatString.Format[84] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_UploadDomain_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sDomainName,
    /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *content)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      11);
        
        
        
        if(!content)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U + 14U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sDomainName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sDomainName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[170] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&content,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[1024],
                         ( void __RPC_FAR * )content);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_UploadDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sDomainName;
    SAFEARRAY __RPC_FAR * __RPC_FAR *content;
    long lKey;
    BSTR sDomainName;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sDomainName = &sDomainName;
    MIDL_memset(
               _p_sDomainName,
               0,
               sizeof( BSTR  ));
    ( SAFEARRAY __RPC_FAR * __RPC_FAR * )content = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[170] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sDomainName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> UploadDomain(
                (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                lKey,
                src,
                sDomainName,
                content);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 0U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)content,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)content,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sDomainName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)content,
                        &__MIDL_TypeFormatString.Format[1024] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_DeleteAllDomains_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      12);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_DeleteAllDomains_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> DeleteAllDomains(
                    (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                    lKey,
                    src);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_DeleteDomain_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sDomainName)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      13);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U + 14U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sDomainName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sDomainName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[184] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_DeleteDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sDomainName;
    long lKey;
    BSTR sDomainName;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sDomainName = &sDomainName;
    MIDL_memset(
               _p_sDomainName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[184] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sDomainName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> DeleteDomain(
                (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                lKey,
                src,
                sDomainName);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sDomainName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_GetDomainList_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [retval][out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *listDomainNames)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      14);
        
        
        
        if(!listDomainNames)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[194] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&listDomainNames,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[1024],
                         ( void __RPC_FAR * )listDomainNames);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_GetDomainList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    SAFEARRAY __RPC_FAR * __RPC_FAR *listDomainNames;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( SAFEARRAY __RPC_FAR * __RPC_FAR * )listDomainNames = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[194] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> GetDomainList(
                 (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                 lKey,
                 src,
                 listDomainNames);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 0U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)listDomainNames,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)listDomainNames,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)listDomainNames,
                        &__MIDL_TypeFormatString.Format[1024] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITransfer_get_DLoadAttribute_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src,
    /* [string][in] */ BSTR sAtributeName,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(pVal)
        {
        MIDL_memset(
               pVal,
               0,
               sizeof( VARIANT  ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      15);
        
        
        
        if(!pVal)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U + 14U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sAtributeName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sAtributeName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[204] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&pVal,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1054],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[1046],
                         ( void __RPC_FAR * )pVal);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_get_DLoadAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    VARIANT _M11;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sAtributeName;
    long lKey;
    VARIANT __RPC_FAR *pVal;
    BSTR sAtributeName;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sAtributeName = &sAtributeName;
    MIDL_memset(
               _p_sAtributeName,
               0,
               sizeof( BSTR  ));
    ( VARIANT __RPC_FAR * )pVal = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[204] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sAtributeName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        pVal = &_M11;
        MIDL_memset(
               pVal,
               0,
               sizeof( VARIANT  ));
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> get_DLoadAttribute(
                      (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                      lKey,
                      src,
                      sAtributeName,
                      pVal);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 8U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)pVal,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1054] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)pVal,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1054] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sAtributeName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)pVal,
                        &__MIDL_TypeFormatString.Format[1046] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_ClearResource_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ CSC_TRANSFER_TARGET src)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      16);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 2U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrSimpleTypeMarshall(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( unsigned char __RPC_FAR * )&src,
                         13);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_ClearResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    CSC_TRANSFER_TARGET src;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[128] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&src,
                           13);
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ClearResource(
                 (ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,
                 lKey,
                 src);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITransfer_FlashResource_Proxy( 
    ITransfer __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      17);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ITransfer_FlashResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ITransfer*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> FlashResource((ITransfer *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(18) _ITransferProxyVtbl = 
{
    &IID_ITransfer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    ITransfer_StartOnlineChange_Proxy ,
    ITransfer_CommitOnlineChange_Proxy ,
    ITransfer_CancelOnlineChange_Proxy ,
    ITransfer_DownloadDomain_Proxy ,
    ITransfer_UploadDomain_Proxy ,
    ITransfer_DeleteAllDomains_Proxy ,
    ITransfer_DeleteDomain_Proxy ,
    ITransfer_GetDomainList_Proxy ,
    ITransfer_get_DLoadAttribute_Proxy ,
    ITransfer_ClearResource_Proxy ,
    ITransfer_FlashResource_Proxy
};


static const PRPC_STUB_FUNCTION ITransfer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    ITransfer_StartOnlineChange_Stub,
    ITransfer_CommitOnlineChange_Stub,
    ITransfer_CancelOnlineChange_Stub,
    ITransfer_DownloadDomain_Stub,
    ITransfer_UploadDomain_Stub,
    ITransfer_DeleteAllDomains_Stub,
    ITransfer_DeleteDomain_Stub,
    ITransfer_GetDomainList_Stub,
    ITransfer_get_DLoadAttribute_Stub,
    ITransfer_ClearResource_Stub,
    ITransfer_FlashResource_Stub
};

CInterfaceStubVtbl _ITransferStubVtbl =
{
    &IID_ITransfer,
    0,
    18,
    &ITransfer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IBreakpoint, ver. 0.0,
   GUID={0x779A4012,0x481C,0x45fe,{0x93,0x24,0x31,0x72,0xA6,0x5B,0xE7,0xA9}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_SetBreakpoint_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sInstanceName,
    /* [in] */ long lBreakpointNumber,
    /* [retval][out] */ long __RPC_FAR *hBP)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!hBP)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U + 11U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sInstanceName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sInstanceName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lBreakpointNumber;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[218] );
            
            *hBP = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[36],
                         ( void __RPC_FAR * )hBP);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IBreakpoint_SetBreakpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    long _M12;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sInstanceName;
    long __RPC_FAR *hBP;
    long lBreakpointNumber;
    long lKey;
    BSTR sInstanceName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sInstanceName = &sInstanceName;
    MIDL_memset(
               _p_sInstanceName,
               0,
               sizeof( BSTR  ));
    ( long __RPC_FAR * )hBP = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[218] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sInstanceName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        lBreakpointNumber = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        hBP = &_M12;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IBreakpoint*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> SetBreakpoint(
                 (IBreakpoint *) ((CStdStubBuffer *)This)->pvServerObject,
                 lKey,
                 sInstanceName,
                 lBreakpointNumber,
                 hBP);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U + 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( long __RPC_FAR * )_StubMsg.Buffer)++ = *hBP;
        
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sInstanceName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_ClearBreakpoint_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ long hBP)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = hBP;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[62] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IBreakpoint_ClearBreakpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long hBP;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[62] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        hBP = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IBreakpoint*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ClearBreakpoint(
                   (IBreakpoint *) ((CStdStubBuffer *)This)->pvServerObject,
                   lKey,
                   hBP);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_ClearAllBreakpoints_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IBreakpoint_ClearAllBreakpoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IBreakpoint*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ClearAllBreakpoints((IBreakpoint *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_ContinueTask_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      10);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sTaskName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sTaskName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IBreakpoint_ContinueTask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sTaskName;
    long lKey;
    BSTR sTaskName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sTaskName = &sTaskName;
    MIDL_memset(
               _p_sTaskName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sTaskName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IBreakpoint*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ContinueTask(
                (IBreakpoint *) ((CStdStubBuffer *)This)->pvServerObject,
                lKey,
                sTaskName);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sTaskName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_SingleStep_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      11);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sTaskName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sTaskName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IBreakpoint_SingleStep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sTaskName;
    long lKey;
    BSTR sTaskName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sTaskName = &sTaskName;
    MIDL_memset(
               _p_sTaskName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sTaskName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IBreakpoint*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> SingleStep(
              (IBreakpoint *) ((CStdStubBuffer *)This)->pvServerObject,
              lKey,
              sTaskName);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sTaskName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_StepOver_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      12);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sTaskName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sTaskName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IBreakpoint_StepOver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sTaskName;
    long lKey;
    BSTR sTaskName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sTaskName = &sTaskName;
    MIDL_memset(
               _p_sTaskName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sTaskName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IBreakpoint*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> StepOver(
            (IBreakpoint *) ((CStdStubBuffer *)This)->pvServerObject,
            lKey,
            sTaskName);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sTaskName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBreakpoint_StepOut_Proxy( 
    IBreakpoint __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [string][in] */ BSTR sTaskName)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      13);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 4U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sTaskName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sTaskName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IBreakpoint_StepOut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sTaskName;
    long lKey;
    BSTR sTaskName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sTaskName = &sTaskName;
    MIDL_memset(
               _p_sTaskName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[232] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sTaskName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IBreakpoint*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> StepOut(
           (IBreakpoint *) ((CStdStubBuffer *)This)->pvServerObject,
           lKey,
           sTaskName);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sTaskName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(14) _IBreakpointProxyVtbl = 
{
    &IID_IBreakpoint,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    IBreakpoint_SetBreakpoint_Proxy ,
    IBreakpoint_ClearBreakpoint_Proxy ,
    IBreakpoint_ClearAllBreakpoints_Proxy ,
    IBreakpoint_ContinueTask_Proxy ,
    IBreakpoint_SingleStep_Proxy ,
    IBreakpoint_StepOver_Proxy ,
    IBreakpoint_StepOut_Proxy
};


static const PRPC_STUB_FUNCTION IBreakpoint_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    IBreakpoint_SetBreakpoint_Stub,
    IBreakpoint_ClearBreakpoint_Stub,
    IBreakpoint_ClearAllBreakpoints_Stub,
    IBreakpoint_ContinueTask_Stub,
    IBreakpoint_SingleStep_Stub,
    IBreakpoint_StepOver_Stub,
    IBreakpoint_StepOut_Stub
};

CInterfaceStubVtbl _IBreakpointStubVtbl =
{
    &IID_IBreakpoint,
    0,
    14,
    &IBreakpoint_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: _FCRemoteBreakpointNotification, ver. 0.0,
   GUID={0x147E5B29,0x26A5,0x40d1,{0xA7,0x0F,0x57,0x27,0x4B,0x51,0x8F,0x4F}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _FCRemoteBreakpointNotification_OnBreakpointReached_Proxy( 
    _FCRemoteBreakpointNotification __RPC_FAR * This,
    /* [in] */ BSTR sTaskName,
    /* [in] */ long bpQuality,
    /* [in] */ SAFEARRAY __RPC_FAR * arrsInstances,
    /* [in] */ SAFEARRAY __RPC_FAR * arrlBreakpointNumber)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U + 11U + 0U + 0U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sTaskName,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)arrsInstances,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)arrlBreakpointNumber,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sTaskName,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = bpQuality;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)arrsInstances,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)arrlBreakpointNumber,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[240] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB _FCRemoteBreakpointNotification_OnBreakpointReached_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sTaskName;
    SAFEARRAY __RPC_FAR * arrlBreakpointNumber;
    SAFEARRAY __RPC_FAR * arrsInstances;
    long bpQuality;
    BSTR sTaskName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sTaskName = &sTaskName;
    MIDL_memset(
               _p_sTaskName,
               0,
               sizeof( BSTR  ));
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[240] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sTaskName,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        bpQuality = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&arrsInstances,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&arrlBreakpointNumber,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((_FCRemoteBreakpointNotification*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> OnBreakpointReached(
                       (_FCRemoteBreakpointNotification *) ((CStdStubBuffer *)This)->pvServerObject,
                       sTaskName,
                       bpQuality,
                       arrsInstances,
                       arrlBreakpointNumber);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sTaskName,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _FCRemoteBreakpointNotification_OnConfigurationChanged_Proxy( 
    _FCRemoteBreakpointNotification __RPC_FAR * This)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 0U;
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB _FCRemoteBreakpointNotification_OnConfigurationChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((_FCRemoteBreakpointNotification*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> OnConfigurationChanged((_FCRemoteBreakpointNotification *) ((CStdStubBuffer *)This)->pvServerObject);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(9) __FCRemoteBreakpointNotificationProxyVtbl = 
{
    &IID__FCRemoteBreakpointNotification,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    _FCRemoteBreakpointNotification_OnBreakpointReached_Proxy ,
    _FCRemoteBreakpointNotification_OnConfigurationChanged_Proxy
};


static const PRPC_STUB_FUNCTION _FCRemoteBreakpointNotification_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    _FCRemoteBreakpointNotification_OnBreakpointReached_Stub,
    _FCRemoteBreakpointNotification_OnConfigurationChanged_Stub
};

CInterfaceStubVtbl __FCRemoteBreakpointNotificationStubVtbl =
{
    &IID__FCRemoteBreakpointNotification,
    0,
    9,
    &_FCRemoteBreakpointNotification_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IForceManagement, ver. 0.0,
   GUID={0x52282971,0xAD8A,0x4b97,{0x8D,0xC7,0x25,0x47,0xC8,0x22,0xBE,0x4E}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_GetForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psItemList,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *psValues)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!psItemList)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        if(!psValues)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[256] );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&psItemList,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036],
                                      (unsigned char)0 );
            
            NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR * __RPC_FAR *)&psValues,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036],
                                      (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[1024],
                         ( void __RPC_FAR * )psItemList);
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[1024],
                         ( void __RPC_FAR * )psValues);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IForceManagement_GetForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    SAFEARRAY __RPC_FAR * __RPC_FAR *psItemList;
    SAFEARRAY __RPC_FAR * __RPC_FAR *psValues;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    ( SAFEARRAY __RPC_FAR * __RPC_FAR * )psItemList = 0;
    ( SAFEARRAY __RPC_FAR * __RPC_FAR * )psValues = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[256] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IForceManagement*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> GetForceList(
                (IForceManagement *) ((CStdStubBuffer *)This)->pvServerObject,
                lKey,
                psItemList,
                psValues);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 0U + 0U + 11U;
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)psItemList,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR *)psValues,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)psItemList,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                (unsigned char __RPC_FAR *)psValues,
                                (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1036] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)psItemList,
                        &__MIDL_TypeFormatString.Format[1024] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)psValues,
                        &__MIDL_TypeFormatString.Format[1024] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_AddForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ SAFEARRAY __RPC_FAR * sItemList,
    /* [in] */ SAFEARRAY __RPC_FAR * sValues)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 0U + 0U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)sItemList,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)sValues,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)sItemList,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)sValues,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[268] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IForceManagement_AddForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    SAFEARRAY __RPC_FAR * sItemList;
    SAFEARRAY __RPC_FAR * sValues;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[268] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&sItemList,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064],
                                  (unsigned char)0 );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&sValues,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IForceManagement*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> AddForceList(
                (IForceManagement *) ((CStdStubBuffer *)This)->pvServerObject,
                lKey,
                sItemList,
                sValues);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_RemoveForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey,
    /* [in] */ SAFEARRAY __RPC_FAR * sItemList)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U + 0U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)sItemList,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)sItemList,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[280] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IForceManagement_RemoveForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    SAFEARRAY __RPC_FAR * sItemList;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[280] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&sItemList,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1064],
                                  (unsigned char)0 );
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IForceManagement*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> RemoveForceList(
                   (IForceManagement *) ((CStdStubBuffer *)This)->pvServerObject,
                   lKey,
                   sItemList);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IForceManagement_ClearForceList_Proxy( 
    IForceManagement __RPC_FAR * This,
    /* [in] */ long lKey)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      10);
        
        
        
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 4U;
            NdrProxyGetBuffer(This, &_StubMsg);
            *(( long __RPC_FAR * )_StubMsg.Buffer)++ = lKey;
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
            
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB IForceManagement_ClearForceList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    long lKey;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[16] );
        
        lKey = *(( long __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((IForceManagement*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> ClearForceList((IForceManagement *) ((CStdStubBuffer *)This)->pvServerObject,lKey);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 4U;
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

CINTERFACE_PROXY_VTABLE(11) _IForceManagementProxyVtbl = 
{
    &IID_IForceManagement,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    IForceManagement_GetForceList_Proxy ,
    IForceManagement_AddForceList_Proxy ,
    IForceManagement_RemoveForceList_Proxy ,
    IForceManagement_ClearForceList_Proxy
};


static const PRPC_STUB_FUNCTION IForceManagement_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    IForceManagement_GetForceList_Stub,
    IForceManagement_AddForceList_Stub,
    IForceManagement_RemoveForceList_Stub,
    IForceManagement_ClearForceList_Stub
};

CInterfaceStubVtbl _IForceManagementStubVtbl =
{
    &IID_IForceManagement,
    0,
    11,
    &IForceManagement_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ICustomConnect, ver. 0.0,
   GUID={0x20B477C8,0x4957,0x43e4,{0xB0,0x7F,0x02,0xB4,0x7C,0x3B,0x34,0xA3}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICustomConnect_CreateVarSession_Proxy( 
    ICustomConnect __RPC_FAR * This,
    BSTR sInit,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppOPCServer)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(ppOPCServer)
        {
        MIDL_memset(
               ppOPCServer,
               0,
               sizeof( IUnknown __RPC_FAR *__RPC_FAR * ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      7);
        
        
        
        if(!ppOPCServer)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sInit,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sInit,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[288] );
            
            NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&ppOPCServer,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62],
                                  (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[62],
                         ( void __RPC_FAR * )ppOPCServer);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ICustomConnect_CreateVarSession_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    IUnknown __RPC_FAR *_M13;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sInit;
    IUnknown __RPC_FAR *__RPC_FAR *ppOPCServer;
    BSTR sInit;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sInit = &sInit;
    MIDL_memset(
               _p_sInit,
               0,
               sizeof( BSTR  ));
    ( IUnknown __RPC_FAR *__RPC_FAR * )ppOPCServer = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[288] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sInit,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        ppOPCServer = &_M13;
        _M13 = 0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ICustomConnect*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> CreateVarSession(
                    (ICustomConnect *) ((CStdStubBuffer *)This)->pvServerObject,
                    sInit,
                    ppOPCServer);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 0U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)ppOPCServer,
                              (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)ppOPCServer,
                            (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sInit,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)ppOPCServer,
                        &__MIDL_TypeFormatString.Format[62] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICustomConnect_CreateCommissioningMgt_Proxy( 
    ICustomConnect __RPC_FAR * This,
    BSTR sInit,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppCommissionMgt)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(ppCommissionMgt)
        {
        MIDL_memset(
               ppCommissionMgt,
               0,
               sizeof( IUnknown __RPC_FAR *__RPC_FAR * ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      8);
        
        
        
        if(!ppCommissionMgt)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sInit,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sInit,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[288] );
            
            NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&ppCommissionMgt,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62],
                                  (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[62],
                         ( void __RPC_FAR * )ppCommissionMgt);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ICustomConnect_CreateCommissioningMgt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    IUnknown __RPC_FAR *_M14;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sInit;
    IUnknown __RPC_FAR *__RPC_FAR *ppCommissionMgt;
    BSTR sInit;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sInit = &sInit;
    MIDL_memset(
               _p_sInit,
               0,
               sizeof( BSTR  ));
    ( IUnknown __RPC_FAR *__RPC_FAR * )ppCommissionMgt = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[288] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sInit,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        ppCommissionMgt = &_M14;
        _M14 = 0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ICustomConnect*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> CreateCommissioningMgt(
                          (ICustomConnect *) ((CStdStubBuffer *)This)->pvServerObject,
                          sInit,
                          ppCommissionMgt);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 0U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)ppCommissionMgt,
                              (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)ppCommissionMgt,
                            (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sInit,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)ppCommissionMgt,
                        &__MIDL_TypeFormatString.Format[62] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICustomConnect_CreateControlManagement_Proxy( 
    ICustomConnect __RPC_FAR * This,
    BSTR sInit,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppControlMgt)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    if(ppControlMgt)
        {
        MIDL_memset(
               ppControlMgt,
               0,
               sizeof( IUnknown __RPC_FAR *__RPC_FAR * ));
        }
    RpcTryExcept
        {
        NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      9);
        
        
        
        if(!ppControlMgt)
            {
            RpcRaiseException(RPC_X_NULL_REF_POINTER);
            }
        RpcTryFinally
            {
            
            _StubMsg.BufferLength = 8U;
            NdrUserMarshalBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                      (unsigned char __RPC_FAR *)&sInit,
                                      (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrUserMarshalMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                    (unsigned char __RPC_FAR *)&sInit,
                                    (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[288] );
            
            NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&ppControlMgt,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62],
                                  (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDL_TypeFormatString.Format[62],
                         ( void __RPC_FAR * )ppControlMgt);
        _RetVal = NdrProxyErrorHandler(RpcExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ICustomConnect_CreateControlManagement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    IUnknown __RPC_FAR *_M15;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    void __RPC_FAR *_p_sInit;
    IUnknown __RPC_FAR *__RPC_FAR *ppControlMgt;
    BSTR sInit;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    _p_sInit = &sInit;
    MIDL_memset(
               _p_sInit,
               0,
               sizeof( BSTR  ));
    ( IUnknown __RPC_FAR *__RPC_FAR * )ppControlMgt = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[288] );
        
        NdrUserMarshalUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&_p_sInit,
                                  (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[1014],
                                  (unsigned char)0 );
        
        ppControlMgt = &_M15;
        _M15 = 0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ICustomConnect*) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> CreateControlManagement(
                           (ICustomConnect *) ((CStdStubBuffer *)This)->pvServerObject,
                           sInit,
                           ppControlMgt);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 0U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)ppControlMgt,
                              (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)ppControlMgt,
                            (PFORMAT_STRING) &__MIDL_TypeFormatString.Format[62] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrUserMarshalFree( &_StubMsg,
                            (unsigned char __RPC_FAR *)&sInit,
                            &__MIDL_TypeFormatString.Format[1014] );
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)ppControlMgt,
                        &__MIDL_TypeFormatString.Format[62] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[3];

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

CINTERFACE_PROXY_VTABLE(10) _ICustomConnectProxyVtbl = 
{
    &IID_ICustomConnect,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch_GetTypeInfoCount_Proxy */ ,
    0 /* IDispatch_GetTypeInfo_Proxy */ ,
    0 /* IDispatch_GetIDsOfNames_Proxy */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    ICustomConnect_CreateVarSession_Proxy ,
    ICustomConnect_CreateCommissioningMgt_Proxy ,
    ICustomConnect_CreateControlManagement_Proxy
};


static const PRPC_STUB_FUNCTION ICustomConnect_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    ICustomConnect_CreateVarSession_Stub,
    ICustomConnect_CreateCommissioningMgt_Stub,
    ICustomConnect_CreateControlManagement_Stub
};

CInterfaceStubVtbl _ICustomConnectStubVtbl =
{
    &IID_ICustomConnect,
    0,
    10,
    &ICustomConnect_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_CSC_OnlineServer_0220, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[3] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            LPSAFEARRAY_UserSize
            ,LPSAFEARRAY_UserMarshal
            ,LPSAFEARRAY_UserUnmarshal
            ,LPSAFEARRAY_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   [wire_marshal] or [user_marshal] attribute.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {
			0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/*  2 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/*  4 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/*  6 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/*  8 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 10 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 12 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */
/* 14 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 16 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 18 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 20 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 22 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 24 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 26 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 28 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 30 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 32 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */
/* 34 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 36 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */
/* 38 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 40 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 42 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 44 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 46 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */
/* 48 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 50 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */
/* 52 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 54 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 56 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 58 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */
/* 60 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 62 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 64 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 66 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 68 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 70 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */
/* 72 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 74 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 76 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 78 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 80 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 82 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 84 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 86 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 88 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 90 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 92 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 94 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */
/* 96 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 98 */	NdrFcShort( 0x3e ),	/* Type Offset=62 */
/* 100 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 102 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 104 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 106 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 108 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 110 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 112 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 114 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 116 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 118 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 120 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 122 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 124 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 126 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 128 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 130 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 132 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 134 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 136 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 138 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 140 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 142 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 144 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 146 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 148 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 150 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 152 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 154 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 156 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 158 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 160 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 162 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */
/* 164 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 166 */	NdrFcShort( 0x54 ),	/* Type Offset=84 */
/* 168 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 170 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 172 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 174 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 176 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */
/* 178 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 180 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */
/* 182 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 184 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 186 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 188 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 190 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */
/* 192 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 194 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 196 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 198 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 200 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */
/* 202 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 204 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 206 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 208 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 210 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */
/* 212 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 214 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */
/* 216 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 218 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 220 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 222 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */
/* 224 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 226 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 228 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */
/* 230 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 232 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 234 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 236 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */
/* 238 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 240 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 242 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */
/* 244 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 246 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x0,		/* x86, MIPS & PPC Stack size = 0 */
#else
			0x0,		/* Alpha Stack size = 0 */
#endif
/* 248 */	NdrFcShort( 0x428 ),	/* Type Offset=1064 */
/* 250 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x0,		/* x86, MIPS & PPC Stack size = 0 */
#else
			0x0,		/* Alpha Stack size = 0 */
#endif
/* 252 */	NdrFcShort( 0x428 ),	/* Type Offset=1064 */
/* 254 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 256 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 258 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 260 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */
/* 262 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 264 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */
/* 266 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 268 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 270 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x0,		/* x86, MIPS & PPC Stack size = 0 */
#else
			0x0,		/* Alpha Stack size = 0 */
#endif
/* 272 */	NdrFcShort( 0x428 ),	/* Type Offset=1064 */
/* 274 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x0,		/* x86, MIPS & PPC Stack size = 0 */
#else
			0x0,		/* Alpha Stack size = 0 */
#endif
/* 276 */	NdrFcShort( 0x428 ),	/* Type Offset=1064 */
/* 278 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 280 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 282 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x0,		/* x86, MIPS & PPC Stack size = 0 */
#else
			0x0,		/* Alpha Stack size = 0 */
#endif
/* 284 */	NdrFcShort( 0x428 ),	/* Type Offset=1064 */
/* 286 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 288 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 290 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */
/* 292 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 294 */	NdrFcShort( 0x3e ),	/* Type Offset=62 */
/* 296 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xc ),	/* Offset= 12 (16) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 16 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 18 */	NdrFcShort( 0x8 ),	/* 8 */
/* 20 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (6) */
/* 22 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 24 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 26 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */
/* 30 */	NdrFcShort( 0x4 ),	/* 4 */
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (2) */
/* 36 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 38 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 40 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 42 */	NdrFcShort( 0x6 ),	/* Offset= 6 (48) */
/* 44 */	
			0x13, 0x0,	/* FC_OP */
/* 46 */	NdrFcShort( 0xffffffe2 ),	/* Offset= -30 (16) */
/* 48 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x4 ),	/* 4 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (44) */
/* 58 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 60 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 62 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/* 64 */	NdrFcShort( 0x2 ),	/* Offset= 2 (66) */
/* 66 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 68 */	NdrFcLong( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0x0 ),	/* 0 */
/* 76 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 78 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 80 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 82 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 84 */	
			0x11, 0x0,	/* FC_RP */
/* 86 */	NdrFcShort( 0x392 ),	/* Offset= 914 (1000) */
/* 88 */	
			0x12, 0x10,	/* FC_UP */
/* 90 */	NdrFcShort( 0x2 ),	/* Offset= 2 (92) */
/* 92 */	
			0x12, 0x0,	/* FC_UP */
/* 94 */	NdrFcShort( 0x378 ),	/* Offset= 888 (982) */
/* 96 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 98 */	NdrFcShort( 0x18 ),	/* 24 */
/* 100 */	NdrFcShort( 0xa ),	/* 10 */
/* 102 */	NdrFcLong( 0x8 ),	/* 8 */
/* 106 */	NdrFcShort( 0x58 ),	/* Offset= 88 (194) */
/* 108 */	NdrFcLong( 0xd ),	/* 13 */
/* 112 */	NdrFcShort( 0x78 ),	/* Offset= 120 (232) */
/* 114 */	NdrFcLong( 0x9 ),	/* 9 */
/* 118 */	NdrFcShort( 0xa6 ),	/* Offset= 166 (284) */
/* 120 */	NdrFcLong( 0xc ),	/* 12 */
/* 124 */	NdrFcShort( 0x268 ),	/* Offset= 616 (740) */
/* 126 */	NdrFcLong( 0x24 ),	/* 36 */
/* 130 */	NdrFcShort( 0x290 ),	/* Offset= 656 (786) */
/* 132 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 136 */	NdrFcShort( 0x2ac ),	/* Offset= 684 (820) */
/* 138 */	NdrFcLong( 0x10 ),	/* 16 */
/* 142 */	NdrFcShort( 0x2c4 ),	/* Offset= 708 (850) */
/* 144 */	NdrFcLong( 0x2 ),	/* 2 */
/* 148 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (880) */
/* 150 */	NdrFcLong( 0x3 ),	/* 3 */
/* 154 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (910) */
/* 156 */	NdrFcLong( 0x14 ),	/* 20 */
/* 160 */	NdrFcShort( 0x30c ),	/* Offset= 780 (940) */
/* 162 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (161) */
/* 164 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 166 */	NdrFcShort( 0x4 ),	/* 4 */
/* 168 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 172 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 174 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 176 */	NdrFcShort( 0x4 ),	/* 4 */
/* 178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0x1 ),	/* 1 */
/* 182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 186 */	0x12, 0x0,	/* FC_UP */
/* 188 */	NdrFcShort( 0xffffff54 ),	/* Offset= -172 (16) */
/* 190 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 192 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 194 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 198 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 200 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 202 */	NdrFcShort( 0x4 ),	/* 4 */
/* 204 */	NdrFcShort( 0x4 ),	/* 4 */
/* 206 */	0x11, 0x0,	/* FC_RP */
/* 208 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (164) */
/* 210 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 212 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 214 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 216 */	NdrFcShort( 0x0 ),	/* 0 */
/* 218 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 222 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 226 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 228 */	NdrFcShort( 0xffffff5e ),	/* Offset= -162 (66) */
/* 230 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 232 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 234 */	NdrFcShort( 0x8 ),	/* 8 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x6 ),	/* Offset= 6 (244) */
/* 240 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 242 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 244 */	
			0x11, 0x0,	/* FC_RP */
/* 246 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (214) */
/* 248 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 250 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 254 */	NdrFcShort( 0x0 ),	/* 0 */
/* 256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 258 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 260 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 262 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 264 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 266 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 270 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 274 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 278 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 280 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (248) */
/* 282 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 284 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 286 */	NdrFcShort( 0x8 ),	/* 8 */
/* 288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 290 */	NdrFcShort( 0x6 ),	/* Offset= 6 (296) */
/* 292 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 294 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 296 */	
			0x11, 0x0,	/* FC_RP */
/* 298 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (266) */
/* 300 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 302 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 304 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 306 */	NdrFcShort( 0x2 ),	/* Offset= 2 (308) */
/* 308 */	NdrFcShort( 0x10 ),	/* 16 */
/* 310 */	NdrFcShort( 0x2b ),	/* 43 */
/* 312 */	NdrFcLong( 0x3 ),	/* 3 */
/* 316 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 318 */	NdrFcLong( 0x11 ),	/* 17 */
/* 322 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 324 */	NdrFcLong( 0x2 ),	/* 2 */
/* 328 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 330 */	NdrFcLong( 0x4 ),	/* 4 */
/* 334 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 336 */	NdrFcLong( 0x5 ),	/* 5 */
/* 340 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 342 */	NdrFcLong( 0xb ),	/* 11 */
/* 346 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 348 */	NdrFcLong( 0xa ),	/* 10 */
/* 352 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 354 */	NdrFcLong( 0x6 ),	/* 6 */
/* 358 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (572) */
/* 360 */	NdrFcLong( 0x7 ),	/* 7 */
/* 364 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 366 */	NdrFcLong( 0x8 ),	/* 8 */
/* 370 */	NdrFcShort( 0xfffffe90 ),	/* Offset= -368 (2) */
/* 372 */	NdrFcLong( 0xd ),	/* 13 */
/* 376 */	NdrFcShort( 0xfffffeca ),	/* Offset= -310 (66) */
/* 378 */	NdrFcLong( 0x9 ),	/* 9 */
/* 382 */	NdrFcShort( 0xffffff7a ),	/* Offset= -134 (248) */
/* 384 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 388 */	NdrFcShort( 0xbe ),	/* Offset= 190 (578) */
/* 390 */	NdrFcLong( 0x24 ),	/* 36 */
/* 394 */	NdrFcShort( 0xbc ),	/* Offset= 188 (582) */
/* 396 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 400 */	NdrFcShort( 0xb6 ),	/* Offset= 182 (582) */
/* 402 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 406 */	NdrFcShort( 0xe4 ),	/* Offset= 228 (634) */
/* 408 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 412 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (638) */
/* 414 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 418 */	NdrFcShort( 0xe0 ),	/* Offset= 224 (642) */
/* 420 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 424 */	NdrFcShort( 0xde ),	/* Offset= 222 (646) */
/* 426 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 430 */	NdrFcShort( 0xdc ),	/* Offset= 220 (650) */
/* 432 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 436 */	NdrFcShort( 0xca ),	/* Offset= 202 (638) */
/* 438 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 442 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (642) */
/* 444 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 448 */	NdrFcShort( 0xce ),	/* Offset= 206 (654) */
/* 450 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 454 */	NdrFcShort( 0xc4 ),	/* Offset= 196 (650) */
/* 456 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 460 */	NdrFcShort( 0xc6 ),	/* Offset= 198 (658) */
/* 462 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 466 */	NdrFcShort( 0xc4 ),	/* Offset= 196 (662) */
/* 468 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 472 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (666) */
/* 474 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 478 */	NdrFcShort( 0xc0 ),	/* Offset= 192 (670) */
/* 480 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 484 */	NdrFcShort( 0xba ),	/* Offset= 186 (670) */
/* 486 */	NdrFcLong( 0x10 ),	/* 16 */
/* 490 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 492 */	NdrFcLong( 0x12 ),	/* 18 */
/* 496 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 498 */	NdrFcLong( 0x13 ),	/* 19 */
/* 502 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 504 */	NdrFcLong( 0x16 ),	/* 22 */
/* 508 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 510 */	NdrFcLong( 0x17 ),	/* 23 */
/* 514 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 516 */	NdrFcLong( 0xe ),	/* 14 */
/* 520 */	NdrFcShort( 0x9a ),	/* Offset= 154 (674) */
/* 522 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 526 */	NdrFcShort( 0xa0 ),	/* Offset= 160 (686) */
/* 528 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 532 */	NdrFcShort( 0x66 ),	/* Offset= 102 (634) */
/* 534 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 538 */	NdrFcShort( 0x64 ),	/* Offset= 100 (638) */
/* 540 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 544 */	NdrFcShort( 0x62 ),	/* Offset= 98 (642) */
/* 546 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 550 */	NdrFcShort( 0x5c ),	/* Offset= 92 (642) */
/* 552 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 556 */	NdrFcShort( 0x56 ),	/* Offset= 86 (642) */
/* 558 */	NdrFcLong( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* Offset= 0 (562) */
/* 564 */	NdrFcLong( 0x1 ),	/* 1 */
/* 568 */	NdrFcShort( 0x0 ),	/* Offset= 0 (568) */
/* 570 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (569) */
/* 572 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 574 */	NdrFcShort( 0x8 ),	/* 8 */
/* 576 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 578 */	
			0x12, 0x0,	/* FC_UP */
/* 580 */	NdrFcShort( 0x192 ),	/* Offset= 402 (982) */
/* 582 */	
			0x12, 0x0,	/* FC_UP */
/* 584 */	NdrFcShort( 0x1e ),	/* Offset= 30 (614) */
/* 586 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 588 */	NdrFcLong( 0x2f ),	/* 47 */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 596 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 598 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 600 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 602 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 604 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 606 */	NdrFcShort( 0x1 ),	/* 1 */
/* 608 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 610 */	NdrFcShort( 0x4 ),	/* 4 */
/* 612 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 614 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 616 */	NdrFcShort( 0x10 ),	/* 16 */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0xa ),	/* Offset= 10 (630) */
/* 622 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 624 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 626 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (586) */
/* 628 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 630 */	
			0x12, 0x0,	/* FC_UP */
/* 632 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (604) */
/* 634 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 636 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 638 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 640 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 642 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 644 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 646 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 648 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 650 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 652 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 654 */	
			0x12, 0x0,	/* FC_UP */
/* 656 */	NdrFcShort( 0xffffffac ),	/* Offset= -84 (572) */
/* 658 */	
			0x12, 0x10,	/* FC_UP */
/* 660 */	NdrFcShort( 0xfffffd6e ),	/* Offset= -658 (2) */
/* 662 */	
			0x12, 0x10,	/* FC_UP */
/* 664 */	NdrFcShort( 0xfffffdaa ),	/* Offset= -598 (66) */
/* 666 */	
			0x12, 0x10,	/* FC_UP */
/* 668 */	NdrFcShort( 0xfffffe5c ),	/* Offset= -420 (248) */
/* 670 */	
			0x12, 0x10,	/* FC_UP */
/* 672 */	NdrFcShort( 0xffffffa2 ),	/* Offset= -94 (578) */
/* 674 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 676 */	NdrFcShort( 0x10 ),	/* 16 */
/* 678 */	0x6,		/* FC_SHORT */
			0x2,		/* FC_CHAR */
/* 680 */	0x2,		/* FC_CHAR */
			0x38,		/* FC_ALIGNM4 */
/* 682 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 684 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 686 */	
			0x12, 0x0,	/* FC_UP */
/* 688 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (674) */
/* 690 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 692 */	NdrFcShort( 0x20 ),	/* 32 */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x0 ),	/* Offset= 0 (696) */
/* 698 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 700 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 702 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 704 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 706 */	NdrFcShort( 0xfffffe6a ),	/* Offset= -406 (300) */
/* 708 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 710 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 712 */	NdrFcShort( 0x4 ),	/* 4 */
/* 714 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 716 */	NdrFcShort( 0x0 ),	/* 0 */
/* 718 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 720 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 722 */	NdrFcShort( 0x4 ),	/* 4 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 726 */	NdrFcShort( 0x1 ),	/* 1 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 732 */	0x12, 0x0,	/* FC_UP */
/* 734 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (690) */
/* 736 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 738 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 740 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 742 */	NdrFcShort( 0x8 ),	/* 8 */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	NdrFcShort( 0x6 ),	/* Offset= 6 (752) */
/* 748 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 750 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 752 */	
			0x11, 0x0,	/* FC_RP */
/* 754 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (710) */
/* 756 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 758 */	NdrFcShort( 0x4 ),	/* 4 */
/* 760 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 764 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 766 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 768 */	NdrFcShort( 0x4 ),	/* 4 */
/* 770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0x1 ),	/* 1 */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 778 */	0x12, 0x0,	/* FC_UP */
/* 780 */	NdrFcShort( 0xffffff5a ),	/* Offset= -166 (614) */
/* 782 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 784 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 786 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 788 */	NdrFcShort( 0x8 ),	/* 8 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 792 */	NdrFcShort( 0x6 ),	/* Offset= 6 (798) */
/* 794 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 796 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 798 */	
			0x11, 0x0,	/* FC_RP */
/* 800 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (756) */
/* 802 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 804 */	NdrFcShort( 0x8 ),	/* 8 */
/* 806 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 808 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 810 */	NdrFcShort( 0x10 ),	/* 16 */
/* 812 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 814 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 816 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (802) */
			0x5b,		/* FC_END */
/* 820 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 822 */	NdrFcShort( 0x18 ),	/* 24 */
/* 824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 826 */	NdrFcShort( 0xa ),	/* Offset= 10 (836) */
/* 828 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 830 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 832 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (808) */
/* 834 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 836 */	
			0x11, 0x0,	/* FC_RP */
/* 838 */	NdrFcShort( 0xfffffd90 ),	/* Offset= -624 (214) */
/* 840 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 842 */	NdrFcShort( 0x1 ),	/* 1 */
/* 844 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 848 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 850 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 852 */	NdrFcShort( 0x8 ),	/* 8 */
/* 854 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 856 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 858 */	NdrFcShort( 0x4 ),	/* 4 */
/* 860 */	NdrFcShort( 0x4 ),	/* 4 */
/* 862 */	0x12, 0x0,	/* FC_UP */
/* 864 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (840) */
/* 866 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 868 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 870 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 872 */	NdrFcShort( 0x2 ),	/* 2 */
/* 874 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 878 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 880 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 882 */	NdrFcShort( 0x8 ),	/* 8 */
/* 884 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 886 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 888 */	NdrFcShort( 0x4 ),	/* 4 */
/* 890 */	NdrFcShort( 0x4 ),	/* 4 */
/* 892 */	0x12, 0x0,	/* FC_UP */
/* 894 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (870) */
/* 896 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 898 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 900 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 902 */	NdrFcShort( 0x4 ),	/* 4 */
/* 904 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 908 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 910 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 912 */	NdrFcShort( 0x8 ),	/* 8 */
/* 914 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 916 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 918 */	NdrFcShort( 0x4 ),	/* 4 */
/* 920 */	NdrFcShort( 0x4 ),	/* 4 */
/* 922 */	0x12, 0x0,	/* FC_UP */
/* 924 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (900) */
/* 926 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 928 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 930 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 932 */	NdrFcShort( 0x8 ),	/* 8 */
/* 934 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 938 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 940 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 942 */	NdrFcShort( 0x8 ),	/* 8 */
/* 944 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 946 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 948 */	NdrFcShort( 0x4 ),	/* 4 */
/* 950 */	NdrFcShort( 0x4 ),	/* 4 */
/* 952 */	0x12, 0x0,	/* FC_UP */
/* 954 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (930) */
/* 956 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 958 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 960 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 964 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 966 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 968 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 970 */	NdrFcShort( 0x8 ),	/* 8 */
/* 972 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 974 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 976 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 978 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (960) */
/* 980 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 982 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 984 */	NdrFcShort( 0x28 ),	/* 40 */
/* 986 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (968) */
/* 988 */	NdrFcShort( 0x0 ),	/* Offset= 0 (988) */
/* 990 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 992 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 994 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 996 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffc7b ),	/* Offset= -901 (96) */
			0x5b,		/* FC_END */
/* 1000 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1002 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1004 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1008 */	NdrFcShort( 0xfffffc68 ),	/* Offset= -920 (88) */
/* 1010 */	
			0x12, 0x0,	/* FC_UP */
/* 1012 */	NdrFcShort( 0xfffffc1c ),	/* Offset= -996 (16) */
/* 1014 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1016 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1018 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1022 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1010) */
/* 1024 */	
			0x11, 0x0,	/* FC_RP */
/* 1026 */	NdrFcShort( 0xa ),	/* Offset= 10 (1036) */
/* 1028 */	
			0x13, 0x10,	/* FC_OP */
/* 1030 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1032) */
/* 1032 */	
			0x13, 0x0,	/* FC_OP */
/* 1034 */	NdrFcShort( 0xffffffcc ),	/* Offset= -52 (982) */
/* 1036 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1038 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1040 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1044 */	NdrFcShort( 0xfffffff0 ),	/* Offset= -16 (1028) */
/* 1046 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1048 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1054) */
/* 1050 */	
			0x13, 0x0,	/* FC_OP */
/* 1052 */	NdrFcShort( 0xfffffe96 ),	/* Offset= -362 (690) */
/* 1054 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1056 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1058 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1062 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1050) */
/* 1064 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1066 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1068 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1072 */	NdrFcShort( 0xfffffe6e ),	/* Offset= -402 (670) */

			0x0
        }
    };

const CInterfaceProxyVtbl * _CSC_OnlineServer_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IMessagesProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IBreakpointProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ITransferProxyVtbl,
    ( CInterfaceProxyVtbl *) &__FCRemoteBreakpointNotificationProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ICommissioningMgtProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IForceManagementProxyVtbl,
    ( CInterfaceProxyVtbl *) &__ICERemoteMessageEventProxyVtbl,
    ( CInterfaceProxyVtbl *) &__FCCommissioningMgtNotificationProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ILicenseMgtProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ICustomConnectProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IConnectionProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IControlMgtProxyVtbl,
    0
};

const CInterfaceStubVtbl * _CSC_OnlineServer_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IMessagesStubVtbl,
    ( CInterfaceStubVtbl *) &_IBreakpointStubVtbl,
    ( CInterfaceStubVtbl *) &_ITransferStubVtbl,
    ( CInterfaceStubVtbl *) &__FCRemoteBreakpointNotificationStubVtbl,
    ( CInterfaceStubVtbl *) &_ICommissioningMgtStubVtbl,
    ( CInterfaceStubVtbl *) &_IForceManagementStubVtbl,
    ( CInterfaceStubVtbl *) &__ICERemoteMessageEventStubVtbl,
    ( CInterfaceStubVtbl *) &__FCCommissioningMgtNotificationStubVtbl,
    ( CInterfaceStubVtbl *) &_ILicenseMgtStubVtbl,
    ( CInterfaceStubVtbl *) &_ICustomConnectStubVtbl,
    ( CInterfaceStubVtbl *) &_IConnectionStubVtbl,
    ( CInterfaceStubVtbl *) &_IControlMgtStubVtbl,
    0
};

PCInterfaceName const _CSC_OnlineServer_InterfaceNamesList[] = 
{
    "IMessages",
    "IBreakpoint",
    "ITransfer",
    "_FCRemoteBreakpointNotification",
    "ICommissioningMgt",
    "IForceManagement",
    "_ICERemoteMessageEvent",
    "_FCCommissioningMgtNotification",
    "ILicenseMgt",
    "ICustomConnect",
    "IConnection",
    "IControlMgt",
    0
};

const IID *  _CSC_OnlineServer_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _CSC_OnlineServer_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _CSC_OnlineServer, pIID, n)

int __stdcall _CSC_OnlineServer_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _CSC_OnlineServer, 12, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _CSC_OnlineServer, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _CSC_OnlineServer, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _CSC_OnlineServer, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _CSC_OnlineServer, 12, *pIndex )
    
}

const ExtendedProxyFileInfo CSC_OnlineServer_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _CSC_OnlineServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _CSC_OnlineServer_StubVtblList,
    (const PCInterfaceName * ) & _CSC_OnlineServer_InterfaceNamesList,
    (const IID ** ) & _CSC_OnlineServer_BaseIIDList,
    & _CSC_OnlineServer_IID_Lookup, 
    12,
    1,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
