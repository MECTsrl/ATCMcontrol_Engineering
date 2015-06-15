//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOCltElement.h                                              |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Element Class for Client Toolkits                           |
//                - SOCltElement: Element for runtime objects                 |
//                - SOCltElementIError: Error interface                       |
//                - SOCltElementErrorStore: Store the errors in the element	  |
//                - SOCltCallback: application callback                       |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLTELEMENT_H_
#define _SOCLTELEMENT_H_

#include "SOCmnElement.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOCltElementIError                                                         |
//-----------------------------------------------------------------------------

class SOCLT_EXPORT SOCltElementIError
{
public:	
	SOCltElementIError();
	virtual ~SOCltElementIError();

	// last error
	virtual HRESULT getLastError(OUT OPTIONAL DWORD *errorcode = NULL);
	virtual BOOL setLastError(IN HRESULT res, IN DWORD errorcode);
	virtual BOOL getErrorDescription(IN DWORD errorcode, OUT SOCmnString& str, IN LCID lcid);

protected:
	virtual void onError(IN HRESULT res, IN DWORD errorcode);

	virtual LPCTSTR getIntTraceId(void);
	virtual SOCmnObject *getObjectCltElementIError(void);
}; // SOCltElementIError




//-----------------------------------------------------------------------------
// SOCltElementErrorStore                                                     |
//-----------------------------------------------------------------------------

class SOCLT_EXPORT SOCltElementErrorStore :	virtual public SOCltElementIError
{
public:	
	SOCltElementErrorStore();

	// last error
	virtual HRESULT getLastError(OUT DWORD *errorcode);
	virtual BOOL setLastError(IN HRESULT res, IN DWORD errorcode);

protected:
	HRESULT	m_lastError;			// includes the last system error
	DWORD m_lastErrorCode;			// toolbox error code of last error
}; // SOCltElementErrorStore




//-----------------------------------------------------------------------------
// SOCltElement                                                               |
//-----------------------------------------------------------------------------

// object state flags for server, group and item objects
#define SOCLT_STATE_FLAG_CONNECTED		0x01
#define SOCLT_STATE_FLAG_STARTED		  0x02

#define SOCLT_EXEC_ASYNC       NULL
#define SOCLT_EXEC_AUTO_SYNC   INVALID_HANDLE_VALUE

class SOCltStorage;
class SOCltWorkerThread;
class SOCltTask;


class SOCLT_EXPORT SOCltElement : 
	public SOCmnElement,
	virtual public SOCltElementIError
{
	friend class SOCltTaskStateTransition;
  friend class SOCltTaskUpdate;
  friend class SOCltTaskConnectionMonitor;

public:
	SOCltElement(IN DWORD objType);

	enum elementState
	{
		disconnected = 0,
		connected    = SOCLT_STATE_FLAG_CONNECTED,
		started      = SOCLT_STATE_FLAG_CONNECTED | SOCLT_STATE_FLAG_STARTED
	};

	// overrides
	virtual BOOL setObjectState(IN BYTE state);
	virtual BOOL setTargetObjectState(IN BYTE state);
	virtual BOOL setParentBranch(IN SOCmnElement *parent);	// parent branch

	// state transition
	virtual BOOL start(IN OPTIONAL BOOL deep = TRUE, IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);
	virtual BOOL connect(IN OPTIONAL BOOL deep = TRUE, IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);		
	virtual BOOL disconnect(IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);	

	BOOL isConnected(void);		// is object connected?
	BOOL isStarted(void);			// is object started?

	// query COM interface
	virtual HRESULT queryInterface(IN REFIID iid, OUT void **interf);

	// update attributes
	virtual BOOL update(IN BOOL fromServer, IN OPTIONAL DWORD whatParameters = 0xFFFFFFFF, IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);

	// serialization
	virtual BOOL serialize(IN OUT SOCltStorage& ar, IN BOOL isStoring);

  // object persistence
	BOOL setPersistence(IN BOOL persist);
	BOOL getPersistence(void) const;

	virtual SOCltWorkerThread* getWorkerThread(void);

	virtual void beforeInterfaceCall(void);
	virtual void afterInterfaceCall(void);

protected:

  BYTE m_persist;					    // should object be stored in configuration?

  // execute task
	BOOL executeTask(IN SOCltTask* task, IN OPTIONAL HANDLE handle = SOCLT_EXEC_AUTO_SYNC);

  // Implementation of state transitions 
	virtual HRESULT doStateTransition(BOOL deep);

  // Recurse children at state transition
  virtual HRESULT doRecurseChlidren(SOCmnList<SOCmnElement>* plist, BYTE state);

  // Implementation of the update
  virtual HRESULT doUpdate(IN BOOL fromServer, IN OPTIONAL DWORD whatParameters);

  // Force the object to disconnect
  virtual HRESULT doDisconnect();

	virtual SOCmnObject *getObjectCltElementIError(void);

  virtual ~SOCltElement();
}; // SOCltElement

inline BOOL SOCltElement::isConnected(void)
{ return ((getObjectState() & connected) == connected); }

inline BOOL SOCltElement::isStarted(void)
{ return ((getObjectState() & started) == started); }

inline BOOL SOCltElement::getPersistence(void) const
{ return m_persist; }

inline BOOL SOCltElement::setPersistence(IN BOOL persist)
{ m_persist = (BYTE)persist; return TRUE; }




//-----------------------------------------------------------------------------
// SOCltCallback                                                              |
//-----------------------------------------------------------------------------

typedef void (SOCmnObject::*SOCLTELEMENT_CALLBACKTYPE)(void);
typedef void (SOCmnObject::*SOCLTELEMENT_CALLBACKTYPE_ERROR)(HRESULT,DWORD);
typedef SOCmnObject* SOCLTELEMENT_VTBL;

class SOCLT_EXPORT SOCltCallback : public SOCmnData
{
public:
	SOCltCallback()
	{
		e = 0;	
		vtlb = 0;
		type = 0;
	}

	virtual void execute(void) const
	{
		_ASSERTION( 0 != vtlb || 0 != e , "Invalid callback-data");
		switch( type )
		{	
			default:
				_ASSERTION( FALSE, "Invalid callback type"); break;
			case 0 : (vtlb->*f)(); break;
			case 1 : (vtlb->*f1.ferr)(f1.dw1, f1.dw2); break;
		}		
	}

	DWORD type;
	union // switch[type]
	{
		SOCLTELEMENT_CALLBACKTYPE f;	

		struct _f1
		{
			SOCLTELEMENT_CALLBACKTYPE_ERROR ferr;
			DWORD dw1;
			DWORD dw2;
		} f1;
	};

	SOCmnObject* e;									// pointer to object
	SOCLTELEMENT_VTBL vtlb;					// v-table, may be different from the default one
																	// if multiple inheritance is used
	virtual ~SOCltCallback(void)
	{
		if (e) e->release();
	}
}; // SOCltCallback

void SOCLT_EXPORT SOCltInvokeCallback(const SOCltCallback& clb);

// On this pointer
#define INVOKE_CALLBACK( cls, fcn )											\
		{ SOCltCallback _clb;																\
			(_clb.e = this)->addRef();												\
			_clb.vtlb = (SOCLTELEMENT_VTBL)(cls##*)this;			\
			_clb.f = (SOCLTELEMENT_CALLBACKTYPE)&cls##::fcn ;	\
			SOCltInvokeCallback(_clb);}												\


// On this pointer, virtual public class
#define INVOKE_CALLBACK_ON( vp, cls, fcn )							\
		{ if(vp) { SOCltCallback _clb;											\
			(_clb.e = vp)->addRef();													\
			_clb.vtlb = (SOCLTELEMENT_VTBL)(cls##*)this;			\
			_clb.f = reinterpret_cast<SOCLTELEMENT_CALLBACKTYPE>(&cls##::fcn) ;	\
			SOCltInvokeCallback(_clb);}}											\

// On object pointer
#define INVOKE_CALLBACK_ON_PTR( obj, cls, fcn )					\
		{ if(obj) { SOCltCallback _clb;											\
			(_clb.e = obj)->addRef();													\
			_clb.vtlb = (SOCLTELEMENT_VTBL)(cls##*)obj;				\
			_clb.f = (SOCLTELEMENT_CALLBACKTYPE)(&cls##::fcn) ;	\
			SOCltInvokeCallback(_clb);}}											\


#pragma pack(pop)
#endif 

