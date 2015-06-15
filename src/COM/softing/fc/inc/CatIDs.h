/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/CatIDs.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CatIDs.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/CatIDs.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           CMN
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  23.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if !defined(_CAT_ID_H) || defined(DEFINE_GLOBAL_VARS)
#undef _CAT_ID_H
#define _CAT_ID_H


//----  Aggregate Includes:   ------------------------------------------------*
#include <comcat.h>

//----  Forward Class Definitions:   -----------------------------------------*

//----  Defines:    ----------------------------------------------------------*
//--------- The following #defines rule creation / reference of global vars:
//  DEFINE_GLOBAL_VARS is set:      Define/instantiate the global variables
//  DEFINE_GLOBAL_VARS not set:     only declare globals for external reference
//
//  DEFINE_GLOBAL_VARS must be #defined only once in a application,
//  most commonly in the implementation of the CWinApp derived appl, class!!!
//
// use a code like this in one of your source files:
/*  -- SAMPLE -- SAMPLE -- SAMPLE -- SAMPLE -- SAMPLE --
// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!

// in DllRegisterServer() add following code:

    HRESULT hr = Create4C_CATIDs();
    if (FAILED(hr)) {
        return hr;
    }
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);

*/ //  SAMPLE -- SAMPLE -- SAMPLE -- SAMPLE -- SAMPLE --

/* Registry Scripts:
Register the component as member of a category by adding following lines to .rgs files:

	InprocServer32 = s '%MODULE%'       // add after this server path entry:

    'Implemented Categories'
    {
        {3269A460-F7E3-11d1-A817-0000B459DC11}
    }

*/

#undef DEFINE
#undef INIT_WITH
#undef DEFINE_CATID
#if defined (DEFINE_GLOBAL_VARS)
#   define DEFINE
#   define INIT_WITH(a)         =a
#   define DEFINE_CATID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
                EXTERN_C const GUID name \
                        = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#else
#   define DEFINE               extern
#   define INIT_WITH(a)
#   define DEFINE_CATID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
                EXTERN_C const GUID FAR name
#endif


//----------    ATCMControl Category IDs:
// define CATID and a descriptive text to use in CreateComponentCategory()
// PLEASE: also add a CreateComponentCategory call in Create4C_CATIDs() below !!!!!!!!!!!!!!

//---   general 4C components; every 4C COM object is part of this category!
DEFINE_CATID(CATID_4CObjects, 0x1c36a78, 0xc95b, 0x44c6, 0xbc, 0x55, 0xf3, 0x20, 0x82, 0x89, 0x3c, 0x24);
DEFINE const TCHAR g_szCATDESC_4CObjects[]      INIT_WITH(_T("4C_20 Objects and Components"));
// {01C36A78-C95B-44c6-BC55-F32082893C24}

//---   4C ActiveDoc Server: Editors, Librarian, IO-configurators:
DEFINE_CATID(CATID_4CActiveDocSrv, 0x1c36a79, 0xc95b, 0x44c6, 0xbc, 0x55, 0xf3, 0x20, 0x82, 0x89, 0x3c, 0x24);
DEFINE const TCHAR g_szCATDESC_4CActiveDocSrv[]      INIT_WITH(_T("4C_20 ActiveDoc Server: Editors, Librarian, Configurators"));
// {01C36A79-C95B-44c6-BC55-F32082893C24}

//---   4C PrjWizards:
DEFINE_CATID(CATID_4CPrjWizards, 0x1c36a7a, 0xc95b, 0x44c6, 0xbc, 0x55, 0xf3, 0x20, 0x82, 0x89, 0x3c, 0x24);
DEFINE const TCHAR g_szCATDESC_4CPrjWizards[]      INIT_WITH(_T("4C_20 Project Wizards"));
// {01C36A7a-C95B-44c6-BC55-F32082893C24}

//---   4C ObjWizards:
DEFINE_CATID(CATID_4CObjWizards, 0x1c36a7b, 0xc95b, 0x44c6, 0xbc, 0x55, 0xf3, 0x20, 0x82, 0x89, 0x3c, 0x24);
DEFINE const TCHAR g_szCATDESC_4CObjWizards[]      INIT_WITH(_T("4C_20 Object Wizards"));
// {01C36A7b-C95B-44c6-BC55-F32082893C24}


//---   OPC Server CATIDs for DataAccess 1.0 and 2.0
DEFINE_CATID(CATID_OPCDAServer10, 0x63d5f430, 0xcfe4, 0x11d1, 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb);
// {63D5F430-CFE4-11d1-B2C8-0060083BA1FB}
DEFINE const TCHAR g_szCATDESC_OPCDAServer10[]      INIT_WITH(_T("OPC DataAccess 1.0"));

DEFINE_CATID(CATID_OPCDAServer20, 0x63d5f432, 0xcfe4, 0x11d1, 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb);
// {63D5F432-CFE4-11d1-B2C8-0060083BA1FB}
DEFINE const TCHAR g_szCATDESC_OPCDAServer20[]      INIT_WITH(_T("OPC DataAccess 2.0"));



//------------------------------------------------------------------*
/**
 * Create4C_CATIDs
 *      create all CATIDs for ATCMControl in registry;
 *      reference these IDs from component's RGS-script!
 *
 * @param           void
 * @return          S_OK; error code
 * @exception       -
 * @see             
*/
#if !defined (DEFINE_GLOBAL_VARS)
HRESULT Create4C_CATIDs(void);
#else // DEFINE_GLOBAL_VARS
HRESULT Create4C_CATIDs(void)
{
#define _CHECKED_HR(hr)     if (FAILED(hr)) return hr;

    _CHECKED_HR(::CreateComponentCategory(CATID_4CObjects, g_szCATDESC_4CObjects));
    _CHECKED_HR(::CreateComponentCategory(CATID_4CActiveDocSrv, g_szCATDESC_4CActiveDocSrv));
    _CHECKED_HR(::CreateComponentCategory(CATID_4CPrjWizards, g_szCATDESC_4CPrjWizards));
    _CHECKED_HR(::CreateComponentCategory(CATID_4CObjWizards, g_szCATDESC_4CObjWizards));

    return S_OK;
}
#endif // DEFINE_GLOBAL_VARS


//------------------------------------------------------------------*
/**
 * CreateComponentCategory
 *
 * @param           catid   ID of category to register
 * @param           catDescription      descriptive text to show for CATID in registry
 * @return          
 * @exception       -
 * @see             
*/
#if !defined (DEFINE_GLOBAL_VARS)
HRESULT CreateComponentCategory(CATID catid, LPCTSTR pszCatDescription);
#else // DEFINE_GLOBAL_VARS
HRESULT CreateComponentCategory(CATID catid, LPCTSTR pszCatDescription)
{
    ICatRegister* pcr = NULL ;    
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
        NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (FAILED(hr)) {
        return hr;   
    }
    // Make sure the HKCR\Component Categories\{..catid...}
    // key is registered    
    CATEGORYINFO catinfo;    
    catinfo.catid = catid;
    catinfo.lcid = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);      // 0x0409 ; // US english
    // Make sure the provided description is not too long.
    // Only copy the first 127 characters if it is
    const int maxLen = (sizeof(catinfo.szDescription) / sizeof(OLECHAR)) - 1;
    int len = _tcslen(pszCatDescription);
    if (len > maxLen) {
        len = maxLen;
    }

#ifndef _UNICODE
    int ret = ::MultiByteToWideChar(CP_ACP, 0, pszCatDescription, -1, catinfo.szDescription, maxLen);
    if (ret <= 0) {
        pcr->Release();	
        return HRESULT_FROM_WIN32(::GetLastError());
    }
#else 
    wcsncpy(catinfo.szDescription, pszCatDescription, len);
#endif 
    // Make sure the description is null terminated
    catinfo.szDescription[len] = '\0';
    hr = pcr->RegisterCategories(1, &catinfo);	
    pcr->Release();	
    return hr;
}
#endif // DEFINE_GLOBAL_VARS

/////////////////////////////////////////////////////////////////////////////
#endif // _CAT_ID_H

#ifdef __cplusplus
}
#endif // __cplusplus


/*
 *----------------------------------------------------------------------------*
 *  $History: CatIDs.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 1.02.01    Time: 10:49
 * Updated in $/4Control/COM/softing/fc/inc
 * New CatIDs for V2.0 added
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:09
 * Created in $/4Control/COM/softing/fc/inc
 * util and baselib from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
