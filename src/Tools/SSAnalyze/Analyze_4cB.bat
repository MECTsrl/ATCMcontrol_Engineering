@ECHO off


@ECHO ON

@rem --------------------------------------------------------------------------------------------
@rem Runs the SS Analyze tools two times. The second run is to check if all problems was
@rem solved correctly.
@rem --------------------------------------------------------------------------------------------

@rem --------------------------------------------------------------------------------------------
@rem Delete old results
@rem --------------------------------------------------------------------------------------------
del \\sfiler\iad_4c_ssdb\ssdb_4cB\analyze\Result1\*.* /F /Q
del \\sfiler\iad_4c_ssdb\ssdb_4cB\analyze\Result2\*.* /F /Q

@rem --------------------------------------------------------------------------------------------
@rem Run Analyze
@rem --------------------------------------------------------------------------------------------
call C:\DevStudio\Common\VSS\win32\analyze -F -C -d -V4 -i- -B\\sfiler\iad_4c_ssdb\ssdb_4cB\analyze\Result1 \\sfiler\iad_4c_ssdb\ssdb_4cB\data
call C:\DevStudio\Common\VSS\win32\analyze -F -C -d -V4 -i- -B\\sfiler\iad_4c_ssdb\ssdb_4cB\analyze\Result2 \\sfiler\iad_4c_ssdb\ssdb_4cB\data

