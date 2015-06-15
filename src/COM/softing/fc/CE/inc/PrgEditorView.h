// PrgEditorView.h: interface for the CPrgEditorView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRGEDITORVIEW_H__4B43F3D3_2348_11D5_B1FE_00E029440C3D__INCLUDED_)
#define AFX_PRGEDITORVIEW_H__4B43F3D3_2348_11D5_B1FE_00E029440C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxcmn.h"
#include "baselib.h"
#include "fccallback.h"
#include "StdEdAttr.h"
#include "CESysDef.h"
#include "Watchpoint.h"
#include "PrgEditorDoc.h"
#include "dragitem.h"

//////////////////////////////////////////////////////
// DATA TIP declaration (common for all editors)
//
enum CCEDataTipAction
{
    CCEShowDataTip,
    CCEHideDataTip,
    CCEFollowDataTip,
    CCERefreshDataTip
};

FC_CALLBACK_DEF1 (DataTipPopupCB, bool &);
FC_CALLBACK_DEF1 (DataTipTextRequestCB, CString &);
FC_CALLBACK_DEF2 (DataTipActionQueryCB, CPoint, CCEDataTipAction &)


class CCEDataTip : public CObject
{
public:
    CCEDataTip ();
    ~CCEDataTip ();

    void SetPopupCB       (FC_CALLBACK_BASE (DataTipPopupCB) *pPopupCB);
    void SetTextRequestCB (FC_CALLBACK_BASE (DataTipTextRequestCB) *pTextRequestCB);
    void SetActionQueryCB (FC_CALLBACK_BASE (DataTipActionQueryCB) *pActionQueryCB);

    HRESULT Create (CWnd *pParent, DWORD dwStyle, CWnd *pTool);
    HRESULT Destroy ();
    bool IsCreated ();

    HRESULT Activate ();
    HRESULT Deactivate ();

    HRESULT Show ();
    HRESULT Hide ();
    HRESULT Follow (bool bRefresh);

    HRESULT SetText (const CString &strText);

    void RelayMessage (LPMSG pMessage);

    bool OnPopup      (UINT id, NMHDR *pTTTStruct, LRESULT *pResult);

protected:
    
    FC_CALLBACK_BASE (DataTipPopupCB)       *m_pPopupCB;
    FC_CALLBACK_BASE (DataTipTextRequestCB) *m_pTextRequestCB;
    FC_CALLBACK_BASE (DataTipActionQueryCB) *m_pActionQueryCB;

    CToolTipCtrl m_ToolTip;

    bool        m_bCreated;
    bool        m_bActive;
    bool        m_bStarted;
    bool        m_bDisplayed;
    CWnd       *m_pTool;
    CString     m_strText;
};





/*-----------------------------------------------------------------------------
  CPrgEditorView

  - common debug and monitoring handling
-----------------------------------------------------------------------------*/
template<class T>
class CPrgEditorView : public T
{
public:
    typedef T TParent;  // for use in derived classes

    typedef enum DrawTarget { ActiveView, InactiveView };

    CPrgEditorView();

    // common datatip handling ...
    CCEDataTip                                          m_DataTip;
    FC_CALLBACK (DataTipPopupCB, CPrgEditorView)        m_DataTipPopupCB;
    FC_CALLBACK (DataTipTextRequestCB, CPrgEditorView)  m_DataTipTextRequestCB;
    FC_CALLBACK (DataTipActionQueryCB, CPrgEditorView)  m_DataTipActionQueryCB;
    LONG                                                m_lDataTipHandle;
    BL_IEC_TYP                                          m_DataTipType;
    CString                                             m_strDataTipName;
    CStdEditorPosition                                  m_DataTipPosition;
    DrawTarget  m_DrawTarget;

    bool AddDataTip ();
    void RemoveDataTip ();
    void UnsubscribeDataTip ();
    
    void OnDataTipPopup (bool &bAllow);
    void OnDataTipTextRequest (CString &strText);
    void OnDataTipActionQuery (CPoint point, CCEDataTipAction &action);

    BOOL OnToolTipNfy(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    virtual void OnUpdateToolTipNotify(TTNfyData* pTTDat); // update handler for CPrgEditorDoc::ToolTipNfyHint

    void RecreateToolTip();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSTEditorView)
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

protected:
    bool        m_bEnteredSuspending;
    CFont       m_WatchpointFont;
    bool        m_bHaveWPFont;
    bool        m_bShowAbbrevNames;     // show data tip if text is not fully visible?
    bool        m_bShowFullText;        // showing full text right now?

protected:
   LPCTSTR GetDataTypeName(UINT uiIECType);
   CString BuildWatchText (const CString &strVariableName, LONG lHandle, BL_IEC_TYP iecType, bool bUsingCrossOut, COLORREF &backgroundColor, bool &bCrossedOut);
   void    BuildWatchpointText (CWatchpoint *pWP);
   void    UpdateWatchpoint (CWatchpoint *pWP);
//   void    UpdateWatchpointArea (CWatchpoint *pWP, CDC *pDC);
   void    OnHideWatchpoint (CWatchpoint *pWP);
   void    UpdateWatchpointExtent (CWatchpoint *pWP, CDC *pDC);
//   bool    UpdateWatchpointVisibility (CPrgWPClientData *pClientData, CRect drawArea);

   virtual CSECEditor* GetEditor(UINT id);
   virtual bool CheckInPlaceActive();
   virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

   //must be overridden
   virtual void WPAreaToClientArea (CRect& rect)=0;//{ assert("override this method!"&&0); }
   virtual CString GetContextExpression (CPoint point, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition)=0;
};


/////////////////////////////////////////////////////////////////////////////// IMPLEMENTATION

template<class T>
CPrgEditorView<T>::CPrgEditorView()
: m_DataTipPopupCB       (this, OnDataTipPopup),
  m_DataTipActionQueryCB (this, OnDataTipActionQuery),
  m_DataTipTextRequestCB (this, OnDataTipTextRequest)
{
   m_lDataTipHandle = 0;

   m_DataTip.SetPopupCB (&m_DataTipPopupCB);
   m_DataTip.SetTextRequestCB (&m_DataTipTextRequestCB);
   m_DataTip.SetActionQueryCB (&m_DataTipActionQueryCB);
   m_bEnteredSuspending = false;
   m_bHaveWPFont        = false;
   m_bShowAbbrevNames   = false;
   m_bShowFullText      = false;
}

template<class T>
CSECEditor* CPrgEditorView<T>::GetEditor(UINT id)
{
    CPrgEditorDoc* pDoc;
    pDoc = dynamic_cast<CPrgEditorDoc*>(GetDocument());
    ASSERT(pDoc!=NULL);
    return pDoc? pDoc->GetEditor(id) :NULL;
}

template<class T>
bool CPrgEditorView<T>::CheckInPlaceActive()
{
    CPrgEditorDoc* pDoc;
    pDoc = dynamic_cast<CPrgEditorDoc*>(GetDocument());
    ASSERT(pDoc!=NULL);
    return pDoc? pDoc->CheckInPlaceActive() :false;
}

template<class T>
void CPrgEditorView<T>::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
    T::OnActivateView(bActivate, pActivateView, pDeactiveView);   // call the base class

    CPrgEditorDoc* pDoc = dynamic_cast<CPrgEditorDoc*>(GetDocument());

    if (bActivate)
    {
        if (pActivateView != this)
        {
            return;
        }
        SetFocus();
        if (pDoc != NULL)
        {
            pDoc->InvalidateReadonlyCache ();
            pDoc->SetLastActiveView(this);
        }
    }
}

//------------------------------------------------------------------*
/**
 * recreate tool tip.
 *
 *  workaround for problems with tool tip update.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
template<class T>
void CPrgEditorView<T>::RecreateToolTip()
{
    if(m_DataTip.IsCreated())
    {
        m_DataTip.Destroy();

        m_DataTip.Create (this, TTS_ALWAYSTIP | TTS_NOPREFIX, this);

        if (m_DataTip.IsCreated ())
        {
            m_DataTip.Activate ();
        }
    }
}


template<class T>
void CPrgEditorView<T>::OnDataTipPopup (bool &bAllow)
{
    if (m_lDataTipHandle != 0)
    {
        bAllow = true;
        return;
    }

    bAllow = AddDataTip ();
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
void CPrgEditorView<T>::OnDataTipTextRequest (CString &strText)
{
    COLORREF backgroundColor;
    bool bCrossedOut;

    CPrgEditorDoc* pDoc = (CPrgEditorDoc*) GetDocument();

    if( pDoc )
    {
        if( pDoc->CanDebug() && pDoc->IsConnected() )
        {
            strText = BuildWatchText (m_strDataTipName, m_lDataTipHandle, m_DataTipType, false, backgroundColor, bCrossedOut);
        } else
        {
            strText = m_strDataTipName;
        }
    }
}



/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
void CPrgEditorView<T>::OnDataTipActionQuery (CPoint point, CCEDataTipAction &action)
{
   CString strExpression;
   CStdEditorPosition posStart;
   CStdEditorPosition posEnd;
   CStdEditorPosition context;

   if (GetDocument () == NULL)
   {
      UnsubscribeDataTip ();
      action = CCEHideDataTip;
      return;
   }
   
   strExpression = GetContextExpression (point, posStart, posEnd);
   if (strExpression.IsEmpty ())
   {
      UnsubscribeDataTip ();
      action = CCEHideDataTip;
      return;
   }
   
   if((m_lDataTipHandle || m_bShowFullText) && (m_DataTipPosition == posStart))
   {
      action = CCEFollowDataTip;
      return;
   }
   
   UnsubscribeDataTip ();
   
   m_strDataTipName = strExpression;
   m_DataTipPosition = posStart;
   
   action = CCEShowDataTip;
}



/*func*------------------------------------------------------------------------
  description: route the notification request from active View to the doc
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
BOOL CPrgEditorView<T>::OnToolTipNfy (UINT id, NMHDR *pTTTStruct, LRESULT *pResult)
{
   TTNfyData TTDat;
   TTDat.id          = id;
   TTDat.pTTTStruct  = pTTTStruct;
   TTDat.pResult     = pResult;
   TTDat.bRes        = FALSE;
   GetCursorPos(&TTDat.Pmouse);
   
   CPrgEditorDoc* pDoc = (CPrgEditorDoc*)GetDocument();
   CRect  Rview(0,0,0,0);
   GetWindowRect(&Rview);

   if(!Rview.PtInRect(TTDat.Pmouse) || pTTTStruct->idFrom != (UINT) m_hWnd)
   {
      if(pDoc)
      {
         pDoc->UpdateAllViews(this, CPrgEditorDoc::ToolTipNfyHint, &TTDat);
         return TTDat.bRes;
      }
   }
   return m_DataTip.OnPopup (id, pTTTStruct, pResult);
}



/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
void CPrgEditorView<T>::OnUpdateToolTipNotify(TTNfyData* pTTDat)
{
   CRect  Rview(0,0,0,0);
   GetWindowRect(&Rview);

   if( Rview.PtInRect(pTTDat->Pmouse) )
   {
      pTTDat->bRes = m_DataTip.OnPopup (pTTDat->id, pTTDat->pTTTStruct, pTTDat->pResult);
   }
}



/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
bool CPrgEditorView<T>::AddDataTip ()
{
   CPrgEditorDoc*       pDoc;
   HRESULT              hr;
   CString              strText;
   CEExpressionState    state;
   LONG                 lHandle = 0;
   BL_IEC_TYP           iecType;
   
   if (m_bEnteredSuspending)
   {
      m_strDataTipName.Empty();
      return false;
   }
   
   pDoc = (CPrgEditorDoc*)GetDocument();
   if (pDoc == NULL)
   {
      return false;
   }
   
   m_lDataTipHandle = 0;
   m_DataTipType = BL_ANY;
//   m_strDataTipName.Empty ();
   bool bDebug = pDoc->CanMonitor() && pDoc->IsConnected();

   if(bDebug)
   {
      // Add watch item. If adding fails do not continue.
      hr = pDoc->AddWatchItem (m_strDataTipName, lHandle);  // writes lHandle
      if (FAILED (hr))
      {
         m_strDataTipName.Empty ();
         return false;
      }
     
      // If watch item type is known and not a simple type do not continue.
      iecType = BL_ANY;
      if (!pDoc->IsUnknownWatchItemType (lHandle) && !pDoc->IsSimpleWatchItemType (lHandle, iecType))
      {
         m_strDataTipName.Empty ();
         pDoc->RemoveWatchItem (lHandle);
         return false;
      }
     
      // Connect watch item. If connecting fails do not continue.
      hr = pDoc->ConnectWatchItem (lHandle);
      if (FAILED (hr))
      {
         m_strDataTipName.Empty ();
         (void)pDoc->RemoveWatchItem (lHandle);
         return false;
      }
      
      m_lDataTipHandle = lHandle;
      m_DataTipType = iecType;

      // Get state of watch item. If
      // 1) the item is unknown at the control or
      // 2) the item is unknown in debug info
      // 3) the watch item will not deliver a non-empty value from the
      //    watch backends point of view (which is in case of a syntax
      //    error)
      // do not continue
     
      state = pDoc->GetWatchExpressionState (lHandle);
      if (state == CEUnknownSubscription || state == CEUnknownField || state == CEEmptyValue)
      {
         UnsubscribeDataTip ();
         return false;
      }
      return true;
   }
   else if(m_bShowAbbrevNames)
   {
       m_bShowFullText = true;
       return true;
   }
   else
   {
      m_strDataTipName.Empty ();
   }
   return false;
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
void CPrgEditorView<T>::RemoveDataTip ()
{
   m_DataTip.Hide ();
   
   UnsubscribeDataTip ();
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
void CPrgEditorView<T>::UnsubscribeDataTip ()
{
   CPrgEditorDoc *pDoc;
   LONG lHandle;
   
   if (m_bEnteredSuspending)
   {
      return;
   }
   
   pDoc = (CPrgEditorDoc*) GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   lHandle = m_lDataTipHandle;
   m_lDataTipHandle = 0;
   m_bShowFullText = false;
   m_strDataTipName.Empty ();
   
   if (lHandle > 0)
   {
      (void)pDoc->DisconnectWatchItem (lHandle);
      (void)pDoc->RemoveWatchItem (lHandle);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
template<class T>
CString CPrgEditorView<T>::BuildWatchText 
    (
    const CString &strVariableName, 
    LONG lHandle, 
    BL_IEC_TYP iecType, 
    bool bUsingCrossOut, 
    COLORREF &backgroundColor, 
    bool &bCrossedOut    
    )
{
   CPrgEditorDoc *pDoc;
   CEExpressionState state;
   CString strTypeName;
   CString strValue;
	CString strText;
   short sQuality;
   DATE timestamp;
   
   ASSERT (lHandle != 0);
   if (lHandle == 0)
   {
      backgroundColor = RGB (255, 255, 255);
      bCrossedOut = true;
      return ("");
   }
   
   pDoc = (CPrgEditorDoc*) GetDocument ();
   state = pDoc->GetWatchExpressionState (lHandle);

	if (iecType == BL_ANY)
	{
		strTypeName = "auto";
	}
	else
	{
		strTypeName = GetDataTypeName (iecType);
	}

   strValue = pDoc->GetWatchExpressionValue (lHandle, sQuality, timestamp);

   backgroundColor = RGB (255, 255, 230);
   bCrossedOut = false;
   
   if (state == CELoadingValue)
   {
      strValue = "loading";
   }
   else if (state == CEGoodExpression || state == CEReloadingValue)
   {
      if ((sQuality & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
      {
         bCrossedOut = true;
      }
   }
    else if (state == CEContainsInOut)
    {
        strValue = "contains VAR_IN_OUT";
    }
   else
   {
      bCrossedOut = true;
   }
   
   if (!bCrossedOut || bUsingCrossOut)
   {
      strText.Format ("%s: %s = %s", (LPCTSTR)strVariableName, (LPCTSTR)strTypeName, (LPCTSTR)strValue);
   }
   else
   {
      strText.Format ("%s: %s bad", (LPCTSTR)strVariableName, (LPCTSTR)strTypeName);
   }

   return (strText);
}


template<class T>
LPCTSTR CPrgEditorView<T>::GetDataTypeName(UINT uiIECType)
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>GetDataTypeName</B>.
*
*	Description = returns a string constant containing the data type name
*	<p>
*	Specification = 
*	<p>
*	@param uiIECType - id of the data type (as defined in baselib.h)
*	<p>
*	@param
*	<p>
*
*	@return - String constant containing the data type name
*	<p>
*	@return
*	<p>
*
*
*	@exception
*	<p>
*	@see
*	<p>
*/
{
    switch (uiIECType)
    {
        case BL_ANY:
            return _T("BOOL");
            break;

        case BL_BOOL:
            return _T("BOOL");
            break;

        case BL_BYTE:
            return _T("BYTE");
            break;

        case BL_WORD:
            return _T("WORD");
            break;

        case BL_DWORD:
            return _T("DWORD");
            break;

        case BL_LWORD:
            return _T("LWORD");
            break;

        case BL_DINT:
            return _T("DINT");
            break;

        case BL_LINT:
            return _T("LINT");
            break;

        case BL_LREAL:
            return _T("LREAL");
            break;

        case BL_TIME:
            return _T("TIME");
            break;

        case BL_DT:
            return _T("DT");
            break;

        case BL_DATE:
            return _T("DATE");
            break;

        case BL_TOD:
            return _T("TOD");
            break;

        case BL_WSTRING:
            return _T("WSTRING");
            break;
		
		case BL_UINT:
            return _T("UINT");
            break;

		case BL_UDINT:
            return _T("UDINT");
            break;

		case BL_ULINT:
            return _T("ULINT");
            break;

        default:
            return _T("");
            break;
    }
}

template<class T>
void CPrgEditorView<T>::BuildWatchpointText (CWatchpoint *pWP)
{
    CPrgWPClientData *pClientData;
    CString strText;
    COLORREF backgroundColor;
    bool bCrossedOut;

	ASSERT (pWP != NULL);
	if (pWP == NULL)
	{
		return;
	}

	pClientData = (CPrgWPClientData *)pWP->GetClientData ();
	ASSERT (pClientData != NULL);
	if (pClientData == NULL)
	{
		return;
	}

    strText = BuildWatchText (pClientData->GetVariableName (), pWP->GetWatchItem (), pWP->GetIECType (), true, backgroundColor, bCrossedOut);
   
    pClientData->SetText (strText);
    pClientData->SetBackgroundColor (backgroundColor);
    pClientData->SetCrossedOut (bCrossedOut);
}


template<class T>
BOOL CPrgEditorView<T>::PreTranslateMessage (MSG* pMsg) 
{
    if (m_DataTip.IsCreated () && pMsg->hwnd == m_hWnd)
    {
        if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
        {
            m_DataTip.RelayMessage (pMsg);
        }
    }

    return (T::PreTranslateMessage (pMsg));
}

/*
template<class T>
bool CPrgEditorView<T>::UpdateWatchpointVisibility (CPrgWPClientData *pClientData, CRect drawArea)
{
    CRect rect;
    CRect clientRect;
    bool bVisible;
    int xOffset;
    int yOffset;

    rect = pClientData->GetLargestArea ();

    if (m_DrawTarget == ActiveView)
    {
        GetClientRect (&clientRect);
    }
    else
    {
        clientRect = drawArea;
    }

    xOffset = m_iLeftEdge * m_iCharWidth;
    yOffset = (m_iFirstLineNo - 1) * m_iLineHeight;

    bVisible = rect.bottom > yOffset && rect.top < yOffset + (clientRect.bottom - clientRect.top) &&
               rect.right > xOffset && rect.left < yOffset + (clientRect.right - clientRect.left);

    if (m_DrawTarget == ActiveView)
    {
        pClientData->SetActiveVisibility (bVisible);
    } else
    {
        pClientData->SetInactiveVisibility (bVisible);
    }

    return (bVisible);
}
*/

template<class T>
void CPrgEditorView<T>::UpdateWatchpoint (CWatchpoint *pWP)
{
	CPrgWPClientData *pClientData;
	CPrgEditorDoc *pDoc;

	pDoc = (CPrgEditorDoc*) GetDocument ();

	if (pDoc->IsInPlaceActive ())
	{
		CClientDC dc (this);
		CRect rect;
      OnPrepareDC(&dc);

		pClientData = (CPrgWPClientData *)pWP->GetClientData ();
		assert (pClientData != NULL);
		if (pClientData == NULL)
			return;

		UpdateWatchpointExtent (pWP, &dc);
		rect = pClientData->GetArea ();
		WPAreaToClientArea (rect);
		InvalidateRect (&rect);

		BuildWatchpointText (pWP);

		UpdateWatchpointExtent (pWP, &dc);
		rect = pClientData->GetArea ();
		WPAreaToClientArea (rect);
		InvalidateRect (&rect);

		UpdateWindow ();
	}
	else
	{
		COleServerItem *pItem;

		BuildWatchpointText (pWP);

		pItem = pDoc->GetEmbeddedItem ();
		ASSERT (pItem != NULL);
		if (pItem != NULL)
		{
			pItem->NotifyChanged ();
		}
	}
}

/*
template<class T>
void CPrgEditorView<T>::UpdateWatchpointArea (CWatchpoint *pWP, CDC *pDC)
{
   CPrgWPClientData *pClientData;
   long lDisplayX;
   long lDisplayY;
   long lDisplayOffsetX;
   long lDisplayOffsetY;
   CRect rect;

   pClientData = (CPrgWPClientData *)pWP->GetClientData ();
   ASSERT (pClientData != NULL);
   if (pClientData == NULL)
   {
      return;
   }

   pClientData->GetDisplay (lDisplayX, lDisplayY, lDisplayOffsetX, lDisplayOffsetY);

   rect.left = rect.right  = lDisplayX * m_iZGrid + lDisplayOffsetX;
   rect.top  = rect.bottom = lDisplayY * m_iZGrid + lDisplayOffsetY;

   pClientData->SetArea (rect);

   UpdateWatchpointExtent (pWP, pDC);
}
*/

template<class T>
void CPrgEditorView<T>::OnHideWatchpoint (CWatchpoint * /*pWP*/)
{
   if ( CheckInPlaceActive())
   {
      Invalidate ();
      UpdateWindow ();
   }
   else
   {
      COleServerItem *pItem;

      pItem = ((CPrgEditorDoc*)GetDocument())->GetEmbeddedItem ();
      assert (pItem != NULL);
      if (pItem != NULL)
      {
         pItem->NotifyChanged ();
      }
   }
}


template <class T>
void CPrgEditorView<T>::UpdateWatchpointExtent (CWatchpoint *pWP, CDC *pDC)
{
   CPrgWPClientData *pClientData;
   CFont *pOldFont=NULL;
   CSize extent;
   CRect rect;

   pClientData = (CPrgWPClientData *)pWP->GetClientData ();
   ASSERT (pClientData != NULL);
   if (pClientData == NULL)
   {
      return;
   }

   rect = pClientData->GetArea ();

   if (m_bHaveWPFont)
   {
      pOldFont = pDC->SelectObject (&m_WatchpointFont);
   }

   extent = pDC->GetTextExtent (pClientData->GetText ());
   extent.cx += 2 * WPHorizontalMargin;
   extent.cy += 2 * WPVerticalMargin;

   rect.right  = rect.left + extent.cx;
   rect.bottom = rect.top  + extent.cy;

   pClientData->SetArea (rect);

   if (m_bHaveWPFont && pOldFont)
   {
      pDC->SelectObject (pOldFont);
   }
}


template<class T>
CString CPrgEditorView<T>::GetContextExpression (CPoint /*point*/, CStdEditorPosition &/*startPosition*/, CStdEditorPosition &/*endPosition*/)
{
    ASSERT(0 && "override this method!");
    return _T("");
}



#endif // !defined(AFX_PRGEDITORVIEW_H__4B43F3D3_2348_11D5_B1FE_00E029440C3D__INCLUDED_)
