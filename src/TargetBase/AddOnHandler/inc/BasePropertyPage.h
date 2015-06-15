#ifndef _BASEPROPERTYPAGE_H_
#define _BASEPROPERTYPAGE_H_

//------------------------------------------------------------------*
/**
 *  class CBasePropertyPage
 *  
 *  - base class for property pages used in target properties
 *    dialog
 *  - provides methods for updating and validation to be used within
 *    the OnOK() method of property sheet.
 *
 *  @see    ....
 */
class CBasePropertyPage : public CPropertyPage
{
public:
    CBasePropertyPage(UINT nIDTemplate, UINT nIDCaption = 0);
    
    virtual void UpdatePageData();
    virtual BOOL Validate();

};

#endif
