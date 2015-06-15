#ifndef __IDPATHHELP_H_
#define __IDPATHHELP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// strip id from id path - separator is '/'
BOOL StripIdFromIdPath(CString& cs_path, CString& cs_id, BOOL bFromBack = FALSE);

BOOL GetParentIdPath(const CString& strIdPath, CString& strParentIdPath);

BOOL IsPrefixOfIdPath(const CString& strPrefix, const CString& strIdPath);

#endif // __IDPATHHELP_H_

