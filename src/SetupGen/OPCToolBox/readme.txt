EF  06.05.2007

These DLLs should only be used by \control\bin\IO_OPC.DLL !!

The source code and makefile for the DLLs,LIBs and PDBs 
can be found at $\4Control\COM\softing\fc\AddOns\Softing\4CPCTarget\IO_OPC\OPCToolbox\
The DDLs are all build for Release,shared and ASCII, for example:
   msdev socmn\SOCmn_vc6.dsp  /MAKE  "SOCmn - Win32 Release shared ASCII" /REBUILD

Important Note: The LIBs SODaCadd.lib, SOCltadd.lib and SOCmnadd.lib are in this folder ONLY that
                we can build the IO_OPC.DLL in debug mode.
                If the IO_OPC.DLL is build with DEBUG=1 it will link to the debug LIBs
                but we do dot include the debug DLLs here (we never testet the debug version of the IO_OPC.DLL!).



The DLLs,LIBs and PDBs are taken from the labeled version: 4101 


The original ToolBox Version is 3.11.305. 
There are no changes to this version except that we have PDBs also in
the release version and the 4C Build number 4101 is visible in the version info
of the DLLs.

