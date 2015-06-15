#ifndef __CEWINDOWPLACEMENT_H_
#define __CEWINDOWPLACEMENT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "XMLHelper/XMLNode.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CCEWindowPlacement
 *
 *  @see    ....
*/
class CCEWindowPlacement
{
public:
    CCEWindowPlacement(WINDOWPLACEMENT* pWindowPlacement = NULL);
    virtual ~CCEWindowPlacement();

    void InitNew();
    BOOL IsInitialized();

    void SetWindowPlacement(WINDOWPLACEMENT& rtWindowPlacement);
    BOOL GetWindowPlacement(WINDOWPLACEMENT& rtWindowPlacement);

    BOOL Load(CXMLNode& rtNode);
    BOOL Save(CXMLNode& rtNode);

    void Reset();

protected:
    WINDOWPLACEMENT m_tWindowPlacement;     // window placement structure
    BOOL            m_bInitialized;         // structure initialized?
};

#endif // __CEWINDOWPLACEMENT_H_