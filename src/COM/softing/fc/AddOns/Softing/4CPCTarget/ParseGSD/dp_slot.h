#ifndef __DP_SLOT_H_
#define __DP_SLOT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "defs.h"
//----  Forward Class Definitions:   -------------------------------*
class yy_parse;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef CMFCObjStore<IModule> ModuleList;
//----------------------------------------------------------------------------*
/**
 *  class CDP_Slot
 *  Object representing a slot description with module assignment 
 *  of the GSD file. The object exposes the interface ISlot to access the 
 *  properties of the slot description (e.g. slot name, default module etc.). 
 *  Via the method get_ModuleCollection() a collection of module objects that 
 *  can be mounted into the slot can be accessed.
 *
 *  @see    CDP_SlotColl (slot collection), CDP_ModuleColl (module collection).
*/
class ATL_NO_VTABLE CDP_Slot : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<ISlot, &IID_ISlot, &LIBID_PARSEGSDLib,2,0>
{
    friend  yy_parse;       // needed to comfortably set private members from parser!
    friend  class gsd_Parser;

//--------------------------
// =Constructor
public:
            CDP_Slot();
	virtual ~CDP_Slot();
    void    FinalRelease();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CDP_Slot)
COM_INTERFACE_ENTRY(IDispatch)
COM_INTERFACE_ENTRY(ISlot)
END_COM_MAP()

//--------------------------
// =Interface
// ISlot
	STDMETHOD(get_SlotNumber)(LONG * pVal);
	STDMETHOD(get_SlotName)(BSTR * psVal);
	STDMETHOD(get_DefaultModule)(IModule * * pp);
	STDMETHOD(get_ModuleCollection)(IModuleCollection** pp);

    void AddModuleReferences(CList<long,long>*);
    void SlotAssignment(CStationCallback*);
private:
    long                m_lSlotNo;
    CString             m_strName;
    long                m_lDefRef;
    CList<long,long>    m_ModRefList;
    IModule*            m_pDefaultModule;
    ModuleList          m_Modules;
};

typedef CComObject<CDP_Slot> CDP_SlotObject;
typedef CMFCObjStore<CDP_SlotObject> listSlot;

#endif // __DP_SLOT_H_


