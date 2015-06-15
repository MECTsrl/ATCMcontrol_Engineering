

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"
#include "GrEditor.h"
#include "ElemPtrList.h"
#include "Elem.h"
#include "GrEditorAction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*const*=====================================================================*/

#define MAX_STACK   22

/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/

class TActRec
{
public:
   CMemFile *mf;

   TActRec();
   ~TActRec();
};

TActRec::TActRec()
{
   mf     = new CMemFile;
   ASSERT( mf );
   ASSERT_VALID(mf);
}

TActRec::~TActRec()
{
   try
   {
      if( mf ) 
      {
         ASSERT_VALID(mf);
         delete mf;
         mf = NULL;
      }
   }
   catch( CException* e )
   {
      e->Delete();
   }

}

/*locvar*====================================================================*/

// BYTE  bBuffer[20000];

/*proto*=====================================================================*/
/*func*======================================================================*/
/*
void CAction::test(char *s)
{
   char str[256];

   sprintf(str, "GrEditor Undo/Redo:[%s] idx:%d  upper:%d\n", 
      s, m_iIdx, m_Hist.GetUpperBound());
   ::OutputDebugString(str);
}
*/
/*func*------------------------------------------------------------------------
  Construction/Destruction
  in :
  out:
-----------------------------------------------------------------------------*/
CAction::CAction()
{
   m_iIdx = m_Hist.GetUpperBound();
   m_iChangeLog = 0;
}

CAction::~CAction()
{
   DeleteAll();
}

/*func*------------------------------------------------------------------------
  delete all entries in history
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CAction::DeleteAll()
{
   TActRec *pAR;

   while ( m_Hist.GetSize() > 0 )
   {
      pAR = (TActRec*) m_Hist.GetAt( m_Hist.GetUpperBound() ); 
      m_Hist.RemoveAt( m_Hist.GetUpperBound() );
      if( pAR )
      {
         delete pAR;
      }
   }
   m_iIdx = m_Hist.GetUpperBound();
   m_iChangeLog = 0;
}


/*func*------------------------------------------------------------------------
  description:
  in         :
  out        :
-----------------------------------------------------------------------------*/
void CAction::StoreState(CElemList *pList)
{
   TActRec *pAR;
   int      i;
   
   m_iChangeLog++;
   // new entry is now the last entry
   if( m_iIdx < m_Hist.GetUpperBound() )
   {
      for( i=m_Hist.GetUpperBound(); i>m_iIdx; i-- )
      {
         pAR = (TActRec*) m_Hist.GetAt( i ); 
         m_Hist.RemoveAt( i );
         if( pAR )
         {
            delete pAR;
         }
      }
   }
   // add entry
   pAR = (TActRec*) new TActRec;
   ASSERT(pAR);
   ASSERT(pAR->mf);
   if (pAR && pAR->mf)
   {
      pAR->mf->SeekToBegin(); 
      CArchive ar( pAR->mf, CArchive::store); 
      pList->Serialize(ar); 

      m_Hist.Add(pAR);
      m_iIdx++;
      ASSERT(m_iIdx == m_Hist.GetUpperBound());
   }
   // work around: limited stack size
   if( m_iIdx > MAX_STACK )
   {
      pAR = (TActRec*) m_Hist.GetAt( 0 ); 
      m_Hist.RemoveAt( 0 );
      if( pAR )
      {
         delete pAR;
      }

      m_iIdx--;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in         :
  out        :
-----------------------------------------------------------------------------*/
void CAction::LoadState(CElemList *pList)
{
   TActRec *pAR;

   m_iChangeLog--;

   if( m_iIdx > 0 )
   {
      m_iIdx--;
      pAR = (TActRec*) m_Hist.GetAt( m_iIdx );
      ASSERT(pAR && pAR->mf);
      ASSERT_VALID(pAR->mf);
      if( pAR && pAR->mf )
      {
         pAR->mf->SeekToBegin(); 
         {  // live time of atchive !
            CArchive ar( pAR->mf, CArchive::load); 
            pList->Serialize(ar);
            pList->IsOneOverlapped();
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  load forward, redo
  in :
  out:
-----------------------------------------------------------------------------*/
void CAction::ReloadState(CElemList *pList)
{
   TActRec *pAR;

   m_iChangeLog++;

   if( m_iIdx < m_Hist.GetUpperBound() )
   {
      m_iIdx++;
      pAR = (TActRec*) m_Hist.GetAt( m_iIdx );
      ASSERT(pAR && pAR->mf);
      ASSERT_VALID(pAR->mf);
      if( pAR && pAR->mf )
      {
         pAR->mf->SeekToBegin(); 
         CArchive ar( pAR->mf, CArchive::load); 
         pList->Serialize(ar);
         pList->IsOneOverlapped();
      }
   }
}

/*func*------------------------------------------------------------------------
  abort last operation, load last valid state again
  in :
  out:
-----------------------------------------------------------------------------*/
void CAction::ReloadLastState(CElemList *pList)
{
   TActRec *pAR;

   if ((m_iIdx <= m_Hist.GetUpperBound()) && 
       (m_Hist.GetSize() > 0))
   {
      pAR = (TActRec*) m_Hist.GetAt( m_iIdx );
      ASSERT(pAR && pAR->mf);
      ASSERT_VALID(pAR->mf);
      if( pAR && pAR->mf )
      {
         pAR->mf->SeekToBegin(); 
         CArchive ar( pAR->mf, CArchive::load); 
         pList->Serialize(ar);
         pList->IsOneOverlapped();
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in         :
  out        :
-----------------------------------------------------------------------------*/
void CAction::RemoveLastState()
{
   TActRec *pAR;

   m_iChangeLog--;

   if( m_iIdx > 0 )
   {
      m_iIdx--;
      pAR = (TActRec*) m_Hist.GetAt( m_iIdx ); 
      m_Hist.RemoveAt( m_iIdx );
      ASSERT(pAR);
      if( pAR )
      {
         delete pAR;
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in         :
  out        :
-----------------------------------------------------------------------------*/
BOOL CAction::IsReloadAvail()
{
   return (BOOL)(m_iIdx < m_Hist.GetUpperBound());
}

/*func*------------------------------------------------------------------------
  description:
  in         :
  out        :
-----------------------------------------------------------------------------*/
BOOL CAction::IsLoadAvail()
{
   return (BOOL)(m_iIdx > 0);
}


bool CAction::StateChangedQ(bool bReset)  // false if the load-state is displayed
{
   if(bReset)
   {
      m_iChangeLog = 1;
   }

   return m_iChangeLog != 1;
}
