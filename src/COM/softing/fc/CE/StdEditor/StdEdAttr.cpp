


#include "stdafx.h"
#include "StdEdAttr.h"


CStdEditorPosition::CStdEditorPosition ()
{
	m_posQual = EDP_STLINE;
    m_lX  = 0;
	m_lY  = 0;
    m_lCX =-1;
    m_lCY =-1;
}

CStdEditorPosition::CStdEditorPosition (EDP_POS_QUALIFIER posQual)
{
	ASSERT(posQual==EDP_GRBEGIN || posQual==EDP_GREND);
    m_posQual = posQual;
 	m_lX      = -1;
	m_lY      = -1;
    m_lCX     = -1;
    m_lCY     = -1;
}

CStdEditorPosition::CStdEditorPosition (EDP_POS_QUALIFIER posQual, long lX, long lY)
{
	ASSERT(posQual==EDP_STLINE || posQual==EDP_GRBOX);
    m_posQual = posQual;
 	m_lX      = lX;
	m_lY      = lY;
    m_lCX     = -1;
    m_lCY     = -1;
}
CStdEditorPosition::CStdEditorPosition (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
	m_posQual = posQual;
 	m_lX      = lX;
	m_lY      = lY;
    m_lCX     = lCX;
    m_lCY     = lCY;
}




CStdEditorPosition::CStdEditorPosition (const CStdEditorPosition &source)
{
	m_posQual = source.m_posQual;
 	m_lX      = source.m_lX;
	m_lY      = source.m_lY;
 	m_lCX     = source.m_lCX;
	m_lCY     = source.m_lCY;
}


const CStdEditorPosition &CStdEditorPosition::operator= (const CStdEditorPosition &source)
{
	m_posQual = source.m_posQual;
 	m_lX      = source.m_lX;
	m_lY      = source.m_lY;
 	m_lCX     = source.m_lCX;
	m_lCY     = source.m_lCY;

	return (*this);
}


bool CStdEditorPosition::operator== (const CStdEditorPosition &other) const
{
	return (m_posQual == other.m_posQual && 
            m_lX == other.m_lX   && m_lY== other.m_lY &&
            m_lCX == other.m_lCX && m_lCY== other.m_lCY
           );
}


bool CStdEditorPosition::operator!= (const CStdEditorPosition &other) const
{
	return (!(*this == other));
}





//////////////////////////////////////////////////////////////
// CStdEditorColor methods

CStdEditorColor::CStdEditorColor (LogicalColor kind, COLORREF color)
{
	m_Kind = kind;
	m_Color = color;
}


CStdEditorColor::CStdEditorColor (const CStdEditorColor &source)
{
	m_Kind = source.m_Kind;
	m_Color = source.m_Color;
}


const CStdEditorColor &CStdEditorColor::operator= (const CStdEditorColor &source)
{
	m_Kind = source.m_Kind;
	m_Color = source.m_Color;

	return (*this);
}


CStdEditorColor::LogicalColor CStdEditorColor::GetKind () const
{
	return (m_Kind);
}


COLORREF CStdEditorColor::GetColor () const
{
	return (m_Color);
}
