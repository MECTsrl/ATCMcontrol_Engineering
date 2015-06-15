/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CntrItem.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CntrItem.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CntrItem.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CELibrarian.h"
#include "CELibrarianDoc.h"
#include "CELibrarianView.h"
#include "CntrItem.h"
//----  Static Initializations:   ----------------------------------*





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELibrarianCntrItem Implementierung

IMPLEMENT_SERIAL(CELibrarianCntrItem, COleClientItem, 0)

CELibrarianCntrItem::CELibrarianCntrItem(CELibrarianDoc* pContainer)
	: COleClientItem(pContainer)
{
	// ZU ERLEDIGEN: Hier Code für One-Time-Konstruktion einfügen
	
}

CELibrarianCntrItem::~CELibrarianCntrItem()
{
	// ZU ERLEDIGEN: Hier Bereinigungscode hinzufügen
	
}

void CELibrarianCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// Wird ein Element bearbeitet (entweder direkt oder voll geöffnet),
	//  sendet es OnChange-Benachrichtigungen für Änderungen im Status des
	//  Elements oder im visuellen Erscheinungsbild seines Inhalts.

	// ZU ERLEDIGEN: Element ungültig machen, indem Sie UpdateAllViews aufrufen
	//  (mit auf Ihre Anwendung zugeschnittenen Tips)

	GetDocument()->UpdateAllViews(NULL);
		// im Moment aktualisieren Sie einfach ALLE Ansichten/keine Tips
}

BOOL CELibrarianCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// Während einer direkten Aktivierung wird CELibrarianCntrItem::OnChangeItemPosition
	//  vom Server aufgerufen, um die Position des In-Place-Fensters 
	//  zu ändern. Üblicherweise ist dies ein Ergebnis von Datenänderungen 
	//  im Server-Dokument, etwa ein geändertes Extent oder als Ergebnis
	//  direkter Größenänderungen.
	//
	// Standardmäßig wird hier die Basisklasse aufgerufen, die wiederum 
	//  COleClientItem::SetItemRects zum Bewegen des Elements
	//  an die neue Position aufruft.

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// ZU ERLEDIGEN: Aktualisieren Sie alle für das Rechteck/das Extent dieses Elements angelegten Caches

	return TRUE;
}

void CELibrarianCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// Während einer direkten Aktivierung wird CELibrarianCntrItem::OnGetItemPosition
	//  zur Bestimmung der Position dieses Elements aufgerufen. Die durch
	//  den Anwendungsassistenten erstellte Standardimplementierung gibt einfach ein 
	//  fest einprogrammiertes Rechteck zurück. Normalerweise gibt dieses Rechteck 
	//  die aktuelle Position des Elements relativ zu der Ansicht an, die zur Aktivierung 
	//  verwendet wird. Sie erhalten die Ansicht, indem Sie CELibrarianCntrItem::GetActiveView aufrufen.

	// ZU ERLEDIGEN: Geben Sie das korrekte Rechteck (in Pixeln) in rPosition zurück

	rPosition.SetRect(10, 10, 210, 210);
}

void CELibrarianCntrItem::OnActivate()
{
    // Nur Elemente mit direkter Aktivierung pro Rahmen zulassen
    CELibrarianView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CELibrarianCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    // Objekt ausblenden, wenn es kein Outside-In-Objekt ist
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CELibrarianCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Vor dem Lesen von Daten aus COleClientItem zuerst Basisklasse aufrufen.
	// Da dies den von CELibrarianCntrItem::GetDocument zurückgegebenen
	//  m_pDocument-Zeiger einrichtet, es ist ratsam, zuerst die Basisklasse 
	//  Serialize aufzurufen.
	COleClientItem::Serialize(ar);

	// jetzt die speziellen Daten für CELibrarianCntrItem einlesen/speichern
	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Hier Code zum Speichern einfügen
	}
	else
	{
		// ZU ERLEDIGEN: Hier Code zum Laden einfügen
	}
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianCntrItem Diagnose

#ifdef _DEBUG
void CELibrarianCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CELibrarianCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////


/*
 *----------------------------------------------------------------------------*
 *  $History: CntrItem.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
