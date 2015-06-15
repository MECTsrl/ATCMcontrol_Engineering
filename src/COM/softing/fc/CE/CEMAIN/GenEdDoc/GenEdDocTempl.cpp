

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "GenEdDocTempl.h"
#include "GenEdDoc.h"
#include "GenEdFrame.h"
#include "GenEdView.h"
#include "GenEdRC.h"

#include <assert.h>

IMPLEMENT_DYNAMIC (CGenEdDocTemplate, CMultiDocTemplate)


CGenEdDocTemplate::CGenEdDocTemplate (bool bTabbed)
: CMultiDocTemplate (IDR_GENEDTYPE, RUNTIME_CLASS (CGenEdDoc), RUNTIME_CLASS (CGenEdFrame), RUNTIME_CLASS (CGenEdView))
{
	m_bTabbed = bTabbed;
}


bool CGenEdDocTemplate::IsTabbed ()
{
	return (m_bTabbed);
}


bool CGenEdDocTemplate::SaveAll (bool bAskBeforeSave)
{
    POSITION pPosition;
    
    pPosition = GetFirstDocPosition ();
    while (pPosition != NULL)
    {
        CGenEdDoc *pDoc;
        
        pDoc = dynamic_cast<CGenEdDoc *> (GetNextDoc (pPosition));
        assert (pDoc != NULL);
        if (pDoc == NULL)
        {
            continue;
        }

        pDoc->EnableParsing(FALSE);
        if (!pDoc->SaveIfModified (bAskBeforeSave))
        {
            pDoc->EnableParsing(TRUE);
            return (false);
        }
        pDoc->EnableParsing(TRUE);
    }

    return (true);
}

void CGenEdDocTemplate::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc,
	BOOL bMakeVisible)
{
    CGenEdFrame*    pGenEdFrame = dynamic_cast<CGenEdFrame*> (pFrame);
    pGenEdFrame->LoadWindowPosition(pDoc->GetPathName());

	// just delagate to implementation in CFrameWnd
	pFrame->InitialUpdateFrame(pDoc, bMakeVisible);
}

