#ifndef _BASECOMMPROPERTYPAGE_H_
#define _BASECOMMPROPERTYPAGE_H_

#include "BasePropertyPage.h"

//------------------------------------------------------------------*
/**
 *  class CBaseCommPropertyPage
 *
 *  - abstract base class for communication property pages used in target 
 *    properties and in communication property dialog (wizard)
 *  - each page displays parameters for a certain communication channel
 *  - page may be enabled/disabled
 *  - derived from CBasePropertyPage
 *
 *  @see    CBasePropertyPage
 */
class CBaseCommPropertyPage : public CBasePropertyPage
{
public:
    // construction
    CBaseCommPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0);

    // communication channel
    virtual CString GetCommChannel() = 0;
    virtual CString GetCommChannelDescription() = 0;

    // connect parameters
    virtual void SetConnectParameter(const CString& crstrParameter) = 0;
    virtual CString GetConnectParameter() = 0;

    // enable/disable page
    virtual void EnablePage(BOOL bEnable = TRUE);
    virtual BOOL OnSetActive();

protected:
    virtual void EnableControls();

protected:
    BOOL    m_bEnablePage;      // page enabled?
};

#endif
