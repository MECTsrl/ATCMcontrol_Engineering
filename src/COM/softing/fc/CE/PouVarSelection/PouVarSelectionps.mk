
PouVarSelectionps.dll: dlldata.obj PouVarSelection_p.obj PouVarSelection_i.obj
	link /dll /out:PouVarSelectionps.dll /def:PouVarSelectionps.def /entry:DllMain dlldata.obj PouVarSelection_p.obj PouVarSelection_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PouVarSelectionps.dll
	@del PouVarSelectionps.lib
	@del PouVarSelectionps.exp
	@del dlldata.obj
	@del PouVarSelection_p.obj
	@del PouVarSelection_i.obj
