

#include <iostream.h>
#include "stdafx.h"
#include "XML_Dings.h"
#include "SFCPatcher.h"
#include "PatchCommandLine.h"


void ErrorMessage(const CString& message);
void ExecuteHelp();

int main(int argc, char* argv[])
{
	HRESULT hr=CoInitializeEx(NULL, COINIT_APARTMENTTHREADED );
	if (hr != S_OK)
	{
		ErrorMessage("CoInitialize failed!");
		return 1;
	}

	int retValue=0;
	{
		CPatchCommandLine cmd;
		retValue=cmd.Parse(argc, argv);
		if (retValue == 0)
		{

			CSFCPatcher patcher(cmd.m_strProjectPath);
			HRESULT hr=patcher.Patch(cmd.m_strGrFile, cmd.m_strPatchFile, cmd.m_strSFCPath, cmd.m_bOptionF);
//			HRESULT hr=patcher.Copy(cmd.m_strGrFile,cmd.m_strPatchFile);
			if (hr != S_OK)
			{
				retValue=1;
				CString strMessage;
				patcher.GetLastError(strMessage);
				ErrorMessage(strMessage);
			}
		}
		else
		{
			ExecuteHelp();
		}
	}

	CoUninitialize();
	return retValue;
}

void ErrorMessage(const CString& message)
{
	cerr << _T("SfcPatch: ") << message << endl;
}

void ExecuteHelp()
{
	cout << "SFCPatch librarian command line tool                       "<< endl;
	cout << "usage:                                                     "<< endl;
	cout << "SFCPatch <project path> <4GR File> <POU>|<DBIFILE> <output file> [-f]"<< endl;
	cout << "<project path> path to ATCMControl project                    "<< endl;
	cout << "<4GR File> filename of SFC file                            " << endl;
	cout << "<POU> path of POU (instance) in ATCMControl project if f option is not set" << endl;
	cout << "<DBIFILE> path of debug info file of POU (instance) in ATCMControl project if f option is set" << endl;
	cout << "<output file> file name of patched SFC file, (will be created)" << endl;
	cout << "option -f : if set, third parameter is taken as debug info file, else as path of POU instance" << endl;
}