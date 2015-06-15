#ifndef _COMMDEFINITIONS_H_
#define _COMMDEFINITIONS_H_

#define COMM_CHANNEL_TCP        _T("TCP")
#define COMM_CHANNEL_NETBUI     _T("NETBEUI")	// NFTASKHANDLING 19.05.05 SIS
#define COMM_CHANNEL_RS232      _T("RS232")
#define COMM_CHANNEL_RS485	    _T("RS485")
#define COMM_CHANNEL_PIPE       _T("PIPE")

#define STD_RS232_PAR           _T("COM1:19200,n,1,0")

#define STD_PROTOCOL            _T("BLOCK")
#define STD_PROTOCOL_PARAMETER  _T("750")

// NFTASKHANDLING 19.05.05 SIS >>
#define PROTOCOL_SIMPLE			_T("SIMPLE")
#define STD_NETBUI_PAR			_T("1000")
// NFTASKHANDLING 19.05.05 SIS <<
#endif

