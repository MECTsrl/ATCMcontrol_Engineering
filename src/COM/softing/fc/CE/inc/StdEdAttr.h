#ifndef StdEdAttr_H
#define StdEdAttr_H

#include "StdEdEdipos.h"

class CStdEditorPosition : public CObject
{
public:
    CStdEditorPosition (); //DBI_STLINE and all number -1 (use only for array ctor)

    //if edp is from string:
    CStdEditorPosition (const TCHAR* pszEdp); 
    
    //if posQual is DBI_GRBEGIN,DBI_GREND:
    CStdEditorPosition (EDP_POS_QUALIFIER posQual); 

    //if posQual is DBI_STLINE,DBI_GRBOX:
    CStdEditorPosition (EDP_POS_QUALIFIER posQual, long lX, long lY);

    //if posQual is ANY
    CStdEditorPosition (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);

    CStdEditorPosition (const CStdEditorPosition &source);
    const CStdEditorPosition &operator= (const CStdEditorPosition &source);

    bool operator== (const CStdEditorPosition &other) const;
    bool operator!= (const CStdEditorPosition &other) const;

    bool isValid()const;

    EDP_POS_QUALIFIER GetPosQual()const{return m_posQual;}
    long GetX () const{return m_lX;}
    long GetY () const{return m_lY;}
    long GetCX() const{return m_lCX;}
    long GetCY() const{return m_lCY;}

    CString toEdpString()const;

    void GetXYPosition(int& X, int& Y)const
    {
        X = m_lX;
        Y = m_lY;
    }

protected:
    EDP_POS_QUALIFIER   m_posQual;
    long                m_lX;
    long                m_lY;
    long                m_lCX;
    long                m_lCY;
};




class CStdEditorColor : public CObject
{
public:
    enum LogicalColor
    {
        BackgroundColor,
        ForegroundColor,
        CommentColor,
        ErrorColor
    };

    CStdEditorColor (LogicalColor kind, COLORREF color);
    CStdEditorColor (const CStdEditorColor &source);
    const CStdEditorColor &operator= (const CStdEditorColor &source);

    LogicalColor GetKind () const;
    COLORREF GetColor () const;

protected:
    LogicalColor m_Kind;
    COLORREF     m_Color;
};

#endif