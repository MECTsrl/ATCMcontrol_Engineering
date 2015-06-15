' ATCM. All rights reserved
function SubscribeOPCList (varList, path)
	on error resume next
	SubscribeOPCList = CP_getProxy().SubscribeStringList(varList, path)
	if (Err.Number <> 0) then
		writeErrorMessage("proxy error while subscribing variable list: " + Err.Description)
	end if
end function

function UnsubscribeOPCList (varList, path)
	on error resume next
	UnsubscribeOPCList = CP_getProxy().UnsubscribeStringList(varList, path)
end function

function ReadOPCList (varList, path)
	on error resume next
	ReadOPCList= CP_getProxy().ReadStringList(varList, path)
	if (Err.Number <> 0) then
		writeErrorMessage("proxy error while reading variable list: " + Err.Description)
	end if
end function

function ReadOPCListTyped (varList, path)
	on error resume next
	ReadOPCListTyped = CP_getProxy().ReadStringListTyped(varList, path)
	if (Err.Number <> 0) then
		writeErrorMessage("proxy error while reading variable list: " + Err.Description)
	end if
end function

function ReadOPCListTypedAndRaw (varList, path)
	on error resume next
	ReadOPCListTypedAndRaw = CP_getProxy().ReadStringListTypedAndRaw(varList, path)
	if (Err.Number <> 0) then
		writeErrorMessage("proxy error while reading variable list: " + Err.Description)
	end if
end function

function WriteOPCValue (varName, value)
	on error resume next
	WriteOPCValue = CP_getProxy().Write(varName, value)
	if (Err.Number <> 0) then
		writeErrorMessage("proxy error while writing " + varName + ": " + Err.Description)
	end if
end function

' @DEPRECATED -- backward compatibility only!
function ReadCachedOPCListTyped (varList, path)
	ReadCachedOPCListTyped = ReadOPCListTyped (varList, path)
end function