/////////////////////////////////////////////////////////////////////////////
// PrintPreviewController.h : Print preview controller class.
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Print preview controller class.
//

#pragma once

#include <Foundation\Print\PrintPreviewModel.h>
#include <Foundation\Events\CommandListener.h>
#include <Foundation\Events\KeyboardListener.h>
#include <Foundation\MVC\MvcVisualComponent.h>
#include <Foundation\MVC\MvcController.h>
#include <Foundation\Debug.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc CPrtPreviewController

class CPrtPreviewController : public CMvcController<CPrtPreviewModel, CMvcVisualComponent>,
                              public CCommandAdapter,
                              public CKeyboardAdapter
{
public:
	CPrtPreviewController()
	{
		AddListener(static_cast<CCommandAdapter*>(this));
	}

	virtual bool OnCommand(UINT nID, int nNotifyCode)
	{
		// To get rid of warning
		nID;
		nNotifyCode;

		return false;
	}

	virtual bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// to get rid of warning
		nRepCnt;
		nFlags;

		bool bHandled = false;
		CPrtPreviewModel* pModel = GetModel();
		if (pModel != NULL)
		{
			if (nChar == VK_NEXT)
			{
				pModel->GotoNextPage();
				bHandled = true;
			}
		}
		return bHandled;
	}
};

}; // namespace stingray::foundation
}; // namespace stingray

