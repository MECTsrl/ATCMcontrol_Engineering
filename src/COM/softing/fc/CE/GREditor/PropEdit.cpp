

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"
#include "GrEditor.h"
#include "PropEdit.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/
/*locvar*====================================================================*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*proto*=====================================================================*/
/*func*======================================================================*/

/////////////////////////////////////////////////////////////////////////////
// CPropEdit

CPropEdit::CPropEdit()
{
}

CPropEdit::~CPropEdit()
{
}


BEGIN_MESSAGE_MAP(CPropEdit, CEdit)
	//{{AFX_MSG_MAP(CPropEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropEdit message handlers

void CPropEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
   CWnd *wnd;

	switch( nChar )
   {
      case VK_ESCAPE:
         wnd = GetParent();
         if( wnd ) wnd->PostMessage( WM_CHAR, nChar );
         break;

      case VK_RETURN:
      case VK_TAB:
         wnd = GetParent();
         if( wnd ) wnd->PostMessage( WM_CHAR, nChar );
         break;

      default:
         CEdit::OnChar(nChar, nRepCnt, nFlags); break;
   }
}

void CPropEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   CWnd *wnd;

   switch( nChar )
   {
      case VK_RETURN:
      case VK_ESCAPE:
      case VK_TAB:
         break;

      case VK_F12:
         wnd = GetParent();
         if( wnd ) wnd->PostMessage( WM_USER, (WPARAM)VK_RETURN, (LPARAM)this );

      default:
         CEdit::OnKeyDown(nChar, nRepCnt, nFlags); break;
   }
}

void CPropEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch( nChar )
   {
      case VK_RETURN:
      case VK_ESCAPE:
      case VK_TAB:
         break;

      default:
	      CEdit::OnKeyUp(nChar, nRepCnt, nFlags); break;
   }
}