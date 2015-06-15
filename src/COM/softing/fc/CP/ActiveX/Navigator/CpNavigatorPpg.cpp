// CpNavigatorPpg.cpp: Implementierung der Eigenschaftenseitenklasse CCpNavigatorPropPage.

#include "stdafx.h"
#include "CpNavigator.h"
#include "CpNavigatorPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCpNavigatorPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Nachrichtenzuordnungstabelle

BEGIN_MESSAGE_MAP(CCpNavigatorPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCpNavigatorPropPage)
	// HINWEIS - Der Klassen-Assistent fügt Nachrichtenzuordnungseinträge hinzu und entfernt diese
	//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Klassenerzeugung und GUID initialisieren

IMPLEMENT_OLECREATE_EX(CCpNavigatorPropPage, "CPNAVIGATOR.CpNavigatorPropPage.1",
	0xb3380725, 0x7157, 0x11d1, 0xa4, 0xa8, 0, 0xa0, 0x24, 0x36, 0x3d, 0xfc)


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorPropPage::CCpNavigatorPropPageFactory::UpdateRegistry -
// Fügt Einträge der Systemregistrierung für CCpNavigatorPropPage hinzu oder entfernt diese

BOOL CCpNavigatorPropPage::CCpNavigatorPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CPNAVIGATOR_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorPropPage::CCpNavigatorPropPage - Konstruktor

CCpNavigatorPropPage::CCpNavigatorPropPage() :
	COlePropertyPage(IDD, IDS_CPNAVIGATOR_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCpNavigatorPropPage)
	// HINWEIS: Der Klassen-Assistent fügt die Elementinitialisierung hier ein
	//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorPropPage::DoDataExchange - Verschiebt Daten zwischen Dialogfeld+++ und den Variablen+++

void CCpNavigatorPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCpNavigatorPropPage)
	// HINWEIS: Der Klassen-Assistent fügt  DDP-, DDX- und DDV-Aufrufe hier ein
	//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorPropPage-Behandlungsroutinen für Nachrichten
