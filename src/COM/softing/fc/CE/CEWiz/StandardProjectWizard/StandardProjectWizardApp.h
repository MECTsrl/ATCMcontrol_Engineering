/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jan 05 15:25:55 2001
 */
/* Compiler settings for D:\Dirk\Programs\4Control\ProjectWizard\StandardProjectWizard\StandardProjectWizard.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __StandardProjectWizard_h__
#define __StandardProjectWizard_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ProjectWizardComObj_FWD_DEFINED__
#define __ProjectWizardComObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class ProjectWizardComObj ProjectWizardComObj;
#else
typedef struct ProjectWizardComObj ProjectWizardComObj;
#endif /* __cplusplus */

#endif 	/* __ProjectWizardComObj_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "CEProjWiz.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __STANDARDPROJECTWIZARDLib_LIBRARY_DEFINED__
#define __STANDARDPROJECTWIZARDLib_LIBRARY_DEFINED__

/* library STANDARDPROJECTWIZARDLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_STANDARDPROJECTWIZARDLib;

EXTERN_C const CLSID CLSID_ProjectWizardComObj;

#ifdef __cplusplus

class DECLSPEC_UUID("F3B04143-CB7F-11D4-B353-0008C77981F8")
ProjectWizardComObj;
#endif
#endif /* __STANDARDPROJECTWIZARDLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif