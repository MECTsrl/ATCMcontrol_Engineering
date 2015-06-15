/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fccallback.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fccallback.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fccallback.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
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
 *  14.12.2000  SIS      File copied from V1.3
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __FCCALLBACK_H_
#define __FCCALLBACK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//
// How to use:
//
// Callback providers (i.e. instances that call callbacks) have to
// define the callback with one of the FC_CALLBACK_DEF macros. E.g.
//
//		FC_CALLBACK_DEF2 (ProviderAction, int, double)
//
// defines a callback named ProviderAction which is able to call back a
// method with an int and a double parameter. The name has to be unique
// for all callbacks used in the same namespace. It is recommended to
// somehow integrate the callback providers name into the callback name
// as this reduces probability of name collisions. The callback definition
// has to be situated in a .h file that a client is able to include.
//
// Additionally callback providers have to provide some means of registering
// and unregistering callback instances. Especially they have to store re-
// gistered callbacks for to call them as soon as the calling condition is
// met. Since the callback provider knows about the base class of callback
// instances only it has to store references to callback instances (callbacks
// are polymorphic!!!). The callback base class name can be retrieved with the
// FC_CALLBACK_BASE macro. This macro takes the callback name and expands into
// the callback base class name. E.g. the callback base class name for the
// above defined callback is retrieved with
//
//		FC_CALLBACK_BASE (ProviderAction)
//
// Assume a simple callback provider that allows to register one single
// callback instance. It could be implemented as follows.
//
//		FC_CALLBACK_DEF2 (ProviderAction, int, double)
//
//		class Provider
//		{
//		private:
//			FC_CALLBACK_BASE (ProviderAction) *m_pCallback;
//
//		public:
//			Provider ()
//			{
//				m_pCallback = NULL;
//			}
//
//			void Register (FC_CALLBACK_BASE (ProviderAction) *pCallback);
//			{
//				m_pCallback = pCallback;
//			}
//
//			void Unregister ()
//			{
//				m_pCallback = NULL;
//			}
//
//			// assumed to be called as soon as callback condition is met
//			void ConditionMet (int i, double d)
//			{
//				if (m_pCallback != NULL)
//					m_pCallback->Execute (i, d);
//			}
//		};
//
//
// After having included the callback definition clients are capable of
// defining callback instances with the FC_CALLBACK macro. This macro takes
// the unique callback name and the class where the called back method re-
// sides. It expands into the callback class name. E.g. the callback class
// name for the above defined callback used in a class named Client is re-
// trieved like this.
//
//		FC_CALLBACK (ProviderAction, Client)
//
// Assume a client that registers at an instance of Provider which is defined
// above. It could be implemented as follows.
//
//		#include "Provider.h"
//
//		class Client
//		{
//		private:
//			Provider *m_pProvider;
//			FC_CALLBACK (ProviderAction, Client) m_Callback;
//
//			void OnProviderCall (int i, double d)
//			{
//				implement reaction on callback here
//			}
//
//		public:
//			Client (Provider *pProvider) : m_Callback (this, OnProviderCall)
//			{
//				m_pProvider = pProvider;
//				pProvider->Register (&m_Callback);
//			}
//
//			~Client ()
//			{
//				pProvider->Unregister ();
//			}
//		};
//


#define FC_CALLBACK_BASE(name) CCallbackBase##name
#define FC_CALLBACK_HOLDER(name) CCallbackHolder##name
#define FC_CALLBACK(name,calleeClass) CCallback##name<calleeClass>
#define FC_CALLBACK_METHOD(name, calleeClass) CCallback##name<calleeClass>::MethodType

#define FC_CALLBACK_DEF(name)\
	class CCallbackBase##name\
	{\
	public:\
		virtual void Execute () = 0;\
	};\
	template<class calleeClass> class CCallback##name : public CCallbackBase##name\
	{\
	public:\
		typedef void (calleeClass::*MethodType)();\
\
	private:\
		calleeClass *m_pCallee;\
		MethodType  m_pMethod;\
\
	public:\
		CCallback##name (calleeClass *pCallee, MethodType pMethod)\
		{\
			m_pCallee = pCallee;\
			m_pMethod = pMethod;\
		}\
\
		virtual void Execute ()\
		{\
			(m_pCallee->*m_pMethod) ();\
		}\
	};\
	class CCallbackHolder##name : public CList<CCallbackBase##name *, CCallbackBase##name *>\
	{\
	public:\
        void Add (CCallbackBase##name *pCB)\
        {\
            AddTail (pCB);\
        }\
\
        void Remove (CCallbackBase##name *pCB)\
        {\
            POSITION pPosition;\
\
            pPosition = Find (pCB);\
            if (pPosition != NULL)\
            {\
                RemoveAt (pPosition);\
            }\
        }\
\
		virtual void Execute ()\
		{\
			POSITION pPosition;\
\
			pPosition = GetHeadPosition ();\
			while (pPosition != NULL)\
			{\
				CCallbackBase##name *pCB;\
\
				pCB = GetNext (pPosition);\
				pCB->Execute ();\
			};\
		};\
	};

#define FC_CALLBACK_DEF1(name, type1)\
	class CCallbackBase##name\
	{\
	public:\
		virtual void Execute (type1) = 0;\
	};\
	template<class calleeClass> class CCallback##name : public CCallbackBase##name\
	{\
	public:\
		typedef void (calleeClass::*MethodType)(type1);\
\
	private:\
		calleeClass *m_pCallee;\
		MethodType  m_pMethod;\
\
	public:\
		CCallback##name (calleeClass *pCallee, MethodType pMethod)\
		{\
			m_pCallee = pCallee;\
			m_pMethod = pMethod;\
		}\
\
		virtual void Execute (type1 p1)\
		{\
			(m_pCallee->*m_pMethod) (p1);\
		}\
	};\
	class CCallbackHolder##name : public CList<CCallbackBase##name *, CCallbackBase##name *>\
	{\
	public:\
        void Add (CCallbackBase##name *pCB)\
        {\
            AddTail (pCB);\
        }\
\
        void Remove (CCallbackBase##name *pCB)\
        {\
            POSITION pPosition;\
\
            pPosition = Find (pCB);\
            if (pPosition != NULL)\
            {\
                RemoveAt (pPosition);\
            }\
        }\
\
		virtual void Execute (type1 p1)\
		{\
			POSITION pPosition;\
\
			pPosition = GetHeadPosition ();\
			while (pPosition != NULL)\
			{\
				CCallbackBase##name *pCB;\
\
				pCB = GetNext (pPosition);\
				pCB->Execute (p1);\
			};\
		};\
	};

#define FC_CALLBACK_DEF2(name, type1, type2)\
	class CCallbackBase##name\
	{\
	public:\
		virtual void Execute (type1, type2) = 0;\
	};\
	template<class calleeClass> class CCallback##name : public CCallbackBase##name\
	{\
	public:\
		typedef void (calleeClass::*MethodType)(type1, type2);\
\
	private:\
		calleeClass *m_pCallee;\
		MethodType  m_pMethod;\
\
	public:\
		CCallback##name (calleeClass *pCallee, MethodType pMethod)\
		{\
			m_pCallee = pCallee;\
			m_pMethod = pMethod;\
		}\
\
		virtual void Execute (type1 p1, type2 p2)\
		{\
			(m_pCallee->*m_pMethod) (p1, p2);\
		}\
	};\
	class CCallbackHolder##name : public CList<CCallbackBase##name *, CCallbackBase##name *>\
	{\
	public:\
        void Add (CCallbackBase##name *pCB)\
        {\
            AddTail (pCB);\
        }\
\
        void Remove (CCallbackBase##name *pCB)\
        {\
            POSITION pPosition;\
\
            pPosition = Find (pCB);\
            if (pPosition != NULL)\
            {\
                RemoveAt (pPosition);\
            }\
        }\
\
		virtual void Execute (type1 p1, type2 p2)\
		{\
			POSITION pPosition;\
\
			pPosition = GetHeadPosition ();\
			while (pPosition != NULL)\
			{\
				CCallbackBase##name *pCB;\
\
				pCB = GetNext (pPosition);\
				pCB->Execute (p1, p2);\
			};\
		};\
	};

#define FC_CALLBACK_DEF3(name, type1, type2, type3)\
	class CCallbackBase##name\
	{\
	public:\
		virtual void Execute (type1, type2, type3) = 0;\
	};\
	template<class calleeClass> class CCallback##name : public CCallbackBase##name\
	{\
	public:\
		typedef void (calleeClass::*MethodType)(type1, type2, type3);\
\
	private:\
		calleeClass *m_pCallee;\
		MethodType  m_pMethod;\
\
	public:\
		CCallback##name (calleeClass *pCallee, MethodType pMethod)\
		{\
			m_pCallee = pCallee;\
			m_pMethod = pMethod;\
		}\
\
		virtual void Execute (type1 p1, type2 p2, type3 p3)\
		{\
			(m_pCallee->*m_pMethod) (p1, p2, p3);\
		}\
	};\
	class CCallbackHolder##name : public CList<CCallbackBase##name *, CCallbackBase##name *>\
	{\
	public:\
        void Add (CCallbackBase##name *pCB)\
        {\
            AddTail (pCB);\
        }\
\
        void Remove (CCallbackBase##name *pCB)\
        {\
            POSITION pPosition;\
\
            pPosition = Find (pCB);\
            if (pPosition != NULL)\
            {\
                RemoveAt (pPosition);\
            }\
        }\
\
		virtual void Execute (type1 p1, type2 p2, type3 p3)\
		{\
			POSITION pPosition;\
\
			pPosition = GetHeadPosition ();\
			while (pPosition != NULL)\
			{\
				CCallbackBase##name *pCB;\
\
				pCB = GetNext (pPosition);\
				pCB->Execute (p1, p2, p3);\
			};\
		};\
	};

#endif   // FCCALLBACK_H

/*
 *----------------------------------------------------------------------------*
 *  $History: fccallback.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Sis          Date: 14.12.00   Time: 16:53
 * Created in $/4Control/COM/softing/fc/inc
 * Copied from V1.3 and replaced header
 *==
 *----------------------------------------------------------------------------*
*/

