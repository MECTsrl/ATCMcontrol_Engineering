//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : OPCTRACE.CPP                                                |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//  Author      : ATCM                                                |
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

#include "stdafx.h"
#include "OPCTrace.h"
#include "GenObj.h"
#include "buildnr.h"

#ifdef SERVER
#include "OPCEngine.h"
#include "OPCGroup.h"
#include "OPCServer.h"
#endif

OPCTrace g_trace;		// trace object


//-----------------------------------------------------------------------------
// GenericTraceHandler                                                        |
//-----------------------------------------------------------------------------

GenericTraceHandler::GenericTraceHandler()
: GenericObject(GENOBJ_TYPE_CALLBACK)
{
	m_objectTraceId.Format(_T("[%s|%8.8X| %s]"), getObjTypeString(), this, _T("TRACE"));
}


//-----------------------------------------------------------------------------
// onTrace
//
// - callback called on every trace call to do external things
//
void GenericTraceHandler::onTrace(
	IN LPCTSTR traceString,	// whole trace string (toolbox format)
	IN USHORT level,		// trace level
	IN DWORD mask,			// trace group
	IN LPCTSTR objId,		// object id string
	IN LPCTSTR text)		// trace output
{} // onTrace


//-----------------------------------------------------------------------------
// getUserMaskString
//
// - fill mask string for user groups
//
void GenericTraceHandler::getUserMaskString(
	IN DWORD mask,		// trace group
	OUT LPTSTR string)	// mask string
{
	switch (mask)
	{
		case TG_USER1: _tcscpy(string, _T("U1")); break;
		case TG_USER2: _tcscpy(string, _T("U2")); break;
		case TG_USER3: _tcscpy(string, _T("U3")); break;
		case TG_USER4: _tcscpy(string, _T("U4")); break;
	}
} // getUserMaskString




//-----------------------------------------------------------------------------
// OPCTrace                                                                   |
//-----------------------------------------------------------------------------

OPCTrace *getOPCTrace(void)
{
	return &g_trace;
}

OPCTrace::OPCTrace()
{
	m_handleTrace = INVALID_HANDLE_VALUE;
	m_handleTrace2nd = INVALID_HANDLE_VALUE;
	m_actHandle = INVALID_HANDLE_VALUE;
	m_fileSize = ULONG_MAX;
	m_lines = 0;

	m_traceLevel[TL_ERR] = 0xFFFFFFFF;
	m_traceLevel[TL_WRN] = 0xFFFFFFFF;
#ifdef _DEBUG
	m_traceLevel[TL_INF] = 0xFFFFFFFF;
	m_traceLevel[TL_DEB] = TG_OBJLOGGEN | TG_OBJLOGCOM;
#else
	m_traceLevel[TL_INF] = 0;
	m_traceLevel[TL_DEB] = TG_OBJLOGGEN | TG_OBJLOGCOM;
#endif
	m_levelChar[TL_ERR] = 'E';
	m_levelChar[TL_WRN] = 'W';
	m_levelChar[TL_INF] = 'I';
	m_levelChar[TL_DEB] = ' ';

	m_trcHandler = NULL;
	m_startedPeriod = FALSE;
}

OPCTrace::~OPCTrace()
{
	// print object trace
	POSITION pos;

#ifdef SERVER
	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGCOM)
	{
		OPCServer *srv;
		OPCGroup *grp;

		pos = m_COMSrv.GetHeadPosition();
		while (pos)
		{
			srv = (OPCServer *)m_COMSrv.GetNext(pos);
			trace(TL_DEB, TG_OBJLOGCOM, srv->getObjTraceId(), 
					expand(_T("refCount %lu"), srv->m_dwRef));	
		}
		m_COMSrv.RemoveAll();

		pos = m_COMGrp.GetHeadPosition();
		while (pos)
		{
			grp = (OPCGroup *)m_COMGrp.GetNext(pos);
			trace(TL_DEB, TG_OBJLOGCOM, grp->getObjTraceId(), 
					expand(_T("refCount %lu"), grp->m_dwRef));	
		}
		m_COMGrp.RemoveAll();
	}
#endif

	if (g_trace.m_traceLevel[TL_DEB] & TG_OBJLOGGEN)
	{
		GenericObject *genObj;
	
		pos = m_genObj.GetHeadPosition();
		while (pos)
		{
			genObj = (GenericObject *)m_genObj.GetNext(pos);
			if (genObj != m_trcHandler)
			{
				trace(TL_DEB, TG_OBJLOGGEN, genObj->getObjTraceId(), 
					expand(_T("refCount %lu"), genObj->getRefCount()));	
			}
		}
		m_genObj.RemoveAll();
	}

	if (m_trcHandler)
		m_trcHandler->resetObjTracePosition();
	setTraceHandler(NULL);
	closeTraceFiles();

	if (m_startedPeriod)
		timeEndPeriod(1);
}


//-----------------------------------------------------------------------------
// expand
//
// - expand the string specified with format and ... arguments
//
// returns:
//		pointer to expanded string (m_expand)
//
LPCTSTR OPCTrace::expand(
	IN LPCTSTR format,	// format string
	IN ...)				// arguments
{
	va_list arglist;
	va_start(arglist, format);
	_vstprintf(m_expand, format, arglist);
	return m_expand;
} // expand


//-----------------------------------------------------------------------------
// trace
//
// - trace text to trace window and file
//
void OPCTrace::trace(
	IN USHORT level,	// trace level 
	IN DWORD mask,		// trace group
	IN LPCTSTR objId,	// object identification string
	IN LPCTSTR text)	// text to trace
{
	SYSTEMTIME now;
	
	::GetSystemTime(&now);

	_stprintf(m_buffer, _T("%c %s %03.3d %02.2d-%02.2d:%02.2d:%02.2d.%03.3d %-35.35s %s\r\n"), 
			  m_levelChar[level], getMaskString(mask), GetCurrentThreadId(),
			  now.wDay, now.wHour, now.wMinute, now.wSecond, now.wMilliseconds,
			  objId, text);
	m_len = _tcslen(m_buffer);

	::AfxTrace(_T("%s"), m_buffer);

	if (m_trcHandler)
	{
		m_trcHandler->onTrace(m_buffer, level, mask, objId, text);
	}

	if (m_actHandle != INVALID_HANDLE_VALUE)
	{
		USES_CONVERSION;
		DWORD len;
		// unicode to ansi
		LPSTR ansiBuf = T2A(m_buffer);
		::WriteFile(m_actHandle, ansiBuf, m_len, &len, NULL);
		m_lines++;

		if (m_handleTrace2nd != INVALID_HANDLE_VALUE)
		{
			// 2 trace files
			if ((m_lines & 0xff) == 0)
			{
				DWORD sizeLow;
				sizeLow = GetFileSize (m_actHandle, NULL);
				if (sizeLow != 0xFFFFFFFF)
				{ 
					if (sizeLow > m_fileSize)
					{
						// change files
						if (m_actHandle == m_handleTrace)
							m_actHandle = m_handleTrace2nd;
						else
							m_actHandle = m_handleTrace;
						SetFilePointer(m_actHandle, 0, NULL, FILE_BEGIN);
						SetEndOfFile(m_actHandle);
					}
				}
			}
		}
	}
} // trace


//-----------------------------------------------------------------------------
// setLevels
//
// - set trace levels
//
void OPCTrace::setLevels(
	IN DWORD error,		// error level
	IN DWORD warning,	// warning level
	IN DWORD info,		// info level
	IN DWORD debug)		// debug level
{
	m_traceLevel[TL_ERR] = error;
	m_traceLevel[TL_WRN] = warning;
	m_traceLevel[TL_INF] = info;
	m_traceLevel[TL_DEB] = debug;
} // setLevels


//-----------------------------------------------------------------------------
// setFile
//
// - set new trace file
// - opens the trace file
//
// returns:
//		TRUE -  new trace file opened
//		FALSE - error open trace file 
//
BOOL OPCTrace::setFile(
	IN LPCTSTR file,						// file name
	IN OPTIONAL LPCTSTR secondFile,			// second file name
	IN OPTIONAL DWORD maxFileSize,			// max file size
	IN OPTIONAL DWORD creationDisposition)	// how to open the file
{ 
	BOOL ret;

	closeTraceFiles();

	m_handleTrace = ::CreateFile(file, GENERIC_WRITE, FILE_SHARE_READ, NULL, 
		creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

	if (secondFile)
	{
		m_handleTrace2nd = ::CreateFile(secondFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, 
		creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

		m_fileSize = maxFileSize;

		ret = ((m_handleTrace != INVALID_HANDLE_VALUE) &&
			   (m_handleTrace2nd != INVALID_HANDLE_VALUE));
	}
	else
	{
		m_handleTrace2nd = INVALID_HANDLE_VALUE;
		m_fileSize = ULONG_MAX;

		ret = (m_handleTrace != INVALID_HANDLE_VALUE);
	}


	if (ret)
	{
		m_actHandle = m_handleTrace;

#ifdef SERVER
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("OPC Server Toolkit (ATCM)")));
#else
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("OPC Client Toolkit (ATCM)")));
#endif
 		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("Version %lu.%2.2lu Build %lu"), MAJOR_VERSION_NUMBER, MINOR_VERSION_NUMBER, BUILD_NUMBER));
#ifdef DEMO_VERSION
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("Demo")));
#else
#ifdef _DEBUG
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("Debug")));
#else
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("Release")));
#endif
#endif
#ifdef _AFXDLL
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("Shared")));
#else
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("Static")));
#endif

#ifdef _UNICODE
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("UNICODE")));
#else
		_TRACEC(TL_INF, TG_OTHER, _T("INFO"), (_T("ASCII")));
#endif
	}
	else
	{
		closeTraceFiles();
	}

	return ret; 
} // setFile


//-----------------------------------------------------------------------------
// closeTraceFiles
//
// - close the trace files
//
void OPCTrace::closeTraceFiles(void)
{
	if (m_handleTrace != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handleTrace);	
		m_handleTrace = INVALID_HANDLE_VALUE;
	}
	if (m_handleTrace2nd != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_handleTrace2nd);	
		m_handleTrace2nd = INVALID_HANDLE_VALUE;
	}
} // closeTraceFiles


//-----------------------------------------------------------------------------
// error
//
// - trace error and call error method
//
void OPCTrace::error(
	IN USHORT level,	// level error or warning
	IN DWORD mask,		// trace mask
	IN LPCTSTR objId,	// object id
	IN DWORD code,		// error code
	IN LPCTSTR text)	// trace text
{
	_tcscpy(m_errwrn, text);
	LPCTSTR string = expand(_T("0x%8.8X - %s"), code, m_errwrn);
	trace(level, mask, objId, string);	
#ifdef SERVER
	GenericPointer<OPCEngine> engine = ::getOPCEngine();			
	engine->error(code, m_errwrn, (level == TL_ERR));					
#endif
} // error


//-----------------------------------------------------------------------------
// variant2String
//
// - variant2string with size check
//
// returns:
//		pointer to string
//
LPCTSTR OPCTrace::variant2String(
	IN LPVARIANT var)	// variant
{
	::variantWithType2string(&m_variantBuffer, var);
	if (m_variantBuffer.GetLength() > 300)
	{
		CString cpy = (LPCTSTR)m_variantBuffer;
		m_variantBuffer.Format(_T("%-.300s"), cpy);
	}
	return (LPCTSTR)m_variantBuffer;
} // variant2String


//-----------------------------------------------------------------------------
// elapsedTime
//
// - calculate elapsed time and convert it to a string
//
// returns:
//		pointer to string
//
LPCTSTR OPCTrace::elapsedTime(void)
{ 
	m_timeBuffer.Format(_T("%lu"),  m_timeStop.u.ms - m_timeStart.u.ms); 
	return (LPCTSTR)m_timeBuffer;
} // elapsedTime


//-----------------------------------------------------------------------------
// setTraceHandler
//
// - set trace handler object
//
void OPCTrace::setTraceHandler(
	IN GenericTraceHandler *trace)	// trace handler object
{ 
	if (m_trcHandler)
		m_trcHandler->release();
 
	_TRACEC(TL_DEB, TG_OTHER, _T("TRACE"), (_T("set trace handler 0x%X"), trace));
	m_trcHandler = trace; 

	if (m_trcHandler)
		m_trcHandler->addRef();
} // setTraceHandler


//-----------------------------------------------------------------------------
// getMaskString
//
// - get string of tracing group
//
// returns:
//		string of tracing group
//
LPCTSTR OPCTrace::getMaskString(
	IN DWORD mask)	// trace group
{
	switch (mask)
	{
		case TG_GEN: return _T("GEN"); break;
		case TG_ENG: return _T("ENG"); break;
		case TG_SES: return _T("SES"); break;
		case TG_REQ: return _T("REQ"); break;
		case TG_CACHE: return _T("CAH"); break;
		case TG_NS: return _T("NS "); break;
		case TG_SRV: return _T("SRV"); break;
		case TG_GRP: return _T("GRP"); break;
		case TG_ITEM: return _T("ITM"); break;
		case TG_COM: return _T("COM"); break;
		case TG_TAC: return _T("TAC"); break;
		case TG_TREE: return _T("TRE"); break;
		case TG_OTHER: return _T("..."); break;
		case TG_OBJLOGGEN: return _T("OLG"); break;
		case TG_OBJLOGCOM: return _T("OLC"); break;
		case TG_TIMECOM: return _T("TIM"); break;
		case TG_USER1:
		case TG_USER2:
		case TG_USER3:
		case TG_USER4:
		{
			if (m_trcHandler)
			{
				m_trcHandler->getUserMaskString(mask, m_userMaskStr); 
				return m_userMaskStr;
			}
		}
		break;
	}

	return _T("   ");
} // getMaskString

