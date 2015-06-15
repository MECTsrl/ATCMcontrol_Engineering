/////////////////////////////////////////////////////////////////////////////
// class.h : Declaration of SECClass
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       <NAME>
// Description:  Declaration of SECClass
//

#ifndef __SECCLASS_H__
#define __SECCLASS_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

// Header Includes
//#ifndef __SECOTHER_H__
//#include "toolkit\secother.h"
//#endif

//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

// Constants

// Macros

// Types

// Forward Declarations

/////////////////////////////////////////////////////////////////////////////
// SECClass

class SECClass : public SECBaseClass
{
	DECLARE_DYNAMIC(SECClass);

	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//
public:

	// Initialization/Destruction
	SECClass();
	virtual ~SECClass();

	// Attributes
	// ...Publicly accessible member variables...

	// Accessors
	// ...Get and Set functions...

	// Operations

	//---------------------------------------------------------------------//
	// 							 Overridables							   //
	//---------------------------------------------------------------------//
protected:

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:

	// Implementation Member Variables

	// Implementation Member Functions

	//---------------------------------------------------------------------//
	// 						  Base Class Overrides						   //
	//---------------------------------------------------------------------//

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECClass)
	//}}AFX_VIRTUAL

	//---------------------------------------------------------------------//
	// 						Message Map Declarations					   //
	//---------------------------------------------------------------------//

	// Generated message map functions
	//{{AFX_MSG(SECClass)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//---------------------------------------------------------------------//
	// 						Private Implementation						   //
	//---------------------------------------------------------------------//
private:

};

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif //__SECCLASS_H__
