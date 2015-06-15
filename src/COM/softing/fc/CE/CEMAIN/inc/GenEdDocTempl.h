
#ifndef __GENEDDOCTEMPL_H_
#define __GENEDDOCTEMPL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CGenEdDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC (CGenEdDocTemplate)

public:
	CGenEdDocTemplate (bool bTabbed);

	bool IsTabbed ();

    bool SaveAll (bool bAskBeforeSave);

protected:
	virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc,
		BOOL bMakeVisible = TRUE);

protected:
	bool m_bTabbed;
};


#endif // __GENEDDOCTEMPL_H_

