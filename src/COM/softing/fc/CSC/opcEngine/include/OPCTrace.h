//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : OPCTRACE.H                                                  |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//                                                                            |
//  Description : Tracing classes                                             |
//                - OPCTrace: basic trancing class                            |
//                - GenericTraceHandler: external trace handler               |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  02/04/99  RT        exported trace class                                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCTRACE_H_
#define _OPCTRACE_H_

#include "GenObj.h"
#include "GenFunctions.h"
#include <mmsystem.h>

//-----------------------------------------------------------------------------
// Trace Groups                                                               |
//-----------------------------------------------------------------------------

#define TG_GEN		 0x00000001			
#define TG_ENG		 0x00010000
#define TG_SES		 0x00020000
#define TG_REQ		 0x00002000		
#define TG_CACHE	 0x00001000			
#define TG_NS		 0x00000010		
#define TG_SRV		 0x00000100			
#define TG_GRP		 0x00000200			
#define TG_ITEM		 0x00000400			
#define TG_COM		 0x01000000		
#define TG_TAC		 0x00000800		
#define TG_TREE		 0x00000002		
#define TG_OTHER	 0x08000000		
#define TG_OBJLOGGEN 0x10000000		
#define TG_OBJLOGCOM 0x20000000		
#define TG_TIMECOM   0x02000000		

#define TG_USER1	 0x00100000		
#define TG_USER2	 0x00200000		
#define TG_USER3	 0x00400000		
#define TG_USER4	 0x00800000		
#define TG_USER		 0x00F00000		


//-----------------------------------------------------------------------------
// Trace Levels                                                               |
//-----------------------------------------------------------------------------

#define TL_ERR	0
#define TL_WRN	1
#define TL_INF	2
#define TL_DEB	3


//-----------------------------------------------------------------------------
// GenericTraceHandler                                                        |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT GenericTraceHandler : public GenericObject
{
public:
	GenericTraceHandler();
	virtual void onTrace(IN LPCTSTR traceString, IN USHORT level, IN DWORD mask, IN LPCTSTR objId, IN LPCTSTR text);
	virtual void getUserMaskString(IN DWORD mask, OUT LPTSTR string);

	void resetObjTracePosition(void);
}; // GenericTraceHandler

inline void GenericTraceHandler::resetObjTracePosition(void)
{ m_objectTracePos = NULL; }




//-----------------------------------------------------------------------------
// OPCTrace                                                                   |
//-----------------------------------------------------------------------------

class OPCENGINE_EXPORT OPCTrace
{
public:	
	OPCTrace();
	~OPCTrace();

	DWORD m_traceLevel[4];		// trace levels
#ifdef _AFXDLL
	CCriticalSection m_lock;	// trace output lock
#else
	CMutex m_lock;				// trace output lock
#endif
	MMTIME m_timeStart, m_timeStop;
	BYTE m_startedPeriod;

	// do trace string
	void trace(IN USHORT level, IN DWORD mask, IN LPCTSTR objId, IN LPCTSTR text);

	void error(IN USHORT level, IN DWORD mask, 
			IN LPCTSTR objId, IN DWORD code, IN LPCTSTR text);


	// expand trance data
	LPCTSTR expand(IN LPCTSTR format, ...);

	LPCTSTR variant2String(LPVARIANT var);

	// time measurement
	LPCTSTR elapsedTime(void);

	// set trace levels
	void setLevels(IN DWORD error, IN DWORD warning, IN DWORD info, 
			IN DWORD debug);

	// set trace file
	BOOL setFile(IN LPCTSTR file, IN LPCTSTR secondFile = NULL, 
			IN DWORD maxFileSize  = 1000000, 
			IN DWORD creationDisposition = CREATE_ALWAYS);

	void setTraceHandler(IN GenericTraceHandler *traceHandler);
	GenericTraceHandler *getTraceHandler(void);

	CPtrList m_genObj;			// created generic object list
	CPtrList m_COMGrp;			// created COM group object list
	CPtrList m_COMSrv;			// created COM server object list

	CString m_variantBuffer;	// variant conversion buffer
	CString m_clipformatBuffer;	// clipformat conversion buffer
	CString m_vartypeBuffer;	// vartype conversion buffer
	CString m_qualityBuffer;	// quality conversion buffer
	CString m_filetimeBuffer;	// filetime conversion buffer
	CString m_timeBuffer;		// time measurement buffer

protected:
	HANDLE m_handleTrace;		// trace file handle
	HANDLE m_handleTrace2nd;	// second trace file handle
	HANDLE m_actHandle;			// current trace file handle
	DWORD m_fileSize;			// current trace file size
	TCHAR m_levelChar[4];		// trace level char
	TCHAR m_buffer[500];		// trace buffer
	TCHAR m_expand[500];		// expand buffer
	TCHAR m_errwrn[500];		// error and warning buffer
	USHORT m_len;				// trace string lenght
	DWORD m_lines;				// lines in trace file
	TCHAR m_userMaskStr[4];		// user mask string

	GenericTraceHandler *m_trcHandler;	
								// trace handler

	// get string for trace mask
	LPCTSTR getMaskString(DWORD mask);

	void closeTraceFiles(void);
}; // OPCTrace

inline GenericTraceHandler *OPCTrace::getTraceHandler(void)
{ if (m_trcHandler) m_trcHandler->addRef(); return m_trcHandler; }



//-----------------------------------------------------------------------------
// Global Functions                                                           |
//-----------------------------------------------------------------------------

// get pointer of the OPC trace object
OPCENGINE_EXPORT OPCTrace *getOPCTrace(void);


//-----------------------------------------------------------------------------
// Tracing Macros                                                             |
//-----------------------------------------------------------------------------

#ifdef OPCENGINE
// macros for toolkit files

extern OPCTrace g_trace;

#define VARIANT2STR(a) (g_trace.variant2String(a)) 
#define CLIPFORMAT2STR(a) (clipformat2string(&g_trace.m_clipformatBuffer, a))
#define QUALITY2STR(a) (quality2string(&g_trace.m_qualityBuffer, a)) 
#define VARTYPE2STR(a) (vartype2string(&g_trace.m_vartypeBuffer, a)) 
#define FILETIME2STR(a) (filetime2string(&g_trace.m_filetimeBuffer, a)) 

#define _TRACE_CREATE_GENOBJ()							\
{														\
	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGGEN)	\
	{													\
		m_objectTracePos = g_trace.m_genObj.AddTail(this);		\
	}													\
}

#define _TRACE_CREATE_COMGRP()							\
{														\
	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGCOM)	\
	{													\
		m_objectTracePos = g_trace.m_COMGrp.AddTail(this);		\
	}													\
}

#define _TRACE_CREATE_COMSRV()							\
{														\
	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGCOM)	\
	{													\
		m_objectTracePos = g_trace.m_COMSrv.AddTail(this);		\
	}													\
}

#define _TRACE_DELETE_GENOBJ()							\
{														\
	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGGEN)	\
	{													\
		if (m_objectTracePos)							\
			g_trace.m_genObj.RemoveAt(m_objectTracePos);		\
	}													\
}

#define _TRACE_DELETE_COMGRP()							\
{														\
	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGCOM)	\
	{													\
		if (m_objectTracePos)							\
			g_trace.m_COMGrp.RemoveAt(m_objectTracePos);		\
	}													\
}

#define _TRACE_DELETE_COMSRV()							\
{														\
	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGCOM)	\
	{													\
		if (m_objectTracePos)							\
			g_trace.m_COMSrv.RemoveAt(m_objectTracePos);		\
	}													\
}

// trace with object id
#define _TRACE(level, mask, y)							\
{														\
	if (g_trace.m_traceLevel[level] & mask)				\
	{													\
		g_trace.m_lock.Lock();							\
		try	{ g_trace.trace(level, mask, this->getObjTraceId(), g_trace.expand y);} \
		catch (...) { g_trace.m_lock.Unlock();}			\
		g_trace.m_lock.Unlock(); 						\
	}													\
}														

// trace with external object id
#define _TRACEO(level, mask, y)							\
{														\
	if (g_trace.m_traceLevel[level] & mask)				\
	{													\
		g_trace.m_lock.Lock();							\
		try	{ g_trace.trace(level, mask, m_obj->getObjTraceId(), g_trace.expand y);} \
		catch (...) { g_trace.m_lock.Unlock();}			\
		g_trace.m_lock.Unlock();						\
	}													\
}														

// trace without object id
#define _TRACEC(level, mask, id, y)						\
{														\
	if (g_trace.m_traceLevel[level] & mask)				\
	{													\
		g_trace.m_lock.Lock();							\
		try	{ g_trace.trace(level, mask, id, g_trace.expand y);} \
		catch (...) { g_trace.m_lock.Unlock();}			\
		g_trace.m_lock.Unlock();						\
	}													\
}														

#define _ERROR(mask, code, y)							\
{														\
	g_trace.m_lock.Lock();								\
	try	{ g_trace.error(TL_ERR, mask, this->getObjTraceId(), code, g_trace.expand y);} \
	catch (...) { g_trace.m_lock.Unlock();}			\
	g_trace.m_lock.Unlock();							\
}

#define _WARNING(mask, code, y)							\
{														\
	g_trace.m_lock.Lock();								\
	try	{ g_trace.error(TL_WRN, mask, this->getObjTraceId(), code, g_trace.expand y);} \
	catch (...) { g_trace.m_lock.Unlock();}				\
	g_trace.m_lock.Unlock();							\
}

#define _TRACE_START_TIME(level, mask)					\
{														\
	if (g_trace.m_traceLevel[level] & mask)				\
	{													\
		if (!g_trace.m_startedPeriod)					\
		{												\
			timeBeginPeriod(1);							\
			g_trace.m_startedPeriod = TRUE; 			\
		}												\
		timeGetSystemTime(&g_trace.m_timeStart, sizeof(MMTIME));	\
	}													\
}

#define _TRACE_STOP_TIME(level, mask)					\
{														\
	if (g_trace.m_traceLevel[level] & mask)				\
	{													\
		timeGetSystemTime(&g_trace.m_timeStop, sizeof(MMTIME));	\
	}													\
}

#define _TRACE_ELAPSED_TIME (g_trace.elapsedTime())

#else
// macros for user files

#define VARIANT2STR(a) (::getOPCTrace()->variant2String(a)) 
#define CLIPFORMAT2STR(a) (clipformat2string(&::getOPCTrace()->m_clipformatBuffer, a))
#define QUALITY2STR(a) (quality2string(&::getOPCTrace()->m_qualityBuffer, a)) 
#define VARTYPE2STR(a) (vartype2string(&::getOPCTrace()->m_vartypeBuffer, a)) 
#define FILETIME2STR(a) (filetime2string(&::getOPCTrace()->m_filetimeBuffer, a)) 

// trace with object id
#define _TRACE(level, mask, y)							\
{														\
	OPCTrace *trace = ::getOPCTrace();					\
	if (trace->m_traceLevel[level] & mask)				\
	{													\
		trace->m_lock.Lock();							\
		try	{ trace->trace(level, mask, this->getObjTraceId(), trace->expand y);} \
		catch (...) { trace->m_lock.Unlock();}			\
		trace->m_lock.Unlock(); 						\
	}													\
}														

// trace without object id
#define _TRACEC(level, mask, id, y)						\
{														\
	OPCTrace *trace = ::getOPCTrace();					\
	if (trace->m_traceLevel[level] & mask)				\
	{													\
		trace->m_lock.Lock();							\
		try	{ trace->trace(level, mask, id, trace->expand y);} \
		catch (...) { trace->m_lock.Unlock();}			\
		trace->m_lock.Unlock();						\
	}													\
}														

#define _ERROR(mask, code, y)							\
{														\
	OPCTrace *trace = ::getOPCTrace();					\
	trace->m_lock.Lock();								\
	try	{ trace->error(TL_ERR, mask, this->getObjTraceId(), code, trace->expand y);} \
	catch (...) { trace->m_lock.Unlock();}			\
	trace->m_lock.Unlock();							\
}

#define _WARNING(mask, code, y)							\
{														\
	OPCTrace *trace = ::getOPCTrace();					\
	trace->m_lock.Lock();								\
	try	{ trace->error(TL_WRN, mask, this->getObjTraceId(), code, trace->expand y);} \
	catch (...) { trace->m_lock.Unlock();}			\
	trace->m_lock.Unlock();							\
}

#define _TRACE_START_TIME(level, mask)					\
{														\
	OPCTrace *trace = ::getOPCTrace();					\
	if (trace->m_traceLevel[level] & mask)				\
	{													\
		if (!trace->m_startedPeriod)					\
		{												\
			timeBeginPeriod(1);							\
			trace->m_startedPeriod = TRUE;				\
		}												\
		timeGetSystemTime(&trace->m_timeStart, sizeof(MMTIME));	\
	}													\
}

#define _TRACE_STOP_TIME(level, mask)					\
{														\
	OPCTrace *trace = ::getOPCTrace();					\
	if (trace->m_traceLevel[level] & mask)				\
	{													\
		timeGetSystemTime(&trace->m_timeStop, sizeof(MMTIME));	\
	}													\
}

#define _TRACE_ELAPSED_TIME (::getOPCTrace()->elapsedTime())

#endif

#endif
