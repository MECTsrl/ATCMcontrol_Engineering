// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Steve Schauer
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved.
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
//
// EditCore.cpp
//
//
// Description: Low level SECEdit routines
//

#include "stdafx.h"
#include "secedit.h"

// AUTOFMT 08.08.06 SIS >>
#ifndef ORIGINAL_VERSION
SECEditSettings::SECEditSettings(const CString& crsFileMappingKey /*=_T("")*/)
{
    m_sFileMappingKey       = crsFileMappingKey;

    m_bInitMode             = TRUE;

    m_iCharSet              = -1;
    m_iFontWeight           = -1;
    m_bFontItalic           = FALSE;
    m_crTextColDefault      = 0;
    m_crTextColKeyword      = 0;
    m_crTextColComment      = 0;
    m_crTextColQuote        = 0;
    m_crTextColSQuote       = 0;
    m_crBackgroundCol       = 0;
    m_bChroma               = FALSE;
    m_bAutoIndent           = FALSE;
    m_bCase                 = FALSE;
    m_bKeepTabs             = FALSE;
    m_bShowTabs             = FALSE;
    m_iTabSize              = 0;
    m_bWordWrap             = FALSE;
    m_bInsert               = FALSE;
    m_bLockFiles            = FALSE;

    m_dwUsed                = 0;
    m_dwChanged             = 0;
}

SECEditSettings::~SECEditSettings()
{
}

void SECEditSettings::SetFileMappingKey(const CString& crsFileMappingKey)
{
    m_sFileMappingKey = crsFileMappingKey;
}

CString SECEditSettings::GetFileMappingKey()
{
    return m_sFileMappingKey;
}

BOOL SECEditSettings::IsInitMode()
{
    return m_bInitMode;
}

void SECEditSettings::SetInitMode(BOOL bInitMode)
{
    m_bInitMode = bInitMode;
}

BOOL SECEditSettings::LoadFromRegistry()
{
    if(m_sFileMappingKey.IsEmpty())
    {
        return FALSE;
    }
    CWinApp* pApp = AfxGetApp();

    CString     sTemp;
    BOOL        bTemp;
    int         iTemp;
    COLORREF    crTemp;

    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_EXTRAWORDCHARS, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetExtraWordChars(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_TERMINATORCHARS, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetTerminatorChars(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_STARTCHARS, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetStartChars(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_KEYWORDFILE, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetKeywordFile(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTSTART1, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetCommentStart1(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTEND1, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetCommentEnd1(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTSTART2, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetCommentStart2(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTEND2, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetCommentEnd2(sTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_FONTNAME, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        SetFontName(sTemp);
    }

    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_FONTSIZE, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        SetFontSize(iTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_CHARSET, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        SetCharSet(iTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_FONTWEIGHT, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        SetFontWeight(iTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_FONTITALIC, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetFontItalic(bTemp);
    }

    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLDEFAULT, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        crTemp = SECEdit::COLORREFFromString(T2CA(sTemp));
        SetTextColDefault(crTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLKEYWORD, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        crTemp = SECEdit::COLORREFFromString(T2CA(sTemp));
        SetTextColKeyword(crTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLCOMMENT, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        crTemp = SECEdit::COLORREFFromString(T2CA(sTemp));
        SetTextColComment(crTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLQUOTE, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        crTemp = SECEdit::COLORREFFromString(T2CA(sTemp));
        SetTextColQuote(crTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLSQUOTE, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        crTemp = SECEdit::COLORREFFromString(T2CA(sTemp));
        SetTextColSQuote(crTemp);
    }
    sTemp = pApp->GetProfileString(m_sFileMappingKey, OE_REGKEY_BACKGROUNDCOL, UNUSED_STRING);
    if(sTemp.Compare(UNUSED_STRING) != 0)
    {
        crTemp = SECEdit::COLORREFFromString(T2CA(sTemp));
        SetBackgroundCol(crTemp);
    }

    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_CHROMA, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetChroma(bTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_AUTOINDENT, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetAutoIndent(bTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_CASE, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetCase(bTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_KEEPTABS, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetKeepTabs(bTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_SHOWTABS, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetShowTabs(bTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_TABSIZE, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        SetTabSize(iTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_WORDWRAP, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetWordWrap(bTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_INSERT, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetInsert(bTemp);
    }
    iTemp = pApp->GetProfileInt(m_sFileMappingKey, OE_REGKEY_LOCKFILES, UNUSED_VALUE);
    if(iTemp != UNUSED_VALUE)
    {
        bTemp = iTemp ? TRUE : FALSE;
        SetLockFiles(bTemp);
    }
	return TRUE;
}

BOOL SECEditSettings::SaveToRegistry()
{
	if(m_sFileMappingKey.IsEmpty())
	{
		return FALSE;
	}
	CWinApp* pApp = AfxGetApp();
	char szColor[64];

    if(IsUsed(OE_FLAG_EXTRAWORDCHARS))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_EXTRAWORDCHARS, m_sExtraWordChars);
        m_dwChanged &= ~OE_FLAG_EXTRAWORDCHARS;
    }
    if(IsUsed(OE_FLAG_TERMINATORCHARS))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_TERMINATORCHARS, m_sTerminatorChars);
        m_dwChanged &= ~OE_FLAG_TERMINATORCHARS;
    }
    if(IsUsed(OE_FLAG_STARTCHARS))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_STARTCHARS, m_sStartChars);
        m_dwChanged &= ~OE_FLAG_STARTCHARS;
    }
    if(IsUsed(OE_FLAG_KEYWORDFILE))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_KEYWORDFILE, m_sKeywordFile);
        m_dwChanged &= ~OE_FLAG_STARTCHARS;
    }
    if(IsUsed(OE_FLAG_COMMENTSTART1))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTSTART1, m_sCommentStart1);
        m_dwChanged &= ~OE_FLAG_COMMENTSTART1;
    }
    if(IsUsed(OE_FLAG_COMMENTEND1))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTEND1, m_sCommentEnd1);
        m_dwChanged &= ~OE_FLAG_COMMENTEND1;
    }
    if(IsUsed(OE_FLAG_COMMENTSTART2))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTSTART2, m_sCommentStart2);
        m_dwChanged &= ~OE_FLAG_COMMENTSTART2;
    }
    if(IsUsed(OE_FLAG_COMMENTEND2))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_COMMENTEND2, m_sCommentEnd2);
        m_dwChanged &= ~OE_FLAG_COMMENTEND2;
    }
    if(IsUsed(OE_FLAG_FONTNAME))
    {
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_FONTNAME, m_sFontName);
        m_dwChanged &= ~OE_FLAG_FONTNAME;
    }
    if(IsUsed(OE_FLAG_FONTSIZE))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_FONTSIZE, m_iFontSize);
        m_dwChanged &= ~OE_FLAG_FONTSIZE;
    }
    if(IsUsed(OE_FLAG_CHARSET))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_CHARSET, m_iCharSet);
        m_dwChanged &= ~OE_FLAG_CHARSET;
    }
    if(IsUsed(OE_FLAG_FONTWEIGHT))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_FONTWEIGHT, m_iFontWeight);
        m_dwChanged &= ~OE_FLAG_FONTWEIGHT;
    }
    if(IsUsed(OE_FLAG_FONTITALIC))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_FONTITALIC, m_bFontItalic);
        m_dwChanged &= ~OE_FLAG_FONTITALIC;
    }
    if(IsUsed(OE_FLAG_TEXTCOLDEFAULT))
    {
        SECEdit::StringFromCOLORREF(m_crTextColDefault, szColor);
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLDEFAULT, szColor);
        m_dwChanged &= ~OE_FLAG_TEXTCOLDEFAULT;
    }
    if(IsUsed(OE_FLAG_TEXTCOLKEYWORD))
    {
        SECEdit::StringFromCOLORREF(m_crTextColKeyword, szColor);
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLKEYWORD, szColor);
        m_dwChanged &= ~OE_FLAG_TEXTCOLKEYWORD;
    }
    if(IsUsed(OE_FLAG_TEXTCOLCOMMENT))
    {
        SECEdit::StringFromCOLORREF(m_crTextColComment, szColor);
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLCOMMENT, szColor);
        m_dwChanged &= ~OE_FLAG_TEXTCOLCOMMENT;
    }
    if(IsUsed(OE_FLAG_TEXTCOLQUOTE))
    {
        SECEdit::StringFromCOLORREF(m_crTextColQuote, szColor);
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLQUOTE, szColor);
        m_dwChanged &= ~OE_FLAG_TEXTCOLQUOTE;
    }
    if(IsUsed(OE_FLAG_TEXTCOLSQUOTE))
    {
        SECEdit::StringFromCOLORREF(m_crTextColSQuote, szColor);
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_TEXTCOLSQUOTE, szColor);
        m_dwChanged &= ~OE_FLAG_TEXTCOLSQUOTE;
    }
    if(IsUsed(OE_FLAG_BACKGROUNDCOL))
    {
        SECEdit::StringFromCOLORREF(m_crBackgroundCol, szColor);
        pApp->WriteProfileString(m_sFileMappingKey, OE_REGKEY_BACKGROUNDCOL, szColor);
        m_dwChanged &= ~OE_FLAG_BACKGROUNDCOL;
    }
    if(IsUsed(OE_FLAG_CHROMA))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_CHROMA, m_bChroma);
        m_dwChanged &= ~OE_FLAG_CHROMA;
    }
    if(IsUsed(OE_FLAG_AUTOINDENT))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_AUTOINDENT, m_bAutoIndent);
        m_dwChanged &= ~OE_FLAG_AUTOINDENT;
    }
    if(IsUsed(OE_FLAG_CASE))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_CASE, m_bCase);
        m_dwChanged &= ~OE_FLAG_CASE;
    }
    if(IsUsed(OE_FLAG_KEEPTABS))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_KEEPTABS, m_bKeepTabs);
        m_dwChanged &= ~OE_FLAG_KEEPTABS;
    }
    if(IsUsed(OE_FLAG_SHOWTABS))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_SHOWTABS, m_bShowTabs);
        m_dwChanged &= ~OE_FLAG_SHOWTABS;
    }
    if(IsUsed(OE_FLAG_TABSIZE))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_TABSIZE, m_iTabSize);
        m_dwChanged &= ~OE_FLAG_TABSIZE;
    }
    if(IsUsed(OE_FLAG_WORDWRAP))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_WORDWRAP, m_bWordWrap);
        m_dwChanged &= ~OE_FLAG_WORDWRAP;
    }
    if(IsUsed(OE_FLAG_INSERT))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_INSERT, m_bInsert);
        m_dwChanged &= ~OE_FLAG_INSERT;
    }
    if(IsUsed(OE_FLAG_LOCKFILES))
    {
        pApp->WriteProfileInt(m_sFileMappingKey, OE_REGKEY_LOCKFILES, m_bLockFiles);
        m_dwChanged &= ~OE_FLAG_LOCKFILES;
    }

	return TRUE;
}

BOOL SECEditSettings::IsUsed(DWORD dwFlag)
{
    return (m_dwUsed & dwFlag) > 0;
}

DWORD SECEditSettings::GetUsedFlags()
{
    return m_dwUsed;
}

//void SECEditSettings::SetUsedFlags(DWORD dwFlags)
//{
//    m_dwUsed = dwFlags;
//}

BOOL SECEditSettings::IsChanged(DWORD dwFlag)
{
    return (m_dwChanged & dwFlag) > 0;
}

DWORD SECEditSettings::GetChangedFlags()
{
    return m_dwChanged;
}

//void SECEditSettings::SetChangedFlags(DWORD dwFlags)
//{
//    m_dwChanged = dwFlags;
//}

// general settings
CString SECEditSettings::GetExtraWordChars()
{
    return m_sExtraWordChars;
}

void SECEditSettings::SetExtraWordChars(const CString& crsExtraWordChars)
{
    m_dwUsed |= OE_FLAG_EXTRAWORDCHARS;
    if(m_sExtraWordChars.Compare(crsExtraWordChars) == 0)
    {
        return;
    }
    m_sExtraWordChars = crsExtraWordChars;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_EXTRAWORDCHARS;
    }
}

CString SECEditSettings::GetTerminatorChars()
{
    return m_sTerminatorChars;
}

void SECEditSettings::SetTerminatorChars(const CString& crsTerminatorChars)
{
    m_dwUsed |= OE_FLAG_TERMINATORCHARS;
    if(m_sTerminatorChars.Compare(crsTerminatorChars) == 0)
    {
        return;
    }
    m_sTerminatorChars = crsTerminatorChars;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_TERMINATORCHARS;
    }
}

CString SECEditSettings::GetStartChars()
{
    return m_sStartChars;
}

void SECEditSettings::SetStartChars(const CString& crsStartChars)
{
    m_dwUsed |= OE_FLAG_STARTCHARS;
    if(m_sStartChars.Compare(crsStartChars) == 0)
    {
        return;
    }
    m_sStartChars = crsStartChars;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_STARTCHARS;
    }
}

CString SECEditSettings::GetKeywordFile()
{
    return m_sKeywordFile;
}

void SECEditSettings::SetKeywordFile(const CString& crsKeywordFile)
{
    m_dwUsed |= OE_FLAG_KEYWORDFILE;
    if(m_sKeywordFile.Compare(crsKeywordFile) == 0)
    {
        return;
    }
    m_sKeywordFile = crsKeywordFile;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_KEYWORDFILE;
    }
}


// comment settings
CString SECEditSettings::GetCommentStart1()
{
    return m_sCommentStart1;
}

void SECEditSettings::SetCommentStart1(const CString& crsCommentStart1)
{
    m_dwUsed |= OE_FLAG_COMMENTSTART1;
    if(m_sCommentStart1.Compare(crsCommentStart1) == 0)
    {
        return;
    }
    m_sCommentStart1 = crsCommentStart1;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_COMMENTSTART1;
    }
}

CString SECEditSettings::GetCommentEnd1()
{
    return m_sCommentEnd1;
}

void SECEditSettings::SetCommentEnd1(const CString& crsCommentEnd1)
{
    m_dwUsed |= OE_FLAG_COMMENTEND1;
    if(m_sCommentEnd1.Compare(crsCommentEnd1) == 0)
    {
        return;
    }
    m_sCommentEnd1 = crsCommentEnd1;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_COMMENTEND1;
    }
}

CString SECEditSettings::GetCommentStart2()
{
    return m_sCommentStart2;
}

void SECEditSettings::SetCommentStart2(const CString& crsCommentStart2)
{
    m_dwUsed |= OE_FLAG_COMMENTSTART2;
    if(m_sCommentStart2.Compare(crsCommentStart2) == 0)
    {
        return;
    }
    m_sCommentStart2 = crsCommentStart2;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_COMMENTSTART2;
    }
}

CString SECEditSettings::GetCommentEnd2()
{
    return m_sCommentEnd2;
}

void SECEditSettings::SetCommentEnd2(const CString& crsCommentEnd2)
{
    m_dwUsed |= OE_FLAG_COMMENTEND2;
    if(m_sCommentEnd2.Compare(crsCommentEnd2) == 0)
    {
        return;
    }
    m_sCommentEnd2 = crsCommentEnd2;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_COMMENTEND2;
    }
}


// font settings
CString SECEditSettings::GetFontName()
{
    return m_sFontName;
}

void SECEditSettings::SetFontName(const CString& crsFontName)
{
    m_dwUsed |= OE_FLAG_FONTNAME;
    if(m_sFontName.Compare(crsFontName) == 0)
    {
        return;
    }
    m_sFontName = crsFontName;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_FONTNAME;
    }
}

int SECEditSettings::GetFontSize()
{
    return m_iFontSize;
}

void SECEditSettings::SetFontSize(int iFontSize)
{
    m_dwUsed |= OE_FLAG_FONTSIZE;
    if(m_iFontSize == iFontSize)
    {
        return;
    }
    m_iFontSize = iFontSize;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_FONTSIZE;
    }
}

int SECEditSettings::GetCharSet()
{
    return m_iCharSet;
}

void SECEditSettings::SetCharSet(int iCharSet)
{
    m_dwUsed |= OE_FLAG_CHARSET;
    if(m_iCharSet == iCharSet)
    {
        return;
    }
    m_iCharSet = iCharSet;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_CHARSET;
    }
}

int SECEditSettings::GetFontWeight()
{
    return m_iFontWeight;
}

void SECEditSettings::SetFontWeight(int iFontWeight)
{
    m_dwUsed |= OE_FLAG_FONTWEIGHT;
    if(m_iFontWeight == iFontWeight)
    {
        return;
    }
    m_iFontWeight = iFontWeight;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_FONTWEIGHT;
    }
}

BOOL SECEditSettings::GetFontItalic()
{
    return m_bFontItalic;
}

void SECEditSettings::SetFontItalic(BOOL bFontItalic)
{
    m_dwUsed |= OE_FLAG_FONTITALIC;
    if(m_bFontItalic == bFontItalic)
    {
        return;
    }
    m_bFontItalic = bFontItalic;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_FONTITALIC;
    }
}


// color settings
COLORREF SECEditSettings::GetTextColDefault()
{
    return m_crTextColDefault;
}

void SECEditSettings::SetTextColDefault(COLORREF crTextColDefault)
{
    m_dwUsed |= OE_FLAG_TEXTCOLDEFAULT;
    if(m_crTextColDefault == crTextColDefault)
    {
        return;
    }
    m_crTextColDefault = crTextColDefault;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_TEXTCOLDEFAULT;
    }
}

COLORREF SECEditSettings::GetTextColKeyword()
{
    return m_crTextColKeyword;
}

void SECEditSettings::SetTextColKeyword(COLORREF crTextColKeyword)
{
    m_dwUsed |= OE_FLAG_TEXTCOLKEYWORD;
    if(m_crTextColKeyword == crTextColKeyword)
    {
        return;
    }
    m_crTextColKeyword = crTextColKeyword;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_TEXTCOLKEYWORD;
    }
}

COLORREF SECEditSettings::GetTextColComment()
{
    return m_crTextColComment;
}

void SECEditSettings::SetTextColComment(COLORREF crTextColComment)
{
    m_dwUsed |= OE_FLAG_TEXTCOLCOMMENT;
    if(m_crTextColComment == crTextColComment)
    {
        return;
    }
    m_crTextColComment = crTextColComment;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_TEXTCOLCOMMENT;
    }
}

COLORREF SECEditSettings::GetTextColQuote()
{
    return m_crTextColQuote;
}

void SECEditSettings::SetTextColQuote(COLORREF crTextColQuote)
{
    m_dwUsed |= OE_FLAG_TEXTCOLQUOTE;
    if(m_crTextColQuote == crTextColQuote)
    {
        return;
    }
    m_crTextColQuote = crTextColQuote;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_TEXTCOLQUOTE;
    }
}

COLORREF SECEditSettings::GetTextColSQuote()
{
    return m_crTextColSQuote;
}

void SECEditSettings::SetTextColSQuote(COLORREF crTextColSQuote)
{
    m_dwUsed |= OE_FLAG_TEXTCOLSQUOTE;
    if(m_crTextColSQuote == crTextColSQuote)
    {
        return;
    }
    m_crTextColSQuote = crTextColSQuote;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_TEXTCOLSQUOTE;
    }
}

COLORREF SECEditSettings::GetBackgroundCol()
{
    return m_crBackgroundCol;
}

void SECEditSettings::SetBackgroundCol(COLORREF crBackgroundCol)
{
    m_dwUsed |= OE_FLAG_BACKGROUNDCOL;
    if(m_crBackgroundCol == crBackgroundCol)
    {
        return;
    }
    m_crBackgroundCol = crBackgroundCol;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_BACKGROUNDCOL;
    }
}


// switches and corresponding settings
BOOL SECEditSettings::GetChroma()
{
    return m_bChroma;
}

void SECEditSettings::SetChroma(BOOL bChroma)
{
    m_dwUsed |= OE_FLAG_CHROMA;
    if(m_bChroma == bChroma)
    {
        return;
    }
    m_bChroma = bChroma;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_CHROMA;
    }
}

BOOL SECEditSettings::GetAutoIndent()
{
    return m_bAutoIndent;
}

void SECEditSettings::SetAutoIndent(BOOL bAutoIndent)
{
    m_dwUsed |= OE_FLAG_AUTOINDENT;
    if(m_bAutoIndent == bAutoIndent)
    {
        return;
    }
    m_bAutoIndent = bAutoIndent;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_AUTOINDENT;
    }
}

BOOL SECEditSettings::GetCase()
{
    return m_bCase;
}

void SECEditSettings::SetCase(BOOL bCase)
{
    m_dwUsed |= OE_FLAG_CASE;
    if(m_bCase == bCase)
    {
        return;
    }
    m_bCase = bCase;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_CASE;
    }
}

BOOL SECEditSettings::GetKeepTabs()
{
    return m_bKeepTabs;
}

void SECEditSettings::SetKeepTabs(BOOL bKeepTabs)
{
    m_dwUsed |= OE_FLAG_KEEPTABS;
    if(m_bKeepTabs == bKeepTabs)
    {
        return;
    }
    m_bKeepTabs = bKeepTabs;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_KEEPTABS;
    }
}

BOOL SECEditSettings::GetShowTabs()
{
    return m_bShowTabs;
}

void SECEditSettings::SetShowTabs(BOOL bShowTabs)
{
    m_dwUsed |= OE_FLAG_SHOWTABS;
    if(m_bShowTabs == bShowTabs)
    {
        return;
    }
    m_bShowTabs = bShowTabs;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_SHOWTABS;
    }
}

int SECEditSettings::GetTabSize()
{
    return m_iTabSize;
}

void SECEditSettings::SetTabSize(int iTabSize)
{
    m_dwUsed |= OE_FLAG_TABSIZE;
    if(m_iTabSize == iTabSize || iTabSize < 1)
    {
        return;
    }
    m_iTabSize = iTabSize;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_TABSIZE;
    }
}

BOOL SECEditSettings::GetWordWrap()
{
    return m_bWordWrap;
}

void SECEditSettings::SetWordWrap(BOOL bWordWrap)
{
    m_dwUsed |= OE_FLAG_WORDWRAP;
    if(m_bWordWrap == bWordWrap)
    {
        return;
    }
    m_bWordWrap = bWordWrap;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_WORDWRAP;
    }
}

BOOL SECEditSettings::GetInsert()
{
    return m_bInsert;
}

void SECEditSettings::SetInsert(BOOL bInsert)
{
    m_dwUsed |= OE_FLAG_INSERT;
    if(m_bInsert == bInsert)
    {
        return;
    }
    m_bInsert = bInsert;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_INSERT;
    }
}

BOOL SECEditSettings::GetLockFiles()
{
    return m_bLockFiles;
}

void SECEditSettings::SetLockFiles(BOOL bLockFiles)
{
    m_dwUsed |= OE_FLAG_LOCKFILES;
    if(m_bLockFiles == bLockFiles)
    {
        return;
    }
    m_bLockFiles = bLockFiles;
    if(!m_bInitMode)
    {
        m_dwChanged |= OE_FLAG_LOCKFILES;
    }
}


#endif
// AUTOFMT 08.08.06 SIS <<

/////////////////////////////////////////////////////////////////////
//
// SECEditAlloc
//
//
// Description: Global function that allocates and zeroes memory
//
// Returns:     pointer to allocated memory
//

LPVOID SECEditAlloc(int iSize)
   {
   LPVOID lp;

   lp = malloc(iSize);
   if (lp)
      memset(lp,0,iSize);
   return lp;
   }


//@doc SECEdit
//@mfunc SECEdit constructor - initializes the SECEdit document data.
SECEdit::SECEdit()
   {
	InitEdit();
   }

//@doc SECEdit
//@mfunc SECEdit destructor - frees all memory from the SECEdit.
SECEdit::~SECEdit()
   {
	// if this ASSERT fires it means you have made a call to SetUndoGroup(TRUE) 
	// without a corresponding call to SetUndoGroup(FALSE).  This will result
	// in Undo records being erroneously bundled together for UndoEdit().
	ASSERT(m_dwUndoTimestamp == 0);
	ClearEdit();
   }

//@doc SECEdit
//@mfunc Internal function. Initializes SECEdit data items
//@rdesc void 
void SECEdit::InitEdit()
	{
   lpNames = NULL;
   glpMyIniFileBuffer = NULL;
	lpScanBuffer = NULL;
	m_pLine = MakeNewLine(64);
   m_pUndoBuffer = (PUNDORECORD)SECEditAlloc(sizeof(UNDORECORD) * OE_MAXUNDO);
	SetModified(FALSE);
	SetReadOnly(FALSE);
	ClearFileHandle();
	InitUndo();
	m_iLinesInFile = 1;
	SetFileMappingKey();
	InitFileTypeSettings();
	CString entry;		entry.LoadString(IDS_OE_LINEENDTYPE);
	m_iLineEndType = AfxGetApp()->GetProfileInt(m_strSection,entry,OE_CRLF);
	SetLineEndType(m_iLineEndType);

	}


//@doc SECEdit
//@mfunc Internal function. Releases the linked list of textlines.
//@rdesc void 
void SECEdit::ClearEdit()
	{
	if (m_pLine)
		LineListFree(m_pLine);
	FreeUndoBuffers();
	free(m_pUndoBuffer);
	FreeTempBuffers();
	}


//@doc SECEdit
//@mfunc Internal function. Sets default values for editor settings from the 
//              Registry.  See also SetRegistryKey().
//@rdesc int 
int SECEdit::InitFileTypeSettings()
   {
	CString entry;

   InitChroma();   // re-read the chroma stuff

	entry.LoadString(IDS_OE_TABSIZE);
	m_iTabSize = AfxGetApp()->GetProfileInt(m_strSection,entry,3);

	entry.LoadString(IDS_OE_KEEPTABS);
	m_bKeepTabs = AfxGetApp()->GetProfileInt(m_strSection,entry,1);

	entry.LoadString(IDS_OE_SHOWTABS);
	m_bShowTabs = AfxGetApp()->GetProfileInt(m_strSection,entry,0);

	entry.LoadString(IDS_OE_FONTNAME);
	m_fontname = AfxGetApp()->GetProfileString(m_strSection,entry,_T("Courier New"));

	entry.LoadString(IDS_OE_FONTSIZE);
	m_fontsize = AfxGetApp()->GetProfileInt(m_strSection,entry,9);

	entry.LoadString(IDS_OE_FONTCHARSET);
	m_fontcharset = AfxGetApp()->GetProfileInt(m_strSection,entry,0);

	entry.LoadString(IDS_OE_FONTWEIGHT);
	m_fontweight = AfxGetApp()->GetProfileInt(m_strSection,entry,400);

	entry.LoadString(IDS_OE_FONTITALIC);
	m_fontitalic = AfxGetApp()->GetProfileInt(m_strSection,entry,0);

	entry.LoadString(IDS_OE_WORDWRAP);
	m_bWordWrap = AfxGetApp()->GetProfileInt(m_strSection,entry,0);

	entry.LoadString(IDS_OE_INSERT);
	m_bInsertMode = AfxGetApp()->GetProfileInt(m_strSection,entry,1);

	entry.LoadString(IDS_OE_LOCKFILES);
	m_bFileLocking = AfxGetApp()->GetProfileInt(m_strSection,entry,0);

	return 0;

	}

//@doc SECEdit
//@mfunc Gets the SECEdit modified status
//@rdesc BOOL bOn TRUE - modified, FALSE - not modified.
BOOL SECEdit::GetModified(void)
{
	return m_bChanged;
}

//@doc SECEdit
//@mfunc Sets the SECEdit modified status
//@parm BOOL bOn TRUE - modified, FALSE - not modified.
void SECEdit::SetModified(BOOL bOn)
{
	m_bChanged = bOn;
}

//@doc SECEdit
//@mfunc Internal function. Determines if pFirst is before pSecond in the
//              linked list.
//@rdesc BOOL 
//@parm PTEXTPOS pFirst
//@parm  PTEXTPOS pSecond
BOOL SECEdit::InOrder(PTEXTPOS pFirst, PTEXTPOS pSecond)
   {
   PLINEDESC pLine;
   if (pFirst->pLine == pSecond->pLine)
      {
      if (pFirst->iOffset <= pSecond->iOffset)
         return TRUE;
      }
   else
      {
      pLine = pFirst->pLine->pNext;
      while (pLine)
         {
         if (pLine == pSecond->pLine)
            return TRUE;
         pLine = pLine->pNext;
         }
      }
   return FALSE;
   }

//@doc SECEdit
//@mfunc Internal function that Replaces tab characters in lpText with the 
//              appropriate number of spaces.
//@rdesc LPTSTR  pointer to the expanded text
//@parm LPTSTR lpText
//@parm int iTextSize
//@parm  int iTabSize
//@parm  int iLineEndLength
//@parm  int *iReturnSize
LPSTR SECEdit::ExpandTabs(LPSTR lpText,int iTextSize, int iTabSize, int iLineEndLength, int *iReturnSize)
  {
	iTabSize; //UNUSED
   LPSTR lpWorkBuffer;
   LPSTR lpNext;
   LPSTR lpStart;
   LPSTR lpTemp;
   int iNewTextSize;
   int iColNo,iInsert;
   int iLength,iCount;
	ASSERT(lpText);

	iLength = iTextSize;

   /* quick check for tabs, if any */
   lpNext = (LPSTR)memchr(lpText,9,iTextSize);

   if (!lpNext) /* no tabs, no work */
      {
      lpStart = (LPSTR)SECEditAlloc(iTextSize+1);
      memcpy(lpStart,lpText,iTextSize);
      *iReturnSize = iTextSize;
      return lpStart;
      }

   /* jump through, counting tabs, to get the needed workbuffer size */
   iCount = 0;
   iLength -= (lpNext - lpText);
   lpTemp = lpNext;
   while (lpNext && *lpNext)
      {
      lpNext = (LPSTR)memchr(lpTemp,9,iLength);
      if (lpNext)
         {
         *lpNext++;
         iCount++;
         iLength -= (lpNext - lpTemp);
         lpTemp = lpNext;
         }
      }

   lpNext = lpText;
     
   iCount += 3;  // a little fudge
   lpStart = lpWorkBuffer = (LPSTR)SECEditAlloc(iTextSize+(iCount*m_iTabSize));

   iNewTextSize = iColNo = 0;

   while (iTextSize)
      {
      switch (*lpNext)
         {
         case 0x09:
            lpNext++;
            iTextSize--;
            iInsert = m_iTabSize - (iColNo % m_iTabSize);
            if (!iInsert)
               iInsert = m_iTabSize;

            for (;iInsert;iInsert--)
               {
               iColNo++;
               iNewTextSize++;
               *lpWorkBuffer++ = ' ';
               }
            break;

         case 0x0D:
         case 0x0A:
            /* skip OE_CR/LFs and reset our ColNo indicator */
            {
            int i;
            for (i=0;i<iLineEndLength;i++)
               *lpWorkBuffer++ = *lpNext++;
            iColNo = 0;
            iTextSize -= iLineEndLength;
            iNewTextSize += iLineEndLength;
            break;
            }

         default:
            *lpWorkBuffer++ = *lpNext++;
            iNewTextSize++;
            iColNo++;
            iTextSize--;

         }
      }

   *iReturnSize = iNewTextSize;
   return lpStart;
   }
   
//@doc SECEdit
//@mfunc Internal function. Returns the actual text offset of a tabbed position
//@rdesc int The actual offset of the tabbed position
//@parm PLINEDESC pLine
//@parm  int iPos
int SECEdit::GetRealOffset(PLINEDESC pLine, int iPos)
   {
	LPSTR lpText;
	int i;       // the real offset count
	int iColNo;  // the apparent offset count
   int iAdd;
	ASSERT(pLine);

	i = iColNo = 0;
	lpText = pLine->pText;
   if (!lpText)
      return iPos;
	while (iColNo <= iPos)
      {
      if (*lpText)
         {
         if (*lpText == 0x09)
            {
            iAdd = m_iTabSize - (iColNo % m_iTabSize);
            if (!iAdd)
               iAdd = m_iTabSize;
            }
         else
            iAdd = 1;
         *lpText++;
         }
      else
         iAdd = 1;
      iColNo += iAdd;
      if (iColNo <= iPos)
         i++;
      }
   return i;
	}

//@doc SECEdit
//@mfunc Internal function. Returns the expanded text offset of an actual position
//@rdesc int The expanded text offset (e.g. the column position)
//@parm PLINEDESC pLine
//@parm  int iPos
int SECEdit::GetTabbedOffset(PLINEDESC pLine, int iPos)
   {
	LPSTR lpText;
	int i;       // the real offset count
	int iColNo;  // the apparent offset count
   int iAdd;
	ASSERT(pLine);

	i = iColNo = 0;
	lpText = pLine->pText;
   if (!lpText)
      return iPos;
	while (i < iPos)
      {
      if (*lpText)
         {
         if (*lpText == 0x09)
            {
            iAdd = m_iTabSize - (iColNo % m_iTabSize);
            if (!iAdd)
               iAdd = m_iTabSize;
            }
         else
            iAdd = 1;
         lpText++;
         }
      else
         iAdd = 1;
      iColNo += iAdd;
      i++;
      }
   return iColNo;
	}


//@doc SECEdit
//@mfunc Sets the SECEdit readonly status
//@rdesc void 
//@parm BOOL bOn TRUE - read only, FALSE - not read only.

void SECEdit::SetReadOnly(BOOL bOn)
   {
   m_bReadOnly = bOn;
   }


//@doc SECEdit
//@mfunc Gets the SECEdit readonly status
//@rdesc BOOL TRUE - read only, FALSE - not read only.
//@parm void
BOOL SECEdit::GetReadOnly(void)
   {
	return m_bReadOnly;
	}

//@doc SECEdit
//@mfunc Sets the SECEdit file locking status
//@rdesc void 
//@parm BOOL bOn TRUE - File locking is on, FALSE - File locking is off.

void SECEdit::SetFileLocking(BOOL bOn)
   {
	m_bFileLocking = bOn;
   }


//@doc SECEdit
//@mfunc Internal member function. - Clears the m_hFile member
//@rdesc void 
//@parm void
void SECEdit::ClearFileHandle(void)
   {
	m_hFile = NULL;
	}

//@doc SECEdit
//@mfunc Gets the SECEdit file locking status.
//@rdesc BOOL  TRUE - File locking is turned on. FALSE - file locking is turned off.
//@parm void
BOOL SECEdit::GetFileLocking(void)
   {
	return m_bFileLocking;
	}

//@doc SECEdit
//@mfunc Gets the current SECEdit tab size setting
//@rdesc int The tab size.
//@parm void
int SECEdit::GetTabSize(void)
   {
	return m_iTabSize;
	}

//@doc SECEdit
//@mfunc Sets the SECEdit tab size setting
//@rdesc void 
//@parm int iSize The new tab size.
void SECEdit::SetTabSize(int iSize)
   {
	m_iTabSize = iSize;
	}

// these are stubbed out, but are not implemented
void SECEdit::SetWordWrap(BOOL bOn)
{
	m_bWordWrap = bOn;
}

BOOL SECEdit::GetWordWrap(void)
{
	return m_bWordWrap;
}

void SECEdit::SetInsertMode(BOOL bOn)
{
	m_bInsertMode = bOn;
}

BOOL SECEdit::GetInsertMode(void)
{
	return m_bInsertMode;
}
// End stubs

//@doc SECEdit
//@mfunc Gets the SECEdit line end length.  Line ends can
//              be set to OE_CR, OE_LF, or OE_CR/OE_LF
//@rdesc int 
//@parm void
int SECEdit::GetLineEndLength(void)
   {
	return m_iLineEndLength;
	}
