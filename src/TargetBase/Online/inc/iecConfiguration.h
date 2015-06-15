/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/iecConfiguration.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: iecConfiguration.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/iecConfiguration.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _IECDEVICE_H_
#define _IECDEVICE_H_

//------------------------------------------------------------------*
/**
 *  class IEC_Configuration
 *
 *  @see    ....
*/
class IEC_Configuration : public GenericNode
{
// =Construction
public:
                            IEC_Configuration(LPCTSTR pszConfigName);
                                // default ctor
protected:
    virtual                 ~IEC_Configuration();
                                // dtor

private:
                            IEC_Configuration(IEC_Configuration const & other);
                                // copy ctor
    IEC_Configuration const &    operator = (IEC_Configuration const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    void                    Disconnect(void)
    {
    }


// =Overrides
public:

protected:

// =Helpers
private:

// =Attributes
protected:

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
