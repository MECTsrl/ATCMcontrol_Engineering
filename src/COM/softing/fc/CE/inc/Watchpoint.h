
#ifndef Watchpoint_H
#define Watchpoint_H

class CSubscription;

#include "StdEdAttr.h"
#include "baselib.h"


class CWatchpoint : public CObject
{
public:
   CWatchpoint (const CStdEditorPosition &position, long lHandle);
   CWatchpoint (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, long lHandle);
   virtual ~CWatchpoint ();
   
   long GetHandle () const;
   bool IsExpressionWP () const;
   
   CStdEditorPosition GetPosition () const;
   CStdEditorPosition GetStartPosition () const;
   CStdEditorPosition GetEndPosition () const;

   void AttachWatchItem (LONG lHandle, BL_IEC_TYP iecType);
   LONG DetachWatchItem ();
   
   LONG GetWatchItem () const;
   BL_IEC_TYP GetIECType () const;
   
   void *GetClientData () const;
   void SetClientData (void *pClientData);
   
   bool IsSuspended () const;
   HRESULT Suspend ();
   HRESULT Resume ();
   
protected:
   long               m_lHandle;
   bool               m_bExpressionWP;
   CStdEditorPosition m_Position; // is now equal to m_StartPosition !!!
   CStdEditorPosition m_EndPosition;
   LONG               m_lWatchHandle;
   BL_IEC_TYP         m_IECType;
   void               *m_pClientData;
   bool               m_bSuspended;
};

#endif
