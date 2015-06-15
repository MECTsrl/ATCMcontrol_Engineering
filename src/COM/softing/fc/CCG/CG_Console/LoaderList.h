
#ifndef __LOADERLIST_H_
#define __LOADERLIST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <fc_todebug\fc_assert.h>
#include <fc_tools\fc_cstring.h>
#include <fc_tools\fc_hash.h>
#include <CGT_CDiagMsg.h>

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// LoaderList.h: interface for the LoaderList class.
//
//////////////////////////////////////////////////////////////////////
enum eFileRel {FILE_GEN_VIS_RELATIVE, FILE_PRJ_DIR_RELATIVE};

class CLoaderList  
{
	FC_CHashStr<eFileRel> m_hsLoaderList; // list of files to be downloaded, 
										  // pVal shows if it is project or gen vis folder relativ	
	FC_CString			  m_sVisDir;
	int					  m_nVisDirLen;

public:
	CLoaderList();
	~CLoaderList();

	void init(const TCHAR* pszVisDir);
	bool addFile(const TCHAR* pszFile, 
		eFileRel lIsPrjRel = FILE_GEN_VIS_RELATIVE); // return true if File created, false if it already exists
													 // bIsPrjRel file should be project relativ or vis gen folder
	bool dump(CGT_CDiagMsg* p_dm, const TCHAR* pszPath, const TCHAR* pszHeader);
	long getCount();
};

#endif // __LOADERLIST_H_

