/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CSrvrItem.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CSrvrItem.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CSrvrItem.cpp $
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
#include "CSrvrItem.h"
//----  Static Initializations:   ----------------------------------*



// CSrvrItem.cpp : Implementierung der Klasse CELibrarianSrvrItem
//



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELibrarianSrvrItem Implementierung

IMPLEMENT_DYNAMIC(CELibrarianSrvrItem, CDocObjectServerItem)

CELibrarianSrvrItem::CELibrarianSrvrItem(CELibrarianDoc* pContainerDoc)
	: CDocObjectServerItem(pContainerDoc, TRUE)
{
	// ZU ERLEDIGEN: Hier Code für One-Time-Konstruktion einfügen
	//  (z.B. Hinzufügen von zusätzlichen Zwischenablageformaten zur Datenquelle des Elements)
}

CELibrarianSrvrItem::~CELibrarianSrvrItem()
{
	// ZU ERLEDIGEN: Hier Bereinigungscode hinzufügen
}

void CELibrarianSrvrItem::Serialize(CArchive& ar)
{
	// CELibrarianSrvrItem::Serialize wird automatisch aufgerufen, wenn
	//  das Element in die Zwischenablage kopiert wird. Dies kann automatisch 
	//  über die OLE-Rückruffunktion OnGetClipboardData geschehen. Ein Standardwert für
	//  das eingebundene Element dient einfach zur Delegierung der Serialisierungsfunktion des 
	//  Dokuments. Wenn Sie Verweise unterstützen, möchten Sie vielleicht nur einen Teil des
	//  Dokuments serialisieren.

	if (!IsLinkedItem())
	{
		CELibrarianDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CELibrarianSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// Die meisten Anwendungen (wie auch diese) unterstützen als einzigen Aspekt das Zeichnen
	//  des Elementinhalts. Wollen Sie andere Aspekte unterstützen, wie z.B.
	//  DVASPECT_THUMBNAIL (durch Überladen von OnDrawEx), so sollte diese
	//  Implementierung von OnGetExtent dahingehend modifiziert werden, dass sie
	//  zusätzliche Aspekte verarbeiten kann.

	if (dwDrawAspect != DVASPECT_CONTENT)
		return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CELibrarianSrvrItem::OnGetExtent wird aufgerufen, um das Extent in 
	//  HIMETRIC-Einheiten des gesamten Elements zu ermitteln. Die Standardimplementierung
	//  liefert hier einfach eine fest programmierte Einheitenanzahl zurück.

	CELibrarianDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// ZU ERLEDIGEN: Ersetzen Sie diese willkürlich gewählte Größe

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC Einheiten

	return TRUE;
}

BOOL CELibrarianSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	// Entfernen Sie dies, wenn Sie rSize verwenden
	UNREFERENCED_PARAMETER(rSize);

	CELibrarianDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// ZU ERLEDIGEN: Setzen Sie Mapping-Modus und Extent
	//  (Das Extent stimmt üblicherweise mit der von OnGetExtent zurückgelieferten Größe überein)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// ZU ERLEDIGEN: Hier Code zum Zeichnen einfügen. Füllen Sie wahlweise das HIMETRIC-Extent aus.
	//  Alle Zeichenoperationen finden innerhalb des Metadatei-Gerätekontexts (pDC) statt.

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianSrvrItem Diagnose

#ifdef _DEBUG
void CELibrarianSrvrItem::AssertValid() const
{
	CDocObjectServerItem::AssertValid();
}

void CELibrarianSrvrItem::Dump(CDumpContext& dc) const
{
	CDocObjectServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////


/*
 *----------------------------------------------------------------------------*
 *  $History: CSrvrItem.cpp $
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
