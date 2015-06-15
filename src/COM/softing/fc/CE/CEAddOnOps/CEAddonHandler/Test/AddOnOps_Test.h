// AddOnOps_Test.h: interface for the AddOnOps_Test class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDONOPS_TEST_H__EA38A363_E717_11D4_B8D2_002018641833__INCLUDED_)
#define AFX_ADDONOPS_TEST_H__EA38A363_E717_11D4_B8D2_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEAddonAction.h"
#include "CEProjInfo.h"

class AddOnOps_Test  
{
public:
	void invoke(LPCTSTR pszDispId,
                LPCTSTR pszSourceFile,
                LPCTSTR pszId,
                LPCTSTR pszAdditionalInfo);
	AddOnOps_Test();
	virtual ~AddOnOps_Test();
	BOOL connect();
	BOOL disconnect();
	BOOL connectProjInfo();
    BOOL disconnectProjInfo();
	ICEAddonAction * m_pIAddOnAction;
	IUnknown * m_pIUnkProjInfo;

private:
	void DispMessage(DWORD dwErrCode);
};

#endif // !defined(AFX_ADDONOPS_TEST_H__EA38A363_E717_11D4_B8D2_002018641833__INCLUDED_)
