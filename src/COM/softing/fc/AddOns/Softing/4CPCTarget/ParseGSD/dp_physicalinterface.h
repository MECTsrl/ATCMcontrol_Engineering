#ifndef __DP_PHYSICALINTERFACE_H_
#define __DP_PHYSICALINTERFACE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "defs.h"
//----  Forward Class Definitions:   -------------------------------*
class   yy_parse;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//----------------------------------------------------------------------------*
/**
 *  class CDP_PhysicalInterface
 *  Object representing a physical interface description of the GSD file. 
 *  The object exposes the interface IPhysicalInterface to access the 
 *  parameters of the physical interface description.
 *
 *  @see    CDP_PhysicalInterfaceColl
*/
class ATL_NO_VTABLE CDP_PhysicalInterface : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IPhysicalInterface, &IID_IPhysicalInterface, &LIBID_PARSEGSDLib,2,0>
{
    friend  yy_parse;       // needed to comfortably set private members from parser!
    friend  class gsd_Parser;

//--------------------------
// =Constructor
public:
            CDP_PhysicalInterface();
	virtual ~CDP_PhysicalInterface();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CDP_PhysicalInterface)
COM_INTERFACE_ENTRY(IDispatch)
COM_INTERFACE_ENTRY(IPhysicalInterface)
END_COM_MAP()

//--------------------------
// Interface IPhysicalInterface
	STDMETHOD(get_TransmissionDelay)(tagBAUDRATE baudrate, LONG * pVal);
	STDMETHOD(get_ReactionDelay)(tagBAUDRATE baudrate, LONG * pVal);
    STDMETHOD(get_InterfaceId)(LONG *pVal);

    void AddTXDelay(long,long);
    void AddRCDelay(long,long);

private:
// =Operations
    void Cleanup();
// =Attributes
    long m_lInterfaceId;
    mapBaudrateToValue m_mapTXDelay;
    mapBaudrateToValue m_mapRCDelay;
};

typedef CComObject<CDP_PhysicalInterface> CDP_PIObject;
typedef CMFCObjStore<CDP_PIObject> listPI;

#endif // __DP_PHYSICALINTERFACE_H_

