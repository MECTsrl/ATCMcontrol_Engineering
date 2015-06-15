

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned HexChar2Number(char c) 
{
	if (c >= '0' && c <= '9')
		return (unsigned)(c - '0');
	else if (c >= 'a' && c <= 'f')
		return (unsigned)(c - 'a' + 0xa);
	else if (c >= 'A' && c <= 'F')
		return (unsigned)(c - 'A' + 0xa);
	
	ASSERT(FALSE);
	return 0;
}

CString FcGetHtmlColorValue(unsigned ColorIndex)
{
	ASSERT(ColorIndex < FcGetNamedColorMax());
	if (ColorIndex < FcGetNamedColorMax())
	{
		CString str;
		str.LoadString(ID_COLOR_MIN + ColorIndex);
		return str.Left(7);
	}
	return CString("");
}


CString FcGetHtmlColorName(unsigned ColorIndex)
{
	ASSERT(ColorIndex < FcGetNamedColorMax());
	if (ColorIndex < FcGetNamedColorMax())
	{
		CString str;
		str.LoadString(ID_COLOR_MIN + ColorIndex);
		return str.Right(str.GetLength() - 8);
	}
	return CString("");
}

int FcGetHtmlColorIndex(const CString& HtmlColorValue)
{
	for (int ii = 0; ii < (int)FcGetNamedColorMax(); ii++)
	{
		if (HtmlColorValue.CompareNoCase(FcGetHtmlColorValue(ii)) == 0)
			return ii;
	}
	return -1;
}


CString FcGetHtmlColorNameAndValue(unsigned ColorIndex)
{
	ASSERT(ColorIndex < FcGetNamedColorMax());
	if (ColorIndex < FcGetNamedColorMax())
	{
		CString str;
		str.LoadString(ID_COLOR_MIN + ColorIndex);
		return str.Right(str.GetLength() - 8) + " (" + str.Left(7) + ")";
	}
	return CString("");
}

CString FcColorRef2HtmlColor(COLORREF color) 
{
	int idx;
	WCHAR buff[8]=L"#000000";
	WCHAR HEX[17]=L"0123456789ABCDEF";

	for (int i=0; i<3; i++)
	{
		idx = color & 0xF;
		buff[2*i+2] = HEX[idx];
		color >>= 4;
		idx = color & 0xF;
		buff[2*i+1] = HEX[idx];
		color >>= 4;
	}
	return CString(buff);
}

COLORREF FcHtmlColor2ColorRef(const CString& HtmlColor) 
{
	ASSERT(HtmlColor.GetLength() == 7);
	unsigned r = 16 * HexChar2Number(HtmlColor.GetAt(1)) + HexChar2Number(HtmlColor.GetAt(2));
	unsigned g = 16 * HexChar2Number(HtmlColor.GetAt(3)) + HexChar2Number(HtmlColor.GetAt(4));
	unsigned b = 16 * HexChar2Number(HtmlColor.GetAt(5)) + HexChar2Number(HtmlColor.GetAt(6));

	return RGB(r, g, b);	
}

CString FcGetHtmlColorValueFromString(const CString& str)
{
	int ii = str.FindOneOf("#");
	if (ii >= 0 && (str.GetLength() - ii) >= 7)
		return str.Mid(ii, 7);

	return CString("");
}

CString FcGetHtmlColorNameFromString(const CString& str)
{
	CString ColNameLower, strLower;
	strLower = str;
	strLower.MakeLower();

	for (unsigned ii = 0; ii < FcGetNamedColorMax(); ii++)
	{
		ColNameLower = FcGetHtmlColorName(ii);
		ColNameLower.MakeLower();
		if (strLower.Find(ColNameLower) >= 0)
			return FcGetHtmlColorName(ii);
	}
	return CString("");
}

BOOL FcCheckHtmlColor(const CString& HtmlColor)
{
	if (HtmlColor.GetLength() == 7 && HtmlColor.GetAt(0) == '#')
	{
		for (int ii = 1; ii < 7; ii++)
		{
			if (HtmlColor.GetAt(ii) != '0' && HexChar2Number(HtmlColor.GetAt(ii)) == 0)
				return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}
