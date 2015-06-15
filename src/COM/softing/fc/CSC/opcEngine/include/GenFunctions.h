//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENFUNCTIONS.H                                              |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//                                                                            |
//  Description : Helper Functions                                            |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENFUNCTIONS_H_
#define _GENFUNCTIONS_H_

#include "GenObj.h"

#define BOOL2STR(a) ((a) ? _T("TRUE") : _T("FALSE")) 

#ifdef _AFXDLL	// shared
#define EMPTYSTRING(a) 
#define INITSTRING(a) 
#define SETSTRING(a, b) { a = b; } 
#define ADDSTRING(a, b) (a).AddTail(b); 
#define EMPTYSTRINGLIST(a) { (a).RemoveAll();}

#define FREEMEMORY(a) free(a);
#define ALLOCATEMEMORY(a) malloc(a); 

#define ADDPOINTER(a, b) (a).AddTail(b); 
#define EMPTYPTRLIST(a) { (a).RemoveAll();}
#else			// static
#define EMPTYSTRING(a) {CString empty(_T("empty"));	::emptyString(empty, a);}
#define INITSTRING(a) {CString &dllEmpty = ::getEmptyString(); a = dllEmpty; }
#define SETSTRING(a, b) { ::setString(a, b); } 
#define ADDSTRING(a, b) ::addStringToList(a, b); 
#define EMPTYSTRINGLIST(a) { ::emptyStringList(a);}

#define FREEMEMORY(a) ::freeMemory(a);
#define ALLOCATEMEMORY(a) ::allocateMemory(a); 

#define ADDPOINTER(a, b) ::addPtrToList(a, b); 
#define EMPTYPTRLIST(a) { ::emptyPtrList(a);}
#endif

// object desruction, list handling and memory functions (only needed for static libraries)
OPCENGINE_EXPORT void emptyStringList(OUT CStringList& stringList);
OPCENGINE_EXPORT void emptyString(IN const CString &empty, OUT CString& string);
OPCENGINE_EXPORT CString& getEmptyString(void);
OPCENGINE_EXPORT void emptyPtrList(OUT CPtrList& ptrList);
OPCENGINE_EXPORT void setString(OUT CString &string, IN LPCTSTR value);
OPCENGINE_EXPORT POSITION addStringToList(IN CStringList& stringList, IN LPCTSTR value);
OPCENGINE_EXPORT POSITION addPtrToList(IN CPtrList& ptrList, IN void* value);
OPCENGINE_EXPORT void freeMemory(void *memptr);
OPCENGINE_EXPORT void* allocateMemory(ULONG size);

// get the time span between two times
OPCENGINE_EXPORT ULONG getTimeSpan(IN DWORD from, IN DWORD till);

// covert COM enum to string list
OPCENGINE_EXPORT void enumString2stringList(IN IEnumString *enumString,
	OUT CStringList *stringList);

// convert variant to string
OPCENGINE_EXPORT LPCTSTR variant2string(IN CString *buffer, IN LPCVARIANT var);
OPCENGINE_EXPORT LPCTSTR variant2string(IN LPTSTR buffer, IN DWORD size, IN LPCVARIANT var);

// convert variant to string and add vartype
OPCENGINE_EXPORT LPCTSTR variantWithType2string(IN CString *buffer, IN LPCVARIANT var);
OPCENGINE_EXPORT LPCTSTR variantWithType2string(IN LPTSTR buffer, IN DWORD size, IN LPCVARIANT var);

// convert vartype to string
OPCENGINE_EXPORT LPCTSTR vartype2string(IN CString *buffer, IN VARTYPE vt);
OPCENGINE_EXPORT LPCTSTR vartype2string(IN LPTSTR buffer, IN DWORD size, IN VARTYPE vt);

// convert clipformat to string
OPCENGINE_EXPORT LPCTSTR clipformat2string(IN CString *buffer, IN CLIPFORMAT format);
OPCENGINE_EXPORT LPCTSTR clipformat2string(IN LPTSTR buffer, IN DWORD size, IN CLIPFORMAT format);

// convert access rights to string
OPCENGINE_EXPORT LPCTSTR accessRights2string(IN CString *buffer, IN DWORD accessRight);
OPCENGINE_EXPORT LPCTSTR accessRights2string(IN LPTSTR buffer, IN DWORD size, IN DWORD accessRight);

// convert quality to string
OPCENGINE_EXPORT LPCTSTR quality2string(IN CString *buffer, IN WORD quality);
OPCENGINE_EXPORT LPCTSTR quality2string(IN LPTSTR buffer, IN DWORD size, IN WORD quality);

// convert filetime to string
OPCENGINE_EXPORT LPCTSTR filetime2string(IN CString *buffer, IN LPFILETIME filetime);
OPCENGINE_EXPORT LPCTSTR filetime2string(IN LPTSTR buffer, IN DWORD size, IN LPFILETIME filetime);

// convert interface id into interface description string
OPCENGINE_EXPORT LPCTSTR IID2string(IN CString *buffer,	IN REFIID riid);

#endif
