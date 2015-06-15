/////////////////////////////////////////////////////////////////////////////
// PageSettings.h : SFL Print Preview package 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       
// Description:  
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


namespace stingray {
namespace foundation {


//////////////////////////////////////////////////////////////////////////
// CPageLayout

class CPageLayout
{
// Constructors/destructor
public:
	CPageLayout()
	{
		m_nFromPage = 0;
		m_nToPage = 0;
		m_rcMargins.left = 0;
		m_rcMargins.top = 0;
		m_rcMargins.right = 0;
		m_rcMargins.bottom = 0;
	}

// Attributes
protected:
	int m_nFromPage;
	int m_nToPage;
	RECT m_rcMargins;

// Operations
public:
	int GetFromPage() const
	{
		return m_nFromPage;
	}

	int GetToPage() const
	{
		return m_nToPage;
	}

	RECT& GetMargins()
	{
		return m_rcMargins;
	}
};

//////////////////////////////////////////////////////////////////////////
// CPageSettings

template <class _PageLayout = CPageLayout>
class CPageSettingsBase
{
// Embedded types
public:
	typedef _PageLayout PageLayoutClass;

// Constructors/destructor
public:
	CPageSettingsBase() :
		m_nFromPage(-1),
		m_nToPage(-1)
	{
	}

// Attributes
protected:
	int m_nFromPage;
	int m_nToPage;

// Operations
public:
	virtual void LoadPageSetupDlg(PAGESETUPDLG& psd)
	{
		// to get rid of the warnings
		psd;
	}

	virtual void StorePageSetupDlg(const PAGESETUPDLG& psd)
	{
		// to get rid of the warnings
		psd;
	}

	int GetFromPage() const
	{
		return m_nFromPage;
	}

	int GetToPage() const
	{
		return m_nToPage;
	}

	virtual PageLayoutClass* GetPageLayout(const int nPageNo) = 0;
};

//////////////////////////////////////////////////////////////////////////
// CPageSettings

class CPageSettings : public CPageSettingsBase<CPageLayout>
{
// Constructors/destructor
public:
	CPageSettings()
	{
	}

// Attributes
protected:
	CPageLayout m_pageLayout;

// Operations
public:
	CPageLayout* GetPageLayout(const int nPageNo)
	{
		nPageNo;
		return &m_pageLayout;
	}
};



};	// namespace stingray::foundation
};	// namespace stingray
