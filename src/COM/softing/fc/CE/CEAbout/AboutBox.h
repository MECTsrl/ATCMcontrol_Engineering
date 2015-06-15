
#ifndef __ABOUTBOX_H_
#define __ABOUTBOX_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#pragma once

#include "axwin2.h"
#include "webextdisp.h"
#include "buildnr.h"
#include "product.h"

class ATL_NO_VTABLE CAboutBoxExternalDispatch : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAboutBoxExternalDispatch>,
  public IWebBrowserExternalDispatchImpl<CAboutBoxExternalDispatch>
{
public:

  CAboutBoxExternalDispatch()
    : m_hWndParent(NULL)
  {
  }

BEGIN_COM_MAP(CAboutBoxExternalDispatch)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


BEGIN_WDISPATCH_MAP(CAboutBoxExternalDispatch)
  WDISPATCH_METHOD_ENTRY(UpdateDialog)
  WDISPATCH_METHOD_ENTRY(CloseDialog)
END_WDISPATCH_MAP()

  STDMETHOD(UpdateDialog)()
  {
    USES_CONVERSION;

    CString strVersion;
    CString strBuild;
    CString strLicense;
    CString strCopyright;

//    BOOL bDemo = TRUE;

    strVersion.Format(_T("%s"), PROD_SHORT_VERSION_STR);
    strBuild.Format(_T("%s"), PROD_VERSION_STR);
    strCopyright.Format(_T("%s"), PROD_COPYRIGHT);

            SetThumbnailElement(_T("idVersion"), strVersion);
            SetThumbnailElement(_T("idBuild"), strBuild);
    SetThumbnailElement(_T("idCopyright"), strCopyright);

    // Check if this is a demo version

//    long dwFeatureBits;
//    long nHoursLeft;
//    long nLicMedia = SLIC_MEDIA_ANY;
//
//    // Call to the licensing API
//
//    if ( SLIC_OK == SLIC_GetLicenseForProduct(
//        SPROD_PROFIBUS_OPC_CONFIG, 
//        &dwFeatureBits,
//        &nHoursLeft,
//        &nLicMedia) )
//    {
//       bDemo = FALSE;
//    }
//
//    if ( !bDemo )
//    {
//      char szKey[SLIC_RKEY_CHARS+1];
//      szKey[0] = 0;
//
//      if ( SLIC_OK == SLIC_GetRegistrationKey( SPROD_PROFIBUS_OPC_CONFIG, szKey, _countof(szKey)) )
//      {
//        SetThumbnailElement(_T("idLicenseKey"), A2T(szKey));
//      }
//    }

    // Hide the appropriate HTML-tag

//    CComPtr<IHTMLElement> spElem;
//    if ( GetThumbnailElement( bDemo ? _T("idDemo") : _T("idRetail") , &spElem) )
//    {
//      CComPtr<IHTMLStyle> spStyle;
//      spElem->get_style( &spStyle ); 
//      if ( spStyle )
//      {
//        spStyle->put_visibility( CComBSTR(L"visible") );
//      }
//    }  
    
    return S_OK;
  }

  void SetThumbnailElement(LPCTSTR szID, LPCTSTR lpszHTML)
  {
    CComPtr<IHTMLElement> spElem;
    if ( GetThumbnailElement(szID, &spElem) )
    {
      spElem->put_innerHTML( CComBSTR(lpszHTML) ); 
    }      
  }
  
  bool GetThumbnailElement(LPCTSTR szID, IHTMLElement** ppElem)
  {
    *ppElem = 0;
    
    CComPtr<IDispatch> pDisp;
    HRESULT hr = m_spWebBrowser2->get_Document(&pDisp);

    CComQIPtr<IHTMLDocument2> spDoc2(pDisp);
    if ( !spDoc2 ) return false;

    CComPtr<IHTMLElementCollection> spAll;
    spDoc2->get_all( &spAll );
    if ( !spAll ) return false;

    pDisp = NULL;
    spAll->item(CComVariant(szID), CComVariant(0), &pDisp); 
    if ( !pDisp ) return false;

    CComQIPtr<IHTMLElement> spElem(pDisp);
    if ( spElem )
    {
      (*ppElem = spElem)->AddRef(); 
    }

    return *ppElem ? true : false;
  }

  STDMETHOD(CloseDialog)()
  {
    ::PostMessage( m_hWndParent, WM_COMMAND, MAKEWPARAM(IDOK,0), 0);
    return S_OK;
  }

  CComPtr<IWebBrowser2> m_spWebBrowser2;
  HWND m_hWndParent;
};


class CAboutBox : public CAxDialogImpl<CAboutBox>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutBox)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtrlColorDialog)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtrlColorStatic)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnCtrlColorDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
    return (LRESULT)::GetStockObject(WHITE_BRUSH);
  }

	LRESULT OnCtrlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
    return (LRESULT)::GetStockObject(WHITE_BRUSH);
  }

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
    HRESULT hr;

		CenterWindow(GetParent());

    CWindow txtAbout( GetDlgItem(IDC_STATIC_ABOUT) );
    CRect rc;
    txtAbout.GetWindowRect(rc);
    ScreenToClient(rc);
    txtAbout.DestroyWindow();


    m_ax.Create(m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE );
    hr = m_ax.CreateControl(IDR_HTML_ABOUT);

    // Disable context menu in the HTML

    CComPtr<IAxWinAmbientDispatch> spIAx;
    m_ax.QueryHost(&spIAx);

    if ( spIAx )
    {
      spIAx->put_AllowContextMenu(VARIANT_FALSE);
    }    

    CComPtr<IWebBrowser2> spWebBrowser2;
    hr = m_ax.QueryControl(&spWebBrowser2);

    if ( SUCCEEDED(hr) ) 
    {
      // Set external dispatch to update the HTML after
      // it has been loaded

      CComObject<CAboutBoxExternalDispatch>* pExtObj = NULL;
      hr = pExtObj->CreateInstance(&pExtObj);
      ATLASSERT( SUCCEEDED(hr) );

      pExtObj->m_spWebBrowser2 = spWebBrowser2;
      pExtObj->m_hWndParent = m_hWnd;
      m_ax.SetExternalDispatch(static_cast<IDispatch*>(pExtObj));
    }

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

  CAxWindow m_ax;
};

#endif // __ABOUTBOX_H_

