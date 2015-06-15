
LibAddonps.dll: dlldata.obj LibAddon_p.obj LibAddon_i.obj
	link /dll /out:LibAddonps.dll /def:LibAddonps.def /entry:DllMain dlldata.obj LibAddon_p.obj LibAddon_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del LibAddonps.dll
	@del LibAddonps.lib
	@del LibAddonps.exp
	@del dlldata.obj
	@del LibAddon_p.obj
	@del LibAddon_i.obj
