
#if !defined __EXTERN__
#define __EXTERN__

#include "CEProjInfo.h"

CString GetInstallationPath();
CString GetProjectPath();
CString GetDocuPath();
void SetProjInfo(ICEProjInfo *pICEProjectInfo);
const TCHAR* const * GetSupportedBaseTypes();
const TCHAR* const * GetProgramClasses();
HRESULT GetSupportedIECLanguages(CStringArray* pStringArray);
BOOL IsRetainSupported();

#endif
