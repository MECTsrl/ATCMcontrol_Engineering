/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 24 16:29:21 2015
 */
/* Compiler settings for CSC_OnlSrv.idl:
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

#ifndef __CSC_OnlSrv_h__
#define __CSC_OnlSrv_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __CustomConnect_FWD_DEFINED__
#define __CustomConnect_FWD_DEFINED__

#ifdef __cplusplus
typedef class CustomConnect CustomConnect;
#else
typedef struct CustomConnect CustomConnect;
#endif /* __cplusplus */

#endif 	/* __CustomConnect_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "opcDA.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_CSC_OnlSrv_0000 */
/* [local] */ 

//+-------------------------------------------------------------------------
//
//  ATCMCONTROL 4CSC Server
//  Copyright ATCM.
//
//--------------------------------------------------------------------------


extern RPC_IF_HANDLE __MIDL_itf_CSC_OnlSrv_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CSC_OnlSrv_0000_v0_0_s_ifspec;


#ifndef __CSC_Srv_Lib_LIBRARY_DEFINED__
#define __CSC_Srv_Lib_LIBRARY_DEFINED__

/* library CSC_Srv_Lib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CSC_Srv_Lib;

EXTERN_C const CLSID CLSID_CustomConnect;

#ifdef __cplusplus

class DECLSPEC_UUID("C31E31A7-567C-49a2-B393-CEC8F2E17F99")
CustomConnect;
#endif
#endif /* __CSC_Srv_Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
