#if !defined(AFX_SIZEDCONTROLBAR_H__DAF638DE_937A_4736_8D82_FAAA0B47B3AF__INCLUDED_)
#define AFX_SIZEDCONTROLBAR_H__DAF638DE_937A_4736_8D82_FAAA0B47B3AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSizedControlBar  : public SECControlBar
{

DECLARE_DYNCREATE(CSizedControlBar)

public:
	CSizedControlBar();
	virtual ~CSizedControlBar();

	virtual void OnBarEndDock();
   	virtual void OnBarEndFloat();
	virtual void OnBarEndMDIFloat();

};

#endif // !defined(AFX_SIZEDCONTROLBAR_H__DAF638DE_937A_4736_8D82_FAAA0B47B3AF__INCLUDED_)



