/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/iecResource.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: iecResource.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/iecResource.cpp $
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
#include "iecResource.h"
#include "Security.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "NSpaceHolder.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS IEC_Resource
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
IEC_Resource::IEC_Resource()
    : GenericNode(GENOBJ_TYPE_LEAF|GENOBJ_TYPE_BRANCH,GENLIST_CREATE_MAP,GENLIST_CREATE_MAP,10,10,101,101)
{
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
IEC_Resource::~IEC_Resource()
{
}

/* ---------------------------------------------------------------------------- */
