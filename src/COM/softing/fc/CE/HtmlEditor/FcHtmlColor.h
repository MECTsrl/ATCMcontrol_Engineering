#if !defined(AFX_FCHTMLCOLOR_H__71B91826_CEF9_11D2_B4B3_001767C30000__INCLUDED_)
#define AFX_FCHTMLCOLOR_H__71B91826_CEF9_11D2_B4B3_001767C30000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcHtmlColor.h : header file
//

inline unsigned FcGetBaseColorMax()  { return 16; }
inline unsigned FcGetNamedColorMax() { return ID_COLOR_MAX - ID_COLOR_MIN + 1; }

int FcGetHtmlColorIndex(const CString& HtmlColorValue);
CString FcGetHtmlColor(unsigned ColorIndex);
CString FcGetHtmlColorName(unsigned ColorIndex);
CString FcGetHtmlColorNameAndValue(unsigned ColorIndex);

CString FcColorRef2HtmlColor(COLORREF color);
COLORREF FcHtmlColor2ColorRef(const CString& HtmlColor);
CString FcGetHtmlColorValueFromString(const CString& str);
CString FcGetHtmlColorNameFromString(const CString& str);
BOOL FcCheckHtmlColor(const CString& HtmlColor);

#endif // !defined(AFX_FCHTMLCOLOR_H__71B91826_CEF9_11D2_B4B3_001767C30000__INCLUDED_)
