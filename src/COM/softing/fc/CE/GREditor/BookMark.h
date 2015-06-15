#ifndef Bookmark_H
#define Bookmark_H
#include "StdEdAttr.h"


struct BookMark
{
public:
	BookMark();
	BookMark(CPoint Pos, const CString& Name);
	BookMark(const BookMark &bookmark);
	const BookMark &operator= (const BookMark &source);
	bool operator== (const BookMark &other) const;
   void  Draw(CDC * pDC, CGrEditorView * pView);
   CSize GetSize(CDC *pDC, class CGrEditorView * pView, CSize *pSize=NULL);
   CPoint m_Pos;
   CString m_Name;
};

inline BookMark::BookMark()
{
   m_Pos = CPoint(0,0);
}

inline BookMark::BookMark(CPoint Pos, const CString& Name)
{
   m_Pos = Pos;
   m_Name = Name;
}

inline BookMark::BookMark(const BookMark &b)
{
   m_Pos    = b.m_Pos;
   m_Name   = b.m_Name;
}

inline const BookMark& BookMark::operator= (const BookMark &b)
{
   if(&b != this)
   {
      m_Pos    = b.m_Pos;
      m_Name   = b.m_Name;
   }
   return *this;
}

inline bool BookMark::operator== (const BookMark &b) const
{
   
   return m_Pos == b.m_Pos ?true :false;
}

#endif
