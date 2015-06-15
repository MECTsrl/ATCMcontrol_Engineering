
#if !defined(AFX_OBJPROPGENBUTTONBASE_H__C7E62905_D5C8_11D4_962B_00A024399A66__INCLUDED_)
#define AFX_OBJPROPGENBUTTONBASE_H__C7E62905_D5C8_11D4_962B_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjPropGenBase.h"

class CObjPropGenButtonBase  
{
public:
	CObjPropGenButtonBase();
	virtual ~CObjPropGenButtonBase();

	CButton	m_NewVersionID;

protected:
   	virtual void DoDataExchange(CDataExchange* pDX);
};

#endif // !defined(AFX_OBJPROPGENBUTTONBASE_H__C7E62905_D5C8_11D4_962B_00A024399A66__INCLUDED_)
