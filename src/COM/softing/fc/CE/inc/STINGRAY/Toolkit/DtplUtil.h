/////////////////////////////////////////////////////////////////////////////
// DtplUtil.h : Declaration of SECDocTemplateUtils
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
// Author:       Mark Isham
// Description:  Declaration of SECDocTemplateUtils
//
// Utility class providing useful functions for examining and manipulating
// data associated with CDocTemplate and its derived classes.

#ifndef __DOCTEMPLATE_UTILS_H__
#define __DOCTEMPLATE_UTILS_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

// Header Includes

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

typedef CTypedPtrList<CPtrList,CDocTemplate*> SECDocTemplatePtrList;

// Forward Declarations

/////////////////////////////////////////////////////////////////////////////
// SECDocTemplateUtils

class SECDocTemplateUtils
{
	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//
public:

	// Initialization/Destruction
	SECDocTemplateUtils() 
	{
		m_bUseDocTypeID = TRUE;
	}
	virtual ~SECDocTemplateUtils() {}

	// Attributes
	// ...Publicly accessible member variables...

	// Accessors
	// ...Get and Set functions...

	// Operations
	virtual int GetDocTemplateIndex(CDocTemplate* pDocTemplate,
										const CString& strDocType);
	virtual CDocTemplate* GetDocTemplateAtIndex(const CString& strDocType,
										int nDocTemplateIndex);
	virtual CDocTemplate* FindDocTemplateBestGuess(CDocument* pDoc,
								CView* pView,CFrameWnd* pFrame);
	virtual UINT GetFrameResourceId(CFrameWnd* pFrame);

	BOOL IsUseDocTypeID()
	{
		return m_bUseDocTypeID;
	}

	void SetUseDocTypeID(BOOL bUseDocTypeID )
	{
		m_bUseDocTypeID = bUseDocTypeID;
	}

	//---------------------------------------------------------------------//
	// 							 Overridables							   //
	//---------------------------------------------------------------------//
protected:

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:


	// Implementation Member Variables

	// Internal classes to provide needed access to MFC protected data.
	class SECDocTemplateProtectedAccess : public CDocTemplate {
	public:
		SECDocTemplateProtectedAccess();
		friend class SECDocTemplateUtils; 
	};
	class SECFrameWndProtectedAccess : public CFrameWnd {
		friend class SECDocTemplateUtils; 
	};

	// Implementation Member Functions
	virtual BOOL BuildDocTemplateList(SECDocTemplatePtrList& listMatches);
	virtual BOOL SearchDTByViewFrame(SECDocTemplatePtrList& listMatches,
								CView* pView,CFrameWnd* pFrame);
	virtual BOOL SearchDTByResource(SECDocTemplatePtrList& listMatches,
								CFrameWnd* pFrame);
	virtual BOOL SearchDTByDocument(SECDocTemplatePtrList& listMatches,
								CDocument* pDoc);
	virtual BOOL DoReturnFromFindDTBestGuess(const SECDocTemplatePtrList& listMatches,
								CDocTemplate*& pDocTemplateMatch);

	// OT-Pro aware functions. 
	virtual BOOL IsProDockFrame(CRuntimeClass* pFrameClass);
	virtual CRuntimeClass* GetAppropFrameClass(const CDocTemplate* pDTTest,
								BOOL bIsDockableFrame);
	//---------------------------------------------------------------------//
	// 						  Base Class Overrides						   //
	//---------------------------------------------------------------------//


	//---------------------------------------------------------------------//
	// 						Private Implementation						   //
	//---------------------------------------------------------------------//
private:
	BOOL	m_bUseDocTypeID;
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

#endif	// WIN32

#endif	//__DOCTEMPLATE_UTILS_H__
