
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

#if !defined(_M_IA64) && !defined(_M_AXP64)

#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_SflPropBags,0xF0A5B360,0x8898,0x11d3,0x9D,0xAC,0x00,0xC0,0x4F,0x91,0xE2,0x86);


MIDL_DEFINE_GUID(IID, IID_IPersistenceStrategy,0x54A28AB0,0x426E,0x11D2,0x9D,0x20,0x00,0xC0,0x4F,0x91,0xE2,0x86);


MIDL_DEFINE_GUID(CLSID, CLSID_RegistryPropertyBag,0x54A28AB1,0x426E,0x11D2,0x9D,0x20,0x00,0xC0,0x4F,0x91,0xE2,0x86);


MIDL_DEFINE_GUID(CLSID, CLSID_XMLPropertyBag,0x54A28AB4,0x426E,0x11D2,0x9D,0x20,0x00,0xC0,0x4F,0x91,0xE2,0x86);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AXP64)*/


#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 5.03.0279 */
/* at Fri Jan 28 19:37:08 2000
 */
/* Compiler settings for \Stingray\SFL\include\foundation\persistence\SflPropBags.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win64 (32b run,appending), ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AXP64)

#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_SflPropBags,0xF0A5B360,0x8898,0x11d3,0x9D,0xAC,0x00,0xC0,0x4F,0x91,0xE2,0x86);


MIDL_DEFINE_GUID(IID, IID_IPersistenceStrategy,0x54A28AB0,0x426E,0x11D2,0x9D,0x20,0x00,0xC0,0x4F,0x91,0xE2,0x86);


MIDL_DEFINE_GUID(CLSID, CLSID_RegistryPropertyBag,0x54A28AB1,0x426E,0x11D2,0x9D,0x20,0x00,0xC0,0x4F,0x91,0xE2,0x86);


MIDL_DEFINE_GUID(CLSID, CLSID_XMLPropertyBag,0x54A28AB4,0x426E,0x11D2,0x9D,0x20,0x00,0xC0,0x4F,0x91,0xE2,0x86);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* defined(_M_IA64) || defined(_M_AXP64)*/

