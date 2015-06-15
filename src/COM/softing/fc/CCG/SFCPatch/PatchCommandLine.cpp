

#include "stdafx.h"
#include "PatchCommandLine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPatchCommandLine::CPatchCommandLine()
{
	m_bOptionF=FALSE;
}

CPatchCommandLine::~CPatchCommandLine()
{

}

int CPatchCommandLine::Parse(int argc, char** argv)
{
	if (argc != 5 && argc != 6)
	{
		return -1;
	}

	//
	// get Project
	//
	if ( argc>=2 )
	{
		m_strProjectPath = argv[1];
	}

	//
	// get gr file
	//
	if ( argc >=3 )
	{
		m_strGrFile = argv[2];
	}

	//
	// get pou or dbi file
	//
	if (argc >= 4)
	{
		m_strSFCPath = argv[3];
	}

	//
	// get output file
	//
	if (argc >= 5)
	{
		m_strPatchFile = argv[4];
	}

	//
	// get option
	//
	if (argc >= 6)
	{
		CString strOption = argv[5];
		if (strOption.CompareNoCase(_T("-f"))==0 || strOption.CompareNoCase(_T("/f"))==0)
		{
			m_bOptionF = TRUE;
		}
		else
		{
			return -1;
		}
	}

	return 0;
}