/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/iecConfiguration.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: iecConfiguration.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/iecConfiguration.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "iecConfiguration.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS IEC_Configuration
/////////////////////////////////////////////////////////////////////

// IMPLEMENT_DYNCREATE(IEC_Configuration, NDevice);
// IMPLEMENT_DYNAMIC(IEC_Configuration, NDevice);
// IMPLEMENT_SERIAL(IEC_Configuration, NDevice, IEC_Configuration::fmtSerial);

//------------------------ MESSAGE MAP -----------------------------*
//BEGIN_MESSAGE_MAP(IEC_Configuration, NDevice)
//    //{{AFX_MSG_MAP(IEC_Configuration)
//    //}}AFX_MSG_MAP
//END_MESSAGE_MAP()


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           pszConfigName   name of configuration
 * @return          void
 * @exception       -
 */
IEC_Configuration::IEC_Configuration(LPCTSTR pszConfigName)
{
    assert(pszConfigName != NULL);
    setName(pszConfigName);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
IEC_Configuration::~IEC_Configuration()
{
}

/* ---------------------------------------------------------------------------- */
