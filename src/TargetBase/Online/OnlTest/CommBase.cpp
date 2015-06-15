/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommBase.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommBase.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommBase.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlTest
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
#include "OnlTest.h"
#include "CommBase.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


IMPLEMENT_DYNAMIC(CCommBase, CObject);


/* CCommBase
 * ----------------------------------------------------------------------------
 */
CCommBase::CCommBase()
{
	m_dwType		= OCT_NO_TYPE;

	Init();
}


/* CCommBase
 * ----------------------------------------------------------------------------
 */
CCommBase::CCommBase(DWORD dwType)
{
	m_dwType		= dwType;

	Init();
}


/* ~CCommBase
 * ----------------------------------------------------------------------------
 */
CCommBase::~CCommBase()
{

}


/* Init
 * ----------------------------------------------------------------------------
 */
void CCommBase::Init()
{
	m_bLoaded		= FALSE;
	m_bConnected	= FALSE;
	m_bLogedIn		= FALSE;
}


/* GetType
 * ----------------------------------------------------------------------------
 */
DWORD CCommBase::GetType()
{
	return m_dwType;
}


/* IsLoaded
 * ----------------------------------------------------------------------------
 */
BOOL CCommBase::IsLoaded()
{
	return m_bLoaded;
}


/* IsConnected
 * ----------------------------------------------------------------------------
 */
BOOL CCommBase::IsConnected()
{
	return m_bConnected;
}


/* IsLogedIn
 * ----------------------------------------------------------------------------
 */
BOOL CCommBase::IsLogedIn()
{
	return m_bLogedIn;
}

/* ---------------------------------------------------------------------------- */
