

#include "stdafx.h"
#include "CeWatchElement.h"
#include "CeWatchBackEnd.h"
#include "CESysDef.h"
#include "DbgInfo.h"
#include "CeSymbol.h"

#include "CeDisplayElement.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCeDisplayElement::CCeDisplayElement()
{
	m_bExpanded = false;
	m_VertLineMask = 0;
	m_pVar = NULL;

	m_hBackEnd = -1;

	m_DebugId = 4711;
}

CCeDisplayElement::~CCeDisplayElement()
{

}

void CCeDisplayElement::SaveExpandInfo()
{
    ASSERT(m_pVar);
    if(m_pVar)
    {
        m_pVar->SetInitExpanded(m_bExpanded);
    }
}


//*****************************************************************************	
bool IsValidDisplayElement(const CCeDisplayElement*  pDisplayElement) 
//*****************************************************************************	
{
	return 
		AfxIsValidAddress(pDisplayElement, sizeof(CCeDisplayElement)) && 
		(pDisplayElement->GetWatchElement() != NULL && IsValidWatchElement(pDisplayElement->GetWatchElement()));
}

