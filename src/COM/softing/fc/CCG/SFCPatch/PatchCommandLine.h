
#if !defined(_PATCHCOMMANDLINE_H)
#define _PATCHCOMMANDLINE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPatchCommandLine  
{
public:
	CPatchCommandLine();
	virtual ~CPatchCommandLine();
	int Parse(int argc, char** argv);

public:
	CString m_strProjectPath;
	CString m_strGrFile;
	CString m_strSFCPath;
	CString m_strPatchFile;
	BOOL m_bOptionF;
};

#endif // !defined(_PATCHCOMMANDLINE_H)
