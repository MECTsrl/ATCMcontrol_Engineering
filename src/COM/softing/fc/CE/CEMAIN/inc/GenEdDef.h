
#ifndef __GENEDDEF_H_
#define __GENEDDEF_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#include "fccallback.h"

class CGenEdDoc;

FC_CALLBACK_DEF2 (GenEdDocSaveCB, CGenEdDoc *, bool &)

// Source document position
struct CE_SRCDOCPOS_TYP
{
	CMultiDocTemplate * pTemplate;
	POSITION            rPos;
};

#endif // __GENEDDEF_H_

