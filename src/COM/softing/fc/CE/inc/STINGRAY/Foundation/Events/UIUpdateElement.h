/////////////////////////////////////////////////////////////////////////////
// UIUpdateElement.h : Interface for UIUpdate elements.
//
// Stingray Software Extension Classes
// Copyright (C) 1999 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Authors:      Jeff Boenig and George Shepherd
// Description:  Framework independent event model.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Events/UIUpdateElement.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __UIUPDATEELEMENT_H__
#define __UIUPDATEELEMENT_H__

#include <foundation\SflGuids.h>
#include <foundation\SflCommon.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc IUIUpdateElement
//
//@class IUIUpdateElement | Interface for user interface elements that
// can be updated by a UI update listener. Provides a consistent
// interface to UI update listeners for modifying menus, toolbars,
// status bars, etc.
//
//@base public | IRefCount, IQueryGuid
//
class __declspec(uuid("85A9C953-7B7D-11d3-8160-00C04F8D83A6"))
				IUIUpdateElement : public IRefCount, public IQueryGuid
{
public:
	//@cmember
	/* Call this to the te the text of the interface element. */
	virtual void SetText(const UINT nCommandID, LPCTSTR) = 0;

	//@cmember
	/* Call this to enable or disable the interface element. */
	virtual void Enable(const UINT nCommandID, bool bEnable = true) = 0;

	//@cmember
	/* Call this to turn on or off a radio button within the interface element. */
	virtual void SetRadio(const UINT nCommandID, bool bEnable = true) = 0;

	//@cmember
	/* Call this to turn on or off a check button within the interface element. */
	virtual void SetCheck(const UINT nCommandID, int nState = 1) = 0;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __UIUPDATEELEMENT_H__
