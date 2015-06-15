<SCRIPT RUNAT=SERVER LANGUAGE=VBScript>
' ATCM. All rights reserved

dim ConfStream
dim NextObjectName
' The following variables serve for expressive error messages only.
dim ConfigFileNameG
dim ObjectNameG

'********************************************************************
function FileExists (ByVal FileName)
'********************************************************************
	on error resume next
	FileName = Server.MapPath (FileName)
	Set FileObject=Server.CreateObject("Scripting.FileSystemObject")
	Set InStream = FileObject.OpenTextFile (FileName, 1, FALSE, FALSE)
	if err = 0 then
		FileExists = TRUE
		InStream.Close()
	else
		FileExists = FALSE
	end if
end function

'********************************************************************
function openConfFile(ConfFile)
'********************************************************************
	openConfFile = false
	NextObjectName = ""
	Set fs = Server.CreateObject("Scripting.FileSystemObject")
	if (FileExists (ConfFile) = True) then
		set ConfStream = fs.OpenTextFile(server.mappath(ConfFile),1,FALSE)
		openConfFile = true		
		ConfigFileNameG = ConfFile
	end if
end	function

'********************************************************************
function closeConfFile()
'********************************************************************
	ConfStream.Close()
	NextObjectName = ""
end	function

'********************************************************************
function readConfObjectName()
'********************************************************************
	dim TmpLine
	
	if (len(NextObjectName) > 0) then
		readConfObjectName = NextObjectName
		NextObjectName = ""
		exit function
	else
		readConfObjectName = ""
		do while ConfStream.AtEndOfStream <> True
			TmpLine = ConfStream.ReadLine()
			if len(TmpLine) > 0 then
				if asc(TmpLine) <> 32 then
					readConfObjectName = RTrim(TmpLine)
					exit function
				end if
			end if
		loop
	end if
	ObjectNameG = readConfObjectName
end	function

'********************************************************************
function readConfValues(ConfValues)
'********************************************************************
	dim ModifiedName
	dim TmpLine
	dim temp
	dim name
	dim value

	temp = confValues.RemoveAll
	readConfValues = 0
		' Reset counters for values of new object.
	resetCounters()
	do while ConfStream.AtEndOfStream <> True
		TmpLine = ConfStream.ReadLine()
		if len(TmpLine) > 0 then
			if asc(TmpLine) <> 32 then
				NextObjectName = RTrim(TmpLine)
				exit function
			end if 
			if (getNameAndValue(TmpLine, name, value)) then
				ModifiedName = modifyName(name)
				if ConfValues.Exists(ModifiedName) = True then
					writeError "???", ObjectNameG, "Tag """ + name + """ not unique for this object in configuration file """ + ConfigFileNameG + """"
				end if
				if (value = "") then
					writeError "???", ObjectNameG, "No value for fag """ + name + """ of this object in configuration file """ + ConfigFileNameG + """"
				end if
				ConfValues.add ModifiedName,value
				readConfValues = readConfValues + 1
			end if
		end if
	loop
end	function

dim InCounter, OutCounter, InoutCounter
	' Initialize counters.
resetCounters()

'********************************************************************
function resetCounters()
'********************************************************************
	' Reset the counters corresponding to the names 'IN', 'OUT', and
	'INOUT'.

	InCounter = 1
	OutCounter = 1
	InoutCounter = 1
end function

'********************************************************************
function modifyName(Name)
'********************************************************************
	' If Name equals 'IN', 'OUT', or 'INOUT', Name is concatenated
	' with the value corresponding counter.
	' The counter is incremented.

    dim Counter
	select case Name
		case "IN"
			Counter = InCounter
			InCounter = InCounter + 1
		case "OUT"
		    Counter = OutCounter
			OutCounter = OutCounter + 1
		case "INOUT"
			Counter = InoutCounter
			InoutCounter = InoutCounter + 1 
		case else
			Counter = ""
	end select
	modifyName = Name & Counter
end function

'********************************************************************
function getConfValues(ConfFile, ObjName, ConfValues)
'********************************************************************
	' ObjName name of function block, variable, etc.
	' confValues directory object of configured values
	' return true, if found, else false

	ObjectNameG = ObjName

	dim l
	Trim(ObjName)
	
	if (openConfFile(ConfFile) = true) then
		dim CurrName
		do 
			CurrName = readConfObjectName()
			l = len(CurrName)
			if (l > 0) then 
				if (CurrName = ObjName) then
					readConfValues(ConfValues)
					getConfValues = true
					exit function
				end if
			end if
		loop while (l <> 0)
		closeConfFile()
	end if
	getConfValues = false
end	function

'********************************************************************
function getNameAndValue(zeile, name, value)
'********************************************************************
	' split zeile at semicolons,
	' name is substring before first semcolon
	' value ist substring after first semicolon

	Dim found, firstsemicolon
	found = False
	zeile=Trim(zeile)
	If zeile <> "" Then
		firstsemicolon = InStr(zeile,";")
		If firstsemicolon = 0 Then
			name = zeile
			value = ""
			found = True
		Else
			name = Trim(left(zeile,firstsemicolon-1))
			value = Right(zeile,Len(zeile)-firstsemicolon)
			value=Trim(value)
			found = True
		End If
	End If
	getNameAndValue = found
end function

</SCRIPT>
