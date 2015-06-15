//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "dp_physicalinterface.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

CDP_PhysicalInterface::CDP_PhysicalInterface()
:   m_mapTXDelay(), m_mapRCDelay()
{
    m_lInterfaceId = MISSING;
}

CDP_PhysicalInterface::~CDP_PhysicalInterface()
{
    Cleanup();
}

void CDP_PhysicalInterface::Cleanup()
{
    m_mapTXDelay.RemoveAll();
    m_mapRCDelay.RemoveAll();
}

void CDP_PhysicalInterface::AddTXDelay(long lBaudRate, long lVal)
{
    BAUDRATE baudrate = static_cast<BAUDRATE>(lBaudRate);
    m_mapTXDelay.SetAt(baudrate,lVal);
}

void CDP_PhysicalInterface::AddRCDelay(long lBaudRate, long lVal)
{
    BAUDRATE baudrate = static_cast<BAUDRATE>(lBaudRate);
    m_mapRCDelay.SetAt(baudrate,lVal);
}

STDMETHODIMP CDP_PhysicalInterface::get_TransmissionDelay(tagBAUDRATE baudrate, LONG * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = MISSING;
    
    long lVal = 0;
    if (!m_mapTXDelay.Lookup(baudrate, lVal)) {
        *pVal = MISSING;
        return S_FALSE;
    }
    *pVal = lVal;
	return S_OK;
}

STDMETHODIMP CDP_PhysicalInterface::get_ReactionDelay(tagBAUDRATE baudrate, LONG * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = MISSING;
    
    long lVal = 0;
    if (!m_mapRCDelay.Lookup(baudrate, lVal)) {
        *pVal = MISSING;
        return S_FALSE;
    }
    *pVal = lVal;
	return S_OK;
}

STDMETHODIMP CDP_PhysicalInterface::get_InterfaceId(LONG *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lInterfaceId;
	return S_OK;
}


