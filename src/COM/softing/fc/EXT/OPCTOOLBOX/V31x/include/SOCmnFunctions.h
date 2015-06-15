//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOCmn                              |
//                                                                            |
//  Filename    : SOCmnFunctions.h                                            |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Helper Functions                                            |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCMNFUNCTIONS_H_
#define _SOCMNFUNCTIONS_H_

// allocate and free memory
SOCMN_EXPORT void freeMemory(IN void *memptr);
SOCMN_EXPORT void* allocateMemory(IN ULONG size);

// get the time span between two times
SOCMN_EXPORT ULONG getTimeSpan(IN DWORD from, IN DWORD till);

// covert COM enum to string list
SOCMN_EXPORT void enumString2stringList(IN IEnumString *enumString,
	OUT SOCmnStringList *stringList);


// old macros - don't use in new code
#define EMPTYSTRING(a) 
#define INITSTRING(a) 
#define SETSTRING(a, b) { a = b; } 
#define ADDSTRING(a, b) (a).add(b); 
#define EMPTYSTRINGLIST(a) { (a).removeAll();}
#define ADDPOINTER(a, b) (a).add(b); 
#define EMPTYPTRLIST(a) { (a).removeAll();}
#define FREEMEMORY(a) ::freeMemory(a);
#define ALLOCATEMEMORY(a) ::allocateMemory(a); 


#endif
