/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "Toolbox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CHourglass
///////////////////////////////////////////////////////////////////////////////

//----------------------------- FUNCTION -------------------------------------*
CHourglass::CHourglass(HWND hwndCap /* = NULL*/)
/*>>>> 
Hourglass constructor:
Change cursor to hourglass, and capture the mouse

I   hwndCap     window wanting to capture focus

Result
  void
<<<<*/
{
   m_hSaveCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
   m_hwndCapture = hwndCap;
   if (m_hwndCapture)
      ::SetCapture(m_hwndCapture);
}

//----------------------------- FUNCTION -------------------------------------*
CHourglass::~CHourglass()
/*>>>> 
Hourglass destructur: restore original cursor.

Result
  void
<<<<*/
{
   ::SetCursor(m_hSaveCursor);
   if (m_hwndCapture)
      ::ReleaseCapture();
}

/* ---------------------------------------------------------------------------- */
