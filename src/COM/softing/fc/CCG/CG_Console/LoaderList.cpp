

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "loaderlist.h"
#include "defines.h"
#include <cgt_misc.h>

//----  Static Initializations:   ----------------------------------*

// LoaderList.cpp: implementation of the LoaderList class.
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoaderList::CLoaderList():
	m_hsLoaderList(true, true, 1024),
	m_sVisDir(MAX_PATH)
{
}

CLoaderList::~CLoaderList()
{
}

void CLoaderList::init(const TCHAR* pszVisDir)
{
	m_sVisDir.load(pszVisDir);
	m_sVisDir.toLower();
	m_nVisDirLen = m_sVisDir.len();
}

long CLoaderList::getCount()
{
	return m_hsLoaderList.countPairs();
}


bool CLoaderList::addFile(const TCHAR* pszFile, eFileRel eIsPrjRel)
{
	return m_hsLoaderList.insertEx(pszFile, eIsPrjRel, false, NULL);
}

bool CLoaderList::dump(CGT_CDiagMsg* p_dm, const TCHAR* pszPath, const TCHAR* pszHeader)
{
	CGT_CDmp dmpLoaderList;

	dmpLoaderList << pszHeader;

	for(const FC_CHashStr<eFileRel>::Pair* p = m_hsLoaderList.firstPair(); p; p = p->pNext)
	{
		switch(p->pVal)
		{
		case FILE_GEN_VIS_RELATIVE:
			dmpLoaderList << m_sVisDir;
			break;
		case FILE_PRJ_DIR_RELATIVE:
			dmpLoaderList << _T("..\\");
			break;
		}

		dmpLoaderList << p->pszKey << _T(";") << p->pszKey << _T("\n");
	}

	return CGT_DmpToFile(p_dm, pszPath, VIS_LOADER_LIST_FILE, dmpLoaderList, false);
}

